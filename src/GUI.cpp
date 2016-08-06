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
    drawingCFSetting.setup("drawing setting");
    drawingCFSetting.add(Globals::minArea.set("Min area", 10, 1, 100));
    drawingCFSetting.add(Globals::maxArea.set("Max area", 5000, 1, 5500));
    drawingCFSetting.add(Globals::threshold.set("Threshold", 128, 0, 255));
    drawingCFSetting.add(Globals::holes.set("Holes", false));
    
    cniGui.setup(Globals::cniParams->params, "settings/CNIParams.xml");
    cniGui.loadFromFile("settings/CNIParams.xml");

	miscGui.setup("misc", "settings/misc.xml");
	miscGui.add(Globals::micSensitivity.set("mic sensitivity", 0.07, 0.01, 1.0));
	miscGui.loadFromFile("settings/misc.xml");
    
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
    
    drawingCFSetting.setPosition(ofPoint(10, 10));
    cniGui.setPosition(ofPoint(drawingCFSetting.getWidth() + 20, 10));
	miscGui.setPosition(ofPoint(drawingCFSetting.getWidth() + cniGui.getWidth() + 30, 10));
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
    }
}

void GUI::draw()
{
    if (!bHide)
    {
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
        ofPopStyle();
        
        drawingCFSetting.draw();
        cniGui.draw();
		miscGui.draw();
    }
}

void GUI::saveProjWarp()
{
    for (int i = 0; i < projWarpers.size(); i++)
        projWarpers.at(i).save("settings/warps/projWarp" + ofToString(i) + ".xml");
}
