#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "ofxTweenzor.h"
#include "ofxLayer.h"
#include "ofxPostGlitch.h"
#include "ofxBpm.h"

#include "Utils.h"
#include "Constants.h"
#include "Globals.hpp"
#include "SE.hpp"

#include "GUI.hpp"
#include "Deforming.hpp"
#include "NowExibit.hpp"
#include "SoundThing.hpp"
#include "TexFlow.hpp"

class ofApp : public ofBaseApp
{
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    
    void drawLeft();
    void drawRight();
    void updateGlitch();
    void onIntensityChanged(float& intensity);
    void onBeat();
    
    GUI gui;
    
    SE se;
    ofxBpm bpm;    

    ofxLayer::Manager lMgmt;
    Deforming* deforming;
    NowExibit* nowExibit;
    SoundThing* soundThing;
    TexFlow* texFlow;
    
    // glitch
    ofxPostGlitch glitch;
    int numMaxRunningGlitch;
    vector<ofxPostGlitchType> glitchCandidtates;
    vector<ofxPostGlitchType> curGlitch;
    float lastPushTime;
    float glitchIntensity;
    
    // debug
    bool bDrawTiny;
    bool bDrawBigRight;
    float beatAlpha;
};
