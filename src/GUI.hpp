//
//  GUI.hpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#ifndef GUI_hpp
#define GUI_hpp

#include "ofMain.h"
#include "ofxGui.h"
#include "Constants.h"
#include "Globals.hpp"
#include "Warper.hpp"

class GUI
{
public:
    
    void setup();
    void update();
    void draw();
    
    void toggleVisible() { bHide = !bHide; }
    void hide() { bHide = true; }
    void show() { bHide = false; }
    bool isVisible() { return !bHide; }
    
    void saveProjWarp();
    void saveKinectWarp();
    
private:
    
    void onleftKinectPosChangedX(float& arg);
    void onleftKinectPosChangedY(float& arg);
    void onleftKinectAngChanged(float& arg);
    void onleftKinectScaleChanged(float& arg);
    void onrightKinectPosChangedX(float& arg);
    void onrightKinectPosChangedY(float& arg);
    void onrightKinectAngChanged(float& arg);
    void onrightKinectScaleChanged(float& arg);
    
    bool bHide;
    
    // drawing contourFinder settings
    ofxPanel drawingCFSetting;
    
    // kinect device & contour
    ofxPanel kinectGUI;
    
    // cni gui
    ofxPanel cniGui;
    
    // proj warp
    vector<Warper> projWarpers;
    
    // kinect warp
    Warper kinectWarper;
    ofFbo kinectFbo;
    vector<ofPolyline> contours;
    vector<ofPath> contourPaths;
};

#endif /* GUI_hpp */
