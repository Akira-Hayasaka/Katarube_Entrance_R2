//
//  Deformer.cpp
//  ShapseMorph
//
//  Created by Akira Hayasaka on 7/31/16.
//
//

#include "Deformer.hpp"

void Deformer::setup(string filePath, ContourFinderSettings settings, int idx)
{
    this->filePath = filePath;
	this->idx = idx;

    ofDisableArbTex();
    ofLoadImage(origTex, filePath);
    ofEnableArbTex();
    ofLog() << "load " << filePath;
    
    tex.allocate(ONESCRN_W, ONESCRN_H);
    texForBinding.allocate(ONESCRN_W, ONESCRN_H);
    screen.allocate(ONESCRN_W, ONESCRN_H);
    
    tex.begin();
    ofClear(0);
    ofSetRectMode(OF_RECTMODE_CENTER);
    origTex.draw(tex.getWidth()/2, tex.getHeight()/2);
    ofSetRectMode(OF_RECTMODE_CORNER);
    tex.end();
    
    texForBinding.begin();
    ofClear(0);
    tex.draw(0, 0);
    texForBinding.end();
    
    screen.begin();
    ofClear(0);
    screen.end();
    
    ofPixels px;
    tex.readToPixels(px);
    
    ofxCv::ContourFinder contourFinder;
    contourFinder.setMinAreaRadius(settings.minArea);
    contourFinder.setMaxAreaRadius(settings.maxArea);
    contourFinder.setThreshold(settings.threshold);
    contourFinder.findContours(px);
    contourFinder.setFindHoles(settings.holes);
    px.clear();
    
    vector<ofPolyline> lines = contourFinder.getPolylines();
    
    ofLog() << "found contour = " << lines.size();
    if (lines.size() > 1)
        ofLogWarning() << "got extra contour for " << filePath << " !!";
    
    if (!lines.empty())
    {
        outline = lines.at(0);
        
        ofLog() << "controu area = " << outline.getArea();
        
        outline = outline.getResampledByCount(numOutlineVts);
        
        if (outline.size() > 5)
        {
            vector<glm::vec3> linePts = outline.getVertices();
            vector<ofPoint> alignedPts;
            
            int topLeftIdx = 0;
            ofPoint tmpStored;
            for (int i = 0; i < linePts.size(); i++)
            {
                if (i == 0)
                {
                    tmpStored = linePts.at(i);
                }
                else
                {
                    if (linePts.at(i).length() < tmpStored.length())
                    {
                        tmpStored = linePts.at(i);
                        topLeftIdx = i;
                    }
                }
            }
            ofLog() << "topLeftIdx = " << topLeftIdx;
            
            outline.clear();
            for (int i = 0; i < linePts.size(); i++)
            {
                int idx = topLeftIdx + i;
                if (idx >= linePts.size())
                    idx -= linePts.size();
                outline.addVertex(linePts.at(idx));
            }
            
            ofxTriangleMesh mesh;
            mesh.triangulate(outline, 28, -1);
            puppetWarp.setup(mesh.triangulatedMesh);
        }
        
        setCtrlPoints();
        getConvexHullEdges(contourFinder.getConvexHull(0));
        getCentroid();
        
        puppetWarp.getDeformedMesh().clearTexCoords();
        for (auto& v: puppetWarp.getDeformedMesh().getVertices())
        {
            puppetWarp.getDeformedMesh().addTexCoord(tex.getTexture().getCoordFromPoint(v.x, v.y));
        }
    }
    
	bMorphing = false;
	type = NONE;
	if (idx < 1)
	{
		initState = NOTYET;
		startThread(false);
	}
	else
	{
		initState = DONE;
	}

    //bMorphing = false;
    //type = NONE;
 //   initState = NOTYET;

 //   //startThread();
	//initState = DONE;
}

void Deformer::threadedFunction()
{
    while (isThreadRunning())
    {
        if (initState == NOTYET)
        {
            puppetWarp.update();
            initState = UPDATED;
            ofLog() << "done warp process for " << filePath;
        }
        ofSleepMillis(1000 * 0.1);
    }
}

void Deformer::update()
{
    if (initState == UPDATED)
    {
        waitForThread(true);
        initState = DONE;
        ofLog() << "initState = DONE; for " << filePath;
    }
    
    if (initState != DONE)
        return;
    
    if (bMorphing)
    {
        for (auto mp : morphProgresses)
        {
            puppetWarp.setControlPoint(mp.idx, mp.pts);
        }
        
        if (type == DEFORM && Globals::ELAPSED_TIME - lastSeqTime > 0.12 &&
            morphSeqIdx < Globals::morphSequence.size())
        {
			Globals::morphSequence.at(morphSeqIdx).begin();
            ofClear(0);
            ofPushMatrix();
            ofScale(MORPH_SEQ_RATIO, MORPH_SEQ_RATIO);
            texForBinding.getTexture().bind();
            puppetWarp.getDeformedMesh().draw();
            texForBinding.getTexture().unbind();
            ofPopMatrix();
			Globals::morphSequence.at(morphSeqIdx).end();
            
            lastSeqTime = Globals::ELAPSED_TIME;
            morphSeqIdx++;
        }
    }
    
    puppetWarp.update();
    
    texForBinding.begin();
    ofClear(0);
    ofPushStyle();
    ofSetColor(ofColor::white, bindTexAlpha);
    tex.draw(0, 0);
    ofPopStyle();
    texForBinding.end();
    
    if (!bDone)
    {
        if (Globals::ELAPSED_TIME - startMorphingTime > totalDur)
        {
            ofNotifyEvent(finEvent);
            for (auto& f : Globals::morphSequence)
            {
                f.begin();
                ofClear(0);
                f.end();
            }
            bDone = true;
        }
    }
    
    screen.begin();
    ofClear(0);
    texForBinding.getTexture().bind();
    puppetWarp.getDeformedMesh().draw();
    texForBinding.getTexture().unbind();
    
    if (type == RESTORE)
    {
        ofPushStyle();
        ofSetColor(ofColor::white, (255 - bindTexAlpha) * 0.6);
        puppetWarp.getDeformedMesh().drawWireframe();
        ofPopStyle();
        
        for (auto l : labelPoints)
        {
            ofVec2f p = puppetWarp.getDeformedMesh().getVertex(l.idx);
            
            ofVec2f offset;
            if (p.x < centroid.pts.x)
            {
                // 1
                if (p.y < centroid.pts.y)
                    offset.set(-100, -100);
                // 4
                else
                    offset.set(-100, 100);
            }
            else
            {
                // 2
                if (p.y < centroid.pts.y)
                    offset.set(100, -100);
                // 3
                else
                    offset.set(100, 100);
            }
            
            ofPushStyle();
            ofSetColor(ofColor::gray, 255 - bindTexAlpha);
            ofDrawLine(p, p + offset);
            ofDrawCircle(p, 4);
            ofPopStyle();
            
            ofPushStyle();
            ofColor bCol(ofColor::darkGray, 255 - bindTexAlpha);
            ofColor fCol(ofColor::white, 255 - bindTexAlpha);
            ofDrawBitmapStringHighlight(ofToString(p.x), p + offset, bCol, fCol);
            ofPopStyle();
        }
    }
    screen.end();
}

void Deformer::makeReadyToDeform(ofPolyline deformTo)
{
    if (initState != DONE)
        return;
    
    deformOutline = deformTo;
    
    alignNumVerts();
    
    morphDestinations.clear();
    morphProgresses.clear();
    for (int i = 0; i < edges.size(); i++)
    {
        VertexDef mdest;
        mdest.idx = edges.at(i).idx;
        mdest.pts = deformOutline.getVertices().at(i);
        morphDestinations.push_back(mdest);
        
        VertexDef mprog;
        mprog.idx = edges.at(i).idx;
        mprog.pts = edges.at(i).pts;
        morphProgresses.push_back(mprog);
    }
    
    bindTexAlpha = 255;
    type = DEFORM;
}

void Deformer::makeReadyToRestore(ofPolyline restoreFrom)
{
    if (initState != DONE)
        return;
    
    deformOutline = restoreFrom;

    alignNumVerts();
    
    morphDestinations.clear();
    morphProgresses.clear();
    for (int i = 0; i < edges.size(); i++)
    {
        VertexDef mdest;
        mdest.idx = edges.at(i).idx;
        mdest.pts = edges.at(i).pts;
        morphDestinations.push_back(mdest);
        
        VertexDef mprog;
        mprog.idx = edges.at(i).idx;
        mprog.pts = deformOutline.getVertices().at(i);
        morphProgresses.push_back(mprog);
    }
    
    // deform, then restore
    for (auto mp : morphProgresses)
    {
        puppetWarp.setControlPoint(mp.idx, mp.pts);
    }
    
    makeCentroidMovePath();
    bindTexAlpha = 0.0;
    type = RESTORE;
}

void Deformer::start()
{
    if (initState != DONE)
        return;
    
    for (int i = 0; i < morphProgresses.size(); i++)
    {
        Tweenzor::add(&morphProgresses.at(i).pts, morphProgresses.at(i).pts,
                      morphDestinations.at(i).pts, 0.0f, morphDur, EASE_OUT_SINE);
    }
    
    if (type == DEFORM)
        Tweenzor::add(&bindTexAlpha, bindTexAlpha, 0.0f, morphDur * 0.8f, 1.0f, EASE_OUT_SINE);
    else if (type == RESTORE)
    {
        Tweenzor::add(&bindTexAlpha, bindTexAlpha, 255.0f, morphDur * 0.8f, 1.0f, EASE_IN_SINE);
        Tweenzor::addCompleteListener(Tweenzor::getTween(&bindTexAlpha), this, &Deformer::onRestoreFinish);
    }
    
    if (type == RESTORE)
    {
        labelPoints.clear();
        for (int i = 0; i < 8; i++)
        {
            int idx = ofRandom(puppetWarp.getDeformedMesh().getVertices().size());
            VertexDef vd;
            vd.idx = idx;
            vd.pts = puppetWarp.getDeformedMesh().getVertex(idx);
            labelPoints.push_back(vd);
        }
    }
    
    for (auto& f : Globals::morphSequence)
    {
        f.begin();
        ofClear(0);
        f.end();
    }
    startMorphingTime = Globals::ELAPSED_TIME;
    bDone = false;
    bMorphing = true;
    lastSeqTime = startMorphingTime;
    seqPos.set(ofRandom(0, APP_W), ofRandom(ONESCRN_H - 100));
    morphSeqIdx = 0;
}

void Deformer::draw(ofVec3f rot)
{
    if (initState != DONE)
        return;
    
    if (bMorphing)
    {
        if (type == DEFORM)
        {
            for (int i = 0; i < morphSeqIdx; i++)
            {
                ofPoint p;
                if (seqPos.x < APP_W/2)
                    p.x = seqPos.x + Globals::morphSequence.at(i).getWidth() * i;
                else
                    p.x = seqPos.x - Globals::morphSequence.at(i).getWidth() * i;
                p.y = seqPos.y;
                ofPushStyle();
                ofSetColor(ofColor::white, bindTexAlpha);
				Globals::morphSequence.at(i).draw(p.x, p.y);
                ofNoFill();
                ofSetColor(ofColor::darkGray, bindTexAlpha);
                ofDrawRectangle(p, Globals::morphSequence.at(i).getWidth(), Globals::morphSequence.at(i).getHeight());
                ofPopStyle();
            }
        }
    }
    
    ofPushMatrix();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(ONESCRN_W + ONESCRN_W/2, ONESCRN_H/2);
//    ofRotateX(rot.x);
//    ofRotateY(rot.y);
//    ofRotateZ(rot.z);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    screen.draw(0, 0);
	glDisable(GL_BLEND);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
}

void Deformer::setCtrlPoints()
{
    edges.clear();
    vector<glm::vec3> pts = outline.getVertices();
    for (int i = 0; i < pts.size(); i++)
    {
        vector<IdxDist> edgeCandidate;
        for (int j = 0; j < puppetWarp.getOriginalMesh().getVertices().size(); j++)
        {
            IdxDist idist;
            idist.idx = j;
            idist.dist = glm::distance(puppetWarp.getOriginalMesh().getVertices().at(j), pts.at(i));
            idist.pts = puppetWarp.getOriginalMesh().getVertices().at(j);
            edgeCandidate.push_back(idist);
        }
        ofSort(edgeCandidate, compDistance);
        puppetWarp.setControlPoint(edgeCandidate.front().idx);
        edges.push_back(edgeCandidate.front());
    }
}

void Deformer::getConvexHullEdges(vector<cv::Point> hullEdge)
{
    ofPolyline hullLine;
	for (auto p : hullEdge)
	{
		glm::vec2 v = ofxCv::toOf(p);
        hullLine.addVertex(glm::vec3(v.x, v.y, 0.));
	}
    hullLine = hullLine.getResampledByCount(6);
    
    for (auto p : hullLine.getVertices())
    {
        int idx = 0;
        getNearestVertex(puppetWarp.getOriginalMesh(), p, idx);
        VertexDef vd;
        vd.idx = idx;
        vd.pts = puppetWarp.getOriginalMesh().getVertex(idx);
        convexHullPoints.push_back(vd);
    }
}

void Deformer::getCentroid()
{
    glm::vec3 cen = outline.getCentroid2D();
    vector<IdxDist> centroidCandidate;
    for (int i = 0; i < puppetWarp.getOriginalMesh().getVertices().size(); i++)
    {
        IdxDist idist;
        idist.idx = i;
        idist.dist = glm::distance(puppetWarp.getOriginalMesh().getVertices().at(i), cen);
        idist.pts = puppetWarp.getOriginalMesh().getVertices().at(i);
        centroidCandidate.push_back(idist);
    }
    ofSort(centroidCandidate, compDistance);
    centroid.idx = centroidCandidate.front().idx;
    centroid.pts = centroidCandidate.front().pts;
}

void Deformer::alignNumVerts()
{
    if (edges.size() > deformOutline.size())
    {
        ofPoint last = deformOutline.getVertices().back();
        for (int i = 0; i < edges.size() - deformOutline.getVertices().size(); i++)
        {
            deformOutline.addVertex(last);
        }
    }
}

void Deformer::makeCentroidMovePath()
{
    centroidMovePathIdx = 0;
    
    float rad = ofRandom(100, 200);
    ofVec2f orig = centroid.pts;
    int devide = 100;
    float angle = PI * 2 / devide;
    
    centroidMovePath.clear();
    centroidMovePath.addVertex(glm::vec3(orig.x, orig.y, 0.));
    for (int i = 0; i < devide; i++)
    {
        ofPoint p(orig.x + rad * cos(angle * i),
                  orig.y + rad * sin(angle * i));
        centroidMovePath.addVertex(p);
    }
    centroidMovePath = centroidMovePath.getResampledByCount(100);
}

float Deformer::getNearestVertex(const ofMesh& mesh, const ofVec2f& target, int& vertexIndex)
{
    float bestDistance = 0;
    for (int i = 0; i < mesh.getNumVertices(); i++)
    {
        float distance = target.distance(mesh.getVertex(i));
        if (distance < bestDistance || i == 0)
        {
            bestDistance = distance;
            vertexIndex = i;
        }
    }
    return bestDistance;
}

void Deformer::onRestoreFinish(float* arg)
{
    Tweenzor::removeCompleteListener(Tweenzor::getTween(&bindTexAlpha));
    
//    centroidMovePathIdx = 0;
//    Tweenzor::add(&centroidMovePathIdx, centroidMovePathIdx, (float)centroidMovePath.getVertices().size()-1.0f,
//                  0.0f, totalDur - (Globals::ELAPSED_TIME - startMorphingTime), EASE_IN_OUT_EXPO);
    
    Globals::testTex = origTex;
}