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
    
private:
    
    bool bHide;
    
    // drawing contourFinder settings
    ofxPanel drawingCFSetting;
    
    // cni gui
    ofxPanel cniGui;

	// misc gui
	ofxPanel miscGui;
    
    // proj warp
    vector<Warper> projWarpers;
};

#endif /* GUI_hpp */
