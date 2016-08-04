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
    
    Kinect kinect;
    vector<ofMatrix4x4> projMats;
    ofMatrix4x4 kinectMat;
    KinectContourTransform leftKinectTrans;
    KinectContourTransform rightKinectTrans;
    
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
    
    ofParameter<int> kinectAngle;
    ofParameter<float> kinectNearThresh;
    ofParameter<float> kinectFarThresh;
    ofParameter<float> kinectAdjustAngle;
    ofParameter<float> contourMinArea;
    ofParameter<float> contourMaxArea;
    ofParameter<float> contourThreshold;
    ofParameter<bool> contourFindHole;
    ofParameter<float> leftKinectPosX;
    ofParameter<float> leftKinectPosY;
    ofParameter<float> leftKinectAng;
    ofParameter<float> leftKinectScale;
    ofParameter<float> rightKinectPosX;
    ofParameter<float> rightKinectPosY;
    ofParameter<float> rightKinectAng;
    ofParameter<float> rightKinectScale;
    
    // events
    ofEvent<float> intensityChangedEvent;
}