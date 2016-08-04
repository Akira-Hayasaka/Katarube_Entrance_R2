//
//  CurlNoiseInstanced.cpp
//  emptyExample
//
//  Created by Akira on 8/7/15.
//
//

#include "CurlNoiseInstanced.h"

CurlNoiseInstanced::CurlNoiseInstanced()
{
    init();    
}

void CurlNoiseInstanced::init()
{
    noiseUpdate.load("shaders/curlNoiseInstanced/noiseUpdate");
    renderInstance.load("shaders/curlNoiseInstanced/renderInstance");
}

void CurlNoiseInstanced::setup(bool bTween, int texSize)
{
    textureSize = texSize;
    numParticles = textureSize * textureSize;
    
    time = 0.0f;
    timeStep = 1.0f / 60.0f;
    
    ofFbo::Settings fboSettings;
    fboSettings.width = textureSize;
    fboSettings.height = textureSize;
    fboSettings.numColorbuffers = 2;
    fboSettings.useDepth = false;
    fboSettings.internalformat = GL_RGBA32F;
    fboSettings.textureTarget = GL_TEXTURE_2D;
    fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
    fboSettings.minFilter = GL_NEAREST;
    fboSettings.maxFilter = GL_NEAREST;
    
    ofDisableTextureEdgeHack();
    noiseData.allocate(fboSettings);
    dynamicInstanceParams.allocate(fboSettings);
    staticInstanceParams.allocate(fboSettings);
    ofEnableTextureEdgeHack();
    
    ofVec4f* startPositionsAndAge = new ofVec4f[numParticles];
    ofVec4f* dynamicInstanceParam = new ofVec4f[numParticles];
    ofVec4f* staticInstanceParam = new ofVec4f[numParticles];
    int tmpIndex = 0;
    for (int y = 0; y < textureSize; y++)
    {
        for (int x = 0; x < textureSize; x++)
        {
            ofVec3f pos(0,0,0);
            float startAge = ofRandom(Globals::cniParams->particleMaxAge);
            startPositionsAndAge[tmpIndex] = ofVec4f(pos.x, pos.y, pos.z, startAge);
            
            ofFloatColor col = ofFloatColor(ofRandom(0.7, 1.0), 0.0, 0.0);

            float scale = 0.0;
            dynamicInstanceParam[tmpIndex] = ofVec4f(col.r, col.g, col.b, scale);
            
            CNInstance instance;
            instance.setup(tmpIndex);
            instance.setColor(col);
            instances.push_back(instance);
            
            staticInstanceParam[tmpIndex] = ofVec4f(0.0, 0.0, 0.0, 0.0);
            
            tmpIndex++;
        }
    }
    noiseData.source()->getTexture(0).loadData((float*)&startPositionsAndAge[0].x, textureSize, textureSize, GL_RGBA);
    dynamicInstanceParams.getTexture().loadData((float*)&dynamicInstanceParam[0].x, textureSize, textureSize, GL_RGBA);
    staticInstanceParams.getTexture().loadData((float*)&staticInstanceParam[0].x, textureSize, textureSize, GL_RGBA);
    
    float mult = 50;
    ofPlanePrimitive plane;
    plane.set(0.0015 * mult, 0.0015 * mult, 10, 10);
    plane.rotate(90, 1, 0, 0);
    quad = plane.getMesh();
    quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    ofAddListener(Globals::intensityChangedEvent, this, &CurlNoiseInstanced::onIntensityChanged);
}

void CurlNoiseInstanced::update(float spawnRadius)
{
    if (ofGetKeyPressed(' '))
        timeStep = ofLerp(timeStep, ofMap(ofGetMouseX(), 0, ofGetWidth(), -(1.0f/60.0f), (1.0f/60.0f)), 0.1f);
    else
        timeStep = ofLerp(timeStep, 1.0f / 60.0f, 0.1f);
    time += timeStep;
    
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    
    noiseData.dest()->begin();
    noiseData.dest()->activateAllDrawBuffers();
    noiseUpdate.begin();
    noiseUpdate.setUniformTexture("particlePosAndAgeTexture", noiseData.source()->getTexture(0), 0);
    noiseUpdate.setUniformTexture("particleVelTexture", noiseData.source()->getTexture(1), 1);
    noiseUpdate.setUniform1f("time", time);
    noiseUpdate.setUniform1f("timeStep", timeStep);
    noiseUpdate.setUniform1f("particleMaxAge", Globals::cniParams->particleMaxAge);
    noiseUpdate.setUniform1f("noisePositionScale", Globals::cniParams->noisePositionScale);
    noiseUpdate.setUniform1f("noiseTimeScale", Globals::cniParams->noiseTimeScale);
    noiseUpdate.setUniform1f("noisePersistence", Globals::cniParams->noisePersistence);
    noiseUpdate.setUniform1f("noiseMagnitude", Globals::cniParams->noiseMagnitude);
    noiseUpdate.setUniform3f("baseSpeed",
                             Globals::cniParams->baseSpeed.get().x,
                             Globals::cniParams->baseSpeed.get().y,
                             Globals::cniParams->baseSpeed.get().z);
    noiseUpdate.setUniform1f("spawnRadius", spawnRadius);
    noiseUpdate.setUniform1f("bSpawn", (ofGetKeyPressed('s')) ? 1.0 : 0.0);
    noiseData.source()->draw(0,0);
    noiseUpdate.end();
    noiseData.dest()->end();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    noiseData.swap();
    
    updateDynamicInstanceParams();
}

void CurlNoiseInstanced::draw(ofCamera* _camera, float scaleFactor)
{
    beginDraw(scaleFactor);
    quad.drawInstanced(OF_MESH_FILL, numParticles);
    endDraw();
}

void CurlNoiseInstanced::beginDraw(float scaleFactor)
{
    renderInstance.begin();
    renderInstance.setUniformTexture("particlePosAndAgeTexture", noiseData.source()->getTexture(0), 1);
    renderInstance.setUniformTexture("particleVelTexture", noiseData.source()->getTexture(1), 2);
    renderInstance.setUniformTexture("dynamicInstanceParamTexture", dynamicInstanceParams.getTexture(), 3);
    renderInstance.setUniformTexture("staticInstanceParamTexture", staticInstanceParams.getTexture(), 4);
    renderInstance.setUniformTexture("drawing", Globals::testTex, 5);
    renderInstance.setUniform2f("resolution", noiseData.source()->getWidth(), noiseData.source()->getHeight());
    renderInstance.setUniform1f("particleMaxAge", Globals::cniParams->particleMaxAge);
    renderInstance.setUniform1f("scale", Globals::cniParams->scale);
    renderInstance.setUniform3f("offset",
                                offset.x,
                                offset.y,
                                offset.z);
    renderInstance.setUniform1f("scaleFactor", scaleFactor);
}

void CurlNoiseInstanced::endDraw()
{
    renderInstance.end();
}

void CurlNoiseInstanced::updateDynamicInstanceParams()
{
    ofVec4f* dynamicInstanceParam = new ofVec4f[numParticles];
    ofVec4f* staticInstanceParam = new ofVec4f[numParticles];
    int tmpIndex = 0;
    for (int y = 0; y < textureSize; y++)
    {
        for (int x = 0; x < textureSize; x++)
        {
            if (tmpIndex < emergeThresh)
                instances.at(tmpIndex).shrink();
            else
                instances.at(tmpIndex).growup();
            
            instances.at(tmpIndex).update();
            float scale = instances.at(tmpIndex).getScale();//1.0;//(bTween) ? 1.0 : instances.at(tmpIndex).getScale();
            ofFloatColor col = instances.at(tmpIndex).getColor();
            dynamicInstanceParam[tmpIndex] = ofVec4f(col.r, col.g, col.b, scale);
            
            staticInstanceParam[tmpIndex] = ofVec4f(1.0, 0.0, 0.0, 0.0);
            
            tmpIndex++;
        }
    }
    dynamicInstanceParams.getTexture().loadData((float*)&dynamicInstanceParam[0].x, textureSize, textureSize, GL_RGBA);
    staticInstanceParams.getTexture().loadData((float*)&staticInstanceParam[0].x, textureSize, textureSize, GL_RGBA);
    
    delete dynamicInstanceParam;
    delete staticInstanceParam;
}

void CurlNoiseInstanced::onIntensityChanged(float& intensity)
{
    emergeThresh = shapedMap(intensity, 0.0, 0.75, numParticles, 0, 5.0);
}