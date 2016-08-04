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
#include "Kinect.hpp"

struct KinectContourTransform
{
    ofVec2f pos;
    float ang;
    float scale;
};

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
    
    extern Kinect kinect;
    extern vector<ofMatrix4x4> projMats;
    extern ofMatrix4x4 kinectMat;
    extern KinectContourTransform leftKinectTrans;
    extern KinectContourTransform rightKinectTrans;
    
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
    
    extern ofParameter<int> kinectAngle;
    extern ofParameter<float> kinectNearThresh;
    extern ofParameter<float> kinectFarThresh;
    extern ofParameter<float> kinectAdjustAngle;
    extern ofParameter<float> contourMinArea;
    extern ofParameter<float> contourMaxArea;
    extern ofParameter<float> contourThreshold;
    extern ofParameter<bool> contourFindHole;
    extern ofParameter<float> leftKinectPosX;
    extern ofParameter<float> leftKinectPosY;
    extern ofParameter<float> leftKinectAng;
    extern ofParameter<float> leftKinectScale;
    extern ofParameter<float> rightKinectPosX;
    extern ofParameter<float> rightKinectPosY;
    extern ofParameter<float> rightKinectAng;
    extern ofParameter<float> rightKinectScale;
    
    // events
    extern ofEvent<float> intensityChangedEvent;
}

#endif /* Globals_hpp */
