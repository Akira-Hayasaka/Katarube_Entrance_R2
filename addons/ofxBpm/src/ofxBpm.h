//
//  ofxBpm.h
//  ofxBpm
//
//  Created by mirrorboy on 2013/08/14.
//
//

/*
You must add listener on testApp and listen beat timing.
 
example on testApp.cpp
 
 
ofAddListener(bpm.beatEvent, this, &testApp::play);

void testApp::play(void){
 
  //sound.play();
  //but, you could not draw somethings.
}

 */

#pragma once
#include "ofMain.h"
#include "ofThread.h"

static const float OFX_BPM_MAX = 300.;
static const float  OFX_BPM_DEFAULT = 120.;
static const float OFX_BPM_MIN = 1.;
static const int OFX_BPM_TICK = 960;

class ofxBpm : private ofThread{
    
public:

    explicit ofxBpm(float bpm = OFX_BPM_DEFAULT,int beatPerBar = 4);
    
    void start();
    void stop();
    void reset();
    
    void setBpm(float bpm);
    float getBpm() const;
    
    void setBeatPerBar(int beatPerBar);
    
    bool isPlaying() const;

    ofEvent<void> beatEvent;

private:
    void threadedFunction();
        
    
    
    float _bpm;
    bool  _isPlaying;
    int _countOfTick;
    unsigned long long _totalTime;
    unsigned long long _durationOfTick;
    unsigned long long _remainderOfTick;
    
    unsigned long long _preTime;
    int _beatPerBar;
    
    inline int getCountOfTick() const;
};

////init
//const float ofxBpm::OFX_BPM_MAX = 300. ;
//const float ofxBpm::OFX_BPM_DEFAULT = 120.;
//const float ofxBpm::OFX_BPM_MIN = 1.;
//const int ofxBpm::OFX_BPM_TICK = 960;
