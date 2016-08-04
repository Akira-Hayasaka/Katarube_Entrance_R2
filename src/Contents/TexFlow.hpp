//
//  TexFlow.hpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/3/16.
//
//

#ifndef TexFlow_hpp
#define TexFlow_hpp

#include "ofMain.h"
#include "ofxLayer.h"

#include "Constants.h"
#include "Globals.hpp"

#include "CurlNoiseInstanced.h"

using namespace ofxLayer;

class TexFlow : public ofxLayer::Layer
{
public:
    
    OFX_LAYER_DEFINE_LAYER_CLASS(TexFlow);
    
    void setup();
    void update();
    void draw();
    
private:
    
    ofPtr<CurlNoiseInstanced> cni;
    
};

#endif /* TexFlow_hpp */
