//
//  NowExibit.hpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#ifndef NowExibit_hpp
#define NowExibit_hpp

#include "ofMain.h"
#include "ofxLayer.h"

#include "Constants.h"

using namespace ofxLayer;

class NowExibit : public ofxLayer::Layer
{
public:
    
    OFX_LAYER_DEFINE_LAYER_CLASS(NowExibit);
    
    void setup();
    void update();
    void draw();
    
private:

    ofVec3f rot;
    ofTexture exibitionTex;
};

#endif /* NowExibit_hpp */
