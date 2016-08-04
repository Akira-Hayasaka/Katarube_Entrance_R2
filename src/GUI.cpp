//
//  GUI.cpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#include "GUI.hpp"

void GUI::setup()
{
    drawingCFSetting.setup();
    drawingCFSetting.add(Globals::minArea.set("Min area", 10, 1, 100));
    drawingCFSetting.add(Globals::maxArea.set("Max area", 5000, 1, 5500));
    drawingCFSetting.add(Globals::threshold.set("Threshold", 128, 0, 255));
    drawingCFSetting.add(Globals::holes.set("Holes", false));
    
    // kinect device & contour gui
    kinectGUI.setup("kinect", "settings/kinect.xml");
    kinectGUI.add(Globals::kinectAngle.set("kinect head angle", 0.0, -30.0, 30.0));
    kinectGUI.add(Globals::kinectNearThresh.set("depth thresh near cm", KINECT_MIN_DIST, KINECT_MAX_DIST, KINECT_MIN_DIST));
    kinectGUI.add(Globals::kinectFarThresh.set("depth thresh far cm", KINECT_MAX_DIST, KINECT_MAX_DIST, KINECT_MIN_DIST));
    kinectGUI.add(Globals::kinectAdjustAngle.set("angle adjust", 0.0, -1.0, 1.0));
    kinectGUI.add(Globals::contourMinArea.set("contour min area", 10, 1, 100));
    kinectGUI.add(Globals::contourMaxArea.set("contour max area", 200, 1, 500));
    kinectGUI.add(Globals::contourThreshold.set("contour bri threshold", 128, 0, 255));
    kinectGUI.add(Globals::contourFindHole.set("find hole", false));
    kinectGUI.add(Globals::leftKinectPosX.set("leftKinectPosX", 0, -APP_W, APP_W));
    kinectGUI.add(Globals::leftKinectPosY.set("leftKinectPosY", 0, -APP_H, APP_H));
    kinectGUI.add(Globals::leftKinectAng.set("leftKinectAng", 0, -180, 180));
    kinectGUI.add(Globals::leftKinectScale.set("leftKinectScale", 1.0, 0.0, 4.0));
    kinectGUI.add(Globals::rightKinectPosX.set("rightKinectPosX", 0, -APP_W, APP_W));
    kinectGUI.add(Globals::rightKinectPosY.set("rightKinectPosY", 0, -APP_H, APP_H));
    kinectGUI.add(Globals::rightKinectAng.set("rightKinectAng", 0, -180, 180));
    kinectGUI.add(Globals::rightKinectScale.set("rightKinectScale", 1.0, 0.0, 4.0));
    
    Globals::kinectAngle.addListener(&Globals::kinect, &Kinect::onAngleChanged);
    Globals::kinectNearThresh.addListener(&Globals::kinect, &Kinect::onNearThreshChanged);
    Globals::kinectFarThresh.addListener(&Globals::kinect, &Kinect::onFarThreshChanged);
    Globals::kinectAdjustAngle.addListener(&Globals::kinect, &Kinect::onAngleAdjustChanged);
    Globals::contourMinArea.addListener(&Globals::kinect, &Kinect::onContourMinAreaChanged);
    Globals::contourMaxArea.addListener(&Globals::kinect, &Kinect::onContourMaxAreaChanged);
    Globals::contourThreshold.addListener(&Globals::kinect, &Kinect::onContourBriThreshChanged);
    Globals::contourFindHole.addListener(&Globals::kinect, &Kinect::onFindContourHoleChanged);
    
    Globals::leftKinectPosX.addListener(this, &GUI::onleftKinectPosChangedX);
    Globals::leftKinectPosY.addListener(this, &GUI::onleftKinectPosChangedY);
    Globals::leftKinectAng.addListener(this, &GUI::onleftKinectAngChanged);
    Globals::leftKinectScale.addListener(this, &GUI::onleftKinectScaleChanged);
    Globals::rightKinectPosX.addListener(this, &GUI::onrightKinectPosChangedX);
    Globals::rightKinectPosY.addListener(this, &GUI::onrightKinectPosChangedY);
    Globals::rightKinectAng.addListener(this, &GUI::onrightKinectAngChanged);
    Globals::rightKinectScale.addListener(this, &GUI::onrightKinectScaleChanged);
    
    kinectGUI.loadFromFile("settings/kinect.xml");
    
    kinectGUI.setSize(KINECT_W, kinectGUI.getHeight());
    kinectGUI.setWidthElements(KINECT_W);
    
    cniGui.setup(Globals::cniParams->params, "settings/CNIParams.xml");
    cniGui.loadFromFile("settings/CNIParams.xml");
    
    // proj warp
    ofVec2f tweakProjRes = ofVec2f(PROJ_W, PROJ_H) * WARP_TWEAK_SCALE;
    projWarpers.resize(NUM_PROJ);
    for (int i = 0; i < projWarpers.size(); i++)
    {
        ofPoint warpOrig = ofPoint(PROJ_W/2 - PROJ_W/2*WARP_TWEAK_SCALE + (PROJ_W - PROJ_W * WARP_TWEAK_SCALE)* i,
                                   PROJ_H/2 - PROJ_H/2*WARP_TWEAK_SCALE);
        projWarpers.at(i).setSourceRect(ofRectangle(0, 0, PROJ_W, PROJ_H));
        projWarpers.at(i).setTopLeftCornerPosition(ofPoint(warpOrig.x + tweakProjRes.x * i, warpOrig.y));
        projWarpers.at(i).setTopRightCornerPosition(ofPoint(warpOrig.x + tweakProjRes.x * i + tweakProjRes.x, warpOrig.y));
        projWarpers.at(i).setBottomLeftCornerPosition(ofPoint(warpOrig.x + tweakProjRes.x * i, warpOrig.y + tweakProjRes.y));
        projWarpers.at(i).setBottomRightCornerPosition(ofPoint(warpOrig.x + tweakProjRes.x * i + tweakProjRes.x, warpOrig.y + tweakProjRes.y));
        projWarpers.at(i).setup();
        projWarpers.at(i).load("settings/warps/projWarp" + ofToString(i) + ".xml");
        projWarpers.at(i).update();
        Globals::projMats.at(i) = projWarpers.at(i).getMatEx();
    }
    
    // kinect warp
    ofVec2f tweakKinectRes = ofVec2f(KINECT_W, KINECT_H) * WARP_TWEAK_SCALE;
    ofPoint warpOrig = ofPoint(APP_W/2 - KINECT_W/2*WARP_TWEAK_SCALE, PROJ_H/2 - KINECT_H/2*WARP_TWEAK_SCALE);
    kinectWarper.setSourceRect(ofRectangle(0, 0, KINECT_W, KINECT_H));
    kinectWarper.setTopLeftCornerPosition(ofPoint(warpOrig.x, warpOrig.y));
    kinectWarper.setTopRightCornerPosition(ofPoint(warpOrig.x + tweakKinectRes.x, warpOrig.y));
    kinectWarper.setBottomLeftCornerPosition(ofPoint(warpOrig.x, warpOrig.y + tweakKinectRes.y));
    kinectWarper.setBottomRightCornerPosition(ofPoint(warpOrig.x + tweakKinectRes.x, warpOrig.y + tweakKinectRes.y));
    kinectWarper.setup();
    kinectWarper.load("settings/warps/kinectWarp.xml");
    kinectWarper.update();
    Globals::kinectMat = kinectWarper.getMatEx();
    
    kinectFbo.allocate(KINECT_W, KINECT_H);
    kinectFbo.begin();
    ofClear(0);
    ofSetColor(ofColor::pink, 100);
    ofDrawRectangle(0, 0, kinectFbo.getWidth(), kinectFbo.getHeight());
    kinectFbo.end();
    
    kinectGUI.setPosition(ofPoint(10, APP_H/2));
    drawingCFSetting.setPosition(ofPoint(kinectGUI.getWidth() + 10, APP_H/2));
    cniGui.setPosition(ofPoint(kinectGUI.getWidth() + drawingCFSetting.getWidth() + 10, APP_H/2));
}

void GUI::update()
{
    if (!bHide)
    {
        int idx = 0;
        for (auto& warper : projWarpers)
        {
            warper.update();
            Globals::projMats.at(idx) = warper.getMatEx();
            idx++;
        }
        
        kinectWarper.update();
        Globals::kinectMat = kinectWarper.getMatEx();
        
        if (Globals::kinect.isInited())
        {
            contours.clear();
            contourPaths.clear();
            
            // get warped contour
            contours = Globals::kinect.getWarpedContour(Globals::kinectMat);
            
            kinectFbo.begin();
            ofClear(0);
            Globals::kinect.getDepthTexture().draw(0, 0);
            kinectFbo.end();
            
            for (auto& c : contours)
            {
                int idx = 0;
                ofPath path;
                for (auto& p : c.getVertices())
                {
                    if (idx == 0)
                    {
                        path.newSubPath();
                        path.moveTo(p);
                    }
                    else
                    {
                        path.lineTo(p);
                    }
                    idx++;
                }
                path.close();
                path.simplify();
                path.setFillColor(ofColor::yellow);
                contourPaths.push_back(path);
            }
        }
    }
}

void GUI::draw()
{
    if (!bHide)
    {
        // draw warped kinect depth & contour
        if (Globals::kinect.isInited())
        {
            //ofPushMatrix();
            //ofMultMatrix(Globals::kinectMat);
            //kinectFbo.draw(0, 0);
            //ofPopMatrix();
            
            ofPushStyle();
            ofFill();
            for (auto c : contourPaths)
                c.draw();
            ofPopStyle();
        }
        
        // proj warpers
        ofPushStyle();
        for (auto warper : projWarpers)
        {
            ofSetColor(ofColor::magenta);
            warper.drawQuadOutline();
            
            ofSetColor(ofColor::yellow);
            warper.drawCorners();
            
            ofSetColor(ofColor::magenta);
            warper.drawHighlightedCorner();
            
            ofSetColor(ofColor::red);
            warper.drawSelectedCorner();
        }
        
        // kinect warper
        ofSetColor(ofColor::magenta.getInverted());
        kinectWarper.drawQuadOutline();
        
        ofSetColor(ofColor::yellow.getInverted());
        kinectWarper.drawCorners();
        
        ofSetColor(ofColor::magenta.getInverted());
        kinectWarper.drawHighlightedCorner();
        
        ofSetColor(ofColor::red.getInverted());
        kinectWarper.drawSelectedCorner();
        
        ofPopStyle();
        
        // draw kinect tex for gui
        if (Globals::kinect.isInited())
        {
            int margin = 4;
            ofPoint p = kinectGUI.getPosition();
            p.y += kinectGUI.getHeight(); + margin;
            ofPushStyle();
            ofSetColor(ofColor::gray);
            ofDrawRectangle(p.x, p.y, kinectGUI.getWidth(), KINECT_H/2);
            ofPopStyle();
            Globals::kinect.getDepthTexture().draw(p.x + margin/2, p.y + margin/2, KINECT_W/2 - margin, KINECT_H/2 - margin);
            Globals::kinect.getThreshedTexture().draw(p.x + KINECT_W/2 + margin/2, p.y + margin/2, KINECT_W/2 - margin, KINECT_H/2 - margin);
            Globals::kinect.drawContour(p.x + KINECT_W/2 + margin/2, p.y + margin/2, KINECT_W/2 - margin, KINECT_H/2 - margin);
        }
        
        kinectGUI.draw();
        drawingCFSetting.draw();
        cniGui.draw();
    }
}

void GUI::saveProjWarp()
{
    for (int i = 0; i < projWarpers.size(); i++)
        projWarpers.at(i).save("settings/warps/projWarp" + ofToString(i) + ".xml");
}

void GUI::saveKinectWarp()
{
    kinectWarper.save("settings/warps/kinectWarp.xml");
}

void GUI::onleftKinectPosChangedX(float& arg)
{
    Globals::leftKinectTrans.pos.x = arg;
}

void GUI::onleftKinectPosChangedY(float& arg)
{
    Globals::leftKinectTrans.pos.y = arg;
}

void GUI::onleftKinectAngChanged(float& arg)
{
    Globals::leftKinectTrans.ang = arg;
}

void GUI::onleftKinectScaleChanged(float& arg)
{
    Globals::leftKinectTrans.scale = arg;
}

void GUI::onrightKinectPosChangedX(float& arg)
{
    Globals::rightKinectTrans.pos.x = arg;
}

void GUI::onrightKinectPosChangedY(float& arg)
{
    Globals::rightKinectTrans.pos.y = arg;
}

void GUI::onrightKinectAngChanged(float& arg)
{
    Globals::rightKinectTrans.ang = arg;
}

void GUI::onrightKinectScaleChanged(float& arg)
{
    Globals::rightKinectTrans.scale = arg;
}
