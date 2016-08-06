//
//  SoundThing.hpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/3/16.
//
//

#ifndef SoundThing_hpp
#define SoundThing_hpp

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxLayer.h"

#include "Constants.h"
#include "Globals.hpp"
#include "Utils.h"

using namespace ofxLayer;

class SoundThing : public ofxLayer::Layer, public ofBaseSoundInput
{
public:
    
    OFX_LAYER_DEFINE_LAYER_CLASS(SoundThing);
    
    void setup();
    void update();
    void draw();
    
    void audioIn(float * input, int bufferSize, int nChannels);
    
private:
    
    ofSoundStream soundStream;
    vector<float> left;
    vector<float> right;
    vector<float> volHistory;
    float smoothedVol;
    float scaledVol;
    
    ofPolyline line;
    ofPolyline origLine;
    float volIntensity;
    const int numHistory = 2500;
    const float volLeakAmt = 0.035;
    const float volInputScale = 0.1;
    
    ofxCv::KalmanPosition kalman;
    float fillAlpha;
    double rot;
    
    const float maxVol = 0.17;
};

#endif /* SoundThing_hpp */
