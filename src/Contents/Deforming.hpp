//
//  Deforming.hpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#ifndef Deforming_hpp
#define Deforming_hpp

#include "ofMain.h"
#include "ofxLayer.h"

#include "Utils.h"
#include "Globals.hpp"
#include "Deformer.hpp"

using namespace ofxLayer;

class Deforming : public ofxLayer::Layer
{
public:
    
    OFX_LAYER_DEFINE_LAYER_CLASS(Deforming);
    
    void setup();
    void update();
    void draw();
    
private:
    
    int getRdmIdx();
    void onFinishEvent();
    
    ofDirectory masterDir;
    deque<int> imageHistory;
    
    ofPtr<Deformer> deformTo;
    ofPtr<Deformer> restoreFrom;
    vector<ofPtr<Deformer> > storedPuppets;
    
    ofVec3f rot;    
};

#endif /* Deforming_hpp */
