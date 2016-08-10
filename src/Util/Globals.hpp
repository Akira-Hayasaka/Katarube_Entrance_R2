//
//  Globals.hpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#ifndef Globals_hpp
#define Globals_hpp

#include "ofMain.h"

struct CNIParams
{
    CNIParams()
    {
        reset();
    }
    
    void reset()
    {
        params.setName("CNI params");
        params.add(particleMaxAge.set("Particle Max Age", 10.0f, 0.0f, 20.0f));
        params.add(noiseMagnitude.set("Noise Magnitude", 0.075, 0.01f, 1.0f));
        params.add(noisePositionScale.set("Noise Position Scale", 1.5f, 0.01f, 5.0f));
        params.add(noiseTimeScale.set("Noise Time Scale", 1.0 / 4000.0, 0.001f, 1.0f));
        params.add(noisePersistence.set("Noise Persistence", 0.2, 0.001f, 1.0f));
        params.add(scale.set("Scale", 1000.0, 0.0, 2000.0));
        params.add(offset.set("Offset", ofVec3f(0.0, 0.0, 0.0), ofVec3f(-1000, -500, -1000), ofVec3f(1000, 500, 1000)));
        params.add(baseSpeed.set("Wind", ofVec3f(0.5,0,0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)));
    }
    
    ofParameterGroup params;
    ofParameter<float> particleMaxAge;
    ofParameter<float> noisePositionScale;
    ofParameter<float> noiseMagnitude;
    ofParameter<float> noiseTimeScale;
    ofParameter<float> noisePersistence;
    ofParameter<float> scale;
    ofParameter<ofVec3f> offset;
    ofParameter<ofVec3f> baseSpeed;
};

namespace Globals
{
    extern float ELAPSED_TIME;
    
    extern vector<ofMatrix4x4> projMats;

	extern vector<ofFbo> morphSequence;
    
    // cni stuffs
    extern ofPtr<ofCamera> cam;
    extern ofPtr<CNIParams> cniParams;
    extern ofParameter<float> brightness;
    extern ofParameter<float> saturation;
    extern ofParameter<float> contrast;
    extern ofTexture testTex;
    
    // params
    extern ofParameter<float> minArea, maxArea, threshold;
    extern ofParameter<bool> holes;
	extern ofParameter<float> micSensitivity;
    
    // events
    extern ofEvent<float> intensityChangedEvent;
    extern ofEvent<void> makeRandomMorphSoundEvent;
    extern ofEvent<void> makeRandomAttackSoundEvent;
    extern ofEvent<void> makeRandomPianoSoundEvent;
    extern ofEvent<void> makeRandomOkimochiSoundEvent;
}

#endif /* Globals_hpp */
