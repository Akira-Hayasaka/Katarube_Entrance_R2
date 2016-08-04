//
//  CurlNoiseInstanced.h
//  emptyExample
//
//  Created by Akira on 8/7/15.
//
//

#ifndef __emptyExample__CurlNoiseInstanced__
#define __emptyExample__CurlNoiseInstanced__

#include "ofMain.h"
#include "Utils.h"
#include "FboPingPong.h"
#include "CNInstance.h"

class CurlNoiseInstanced
{
public:
    
    CurlNoiseInstanced();
    void init();
    
    void setup(bool bTween = false, int texSize = 32);
    void update(float spawnRadius = 0.1);
    void draw(ofCamera* _camera, float scaleFactor = 1.0);
    void drawNoiseTex() { noiseData.source()->getTexture(0).draw(0, 0, 196, 196); }
    
    ofPtr<CNIParams> getParams() { return Globals::cniParams; }
    
private:
    
    void setupStaticInstanceParams();
    void updateDynamicInstanceParams();
    void beginDraw(float scaleFactor);
    void endDraw();
    
    void onIntensityChanged(float& intensity);
    
    float time;
    float timeStep;
    
    ofShader noiseUpdate;
    ofShader renderInstance;
    
    int numParticles;
    int textureSize;
    
    FboPingPong noiseData;
    
    ofVboMesh quad;
    
    vector<CNInstance> instances;
    ofFbo dynamicInstanceParams;
    ofFbo staticInstanceParams;
    
    ofPoint offset;
    
    int emergeThresh;
};

#endif /* defined(__emptyExample__CurlNoiseInstanced__) */
