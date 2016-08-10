//
//  Globals.cpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#include "Globals.hpp"

namespace Globals
{
    float ELAPSED_TIME;
    
    vector<ofMatrix4x4> projMats;

	vector<ofFbo> morphSequence;
    
    // cni stuffs
    ofPtr<ofCamera> cam;
    ofPtr<CNIParams> cniParams;
    ofParameter<float> brightness;
    ofParameter<float> saturation;
    ofParameter<float> contrast;
    ofTexture testTex;
    
    // params    
    ofParameter<float> minArea, maxArea, threshold;
    ofParameter<bool> holes;
	ofParameter<float> micSensitivity;
    
    // events
    ofEvent<float> intensityChangedEvent;
    ofEvent<void> makeRandomMorphSoundEvent;
    ofEvent<void> makeRandomAttackSoundEvent;
    ofEvent<void> makeRandomPianoSoundEvent;
    ofEvent<void> makeRandomOkimochiSoundEvent;
}