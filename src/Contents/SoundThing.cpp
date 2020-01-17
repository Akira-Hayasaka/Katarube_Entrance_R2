//
//  SoundThing.cpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/3/16.
//
//

#include "SoundThing.hpp"

using namespace ofxCv;
using namespace cv;

void SoundThing::setup()
{
	int bufferSize = 256;
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(numHistory, 0.0);

	ofSoundStreamSettings settings;
	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);
	bFound = true;

    line.addVertices(getCircularPts(500, ofPoint(0, 0), numHistory));
    line.setClosed(true);
    origLine = line;
    
    kalman.init(1/100000., 1/10.);
    
    volIntensity = 0.0;
    rot = 0.0;
}

void SoundThing::update()
{
	audio_update();

    volIntensity = ofMap(smoothedVol, 0.0, maxVol, 0.3, 1.0, true);

//    ofVec2f filter(volAccumlated, volAccumlated);
//    kalman.update(filter);
//    volAccumlated = kalman.getEstimation().x;
    
    fillAlpha = ofMap(volIntensity, 0.3, 1.0, 20, 150, true);
    rot += ofMap(volIntensity, 0.3, 1.0, 0.05, 10.0, true);
    float intensity = ofMap(volIntensity, 0.3, 1.0, 0.0, 1.0, true);
    
    if (bFound)
        ofNotifyEvent(Globals::intensityChangedEvent, intensity);
    else
    {
        float none = 0.0;
        ofNotifyEvent(Globals::intensityChangedEvent, none);
    }
 
    scaledVol = ofMap(smoothedVol, 0.0, maxVol, 0.0, 1.0, true);
    volHistory.push_back(scaledVol);
    if (volHistory.size() >= numHistory)
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    
    for (int i = 0; i < line.getVertices().size(); i++)
    {
        ofVec2f dir = (ofVec2f(line.getVertices().at(i).x, line.getVertices().at(i).y) - ofVec2f::zero()).normalize();
        dir *= volHistory.at(i) * 300;
		ofVec2f res = ofVec2f(origLine.getVertices().at(i).x, origLine.getVertices().at(i).y) + dir;
        line.getVertices().at(i) = glm::vec3(res.x, res.y, 0.);
    }
}

void SoundThing::draw()
{
    int idx = 0;
    ofPath path;
    for (auto& p : line.getVertices())
    {
        if (idx == 0)
        {
            path.newSubPath();
            path.moveTo(p);
        }
        else
        {
            path.lineTo(p);
        }
        idx++;
    }
    path.close();
    path.setFillColor(ofColor(ofColor::white, fillAlpha));
    
    ofPushMatrix();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(ONESCRN_W/2, APP_H/2);
    ofRotateZ(rot);
    ofScale(volIntensity, volIntensity);
    ofPushStyle();
    path.draw();
    ofSetColor(ofColor::white, 180);
    ofDrawLine(ofVec2f::zero(), line.getVertices().front());
    ofSetColor(ofColor::white, 150);
    line.draw();
    ofPopStyle();
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
}

void SoundThing::audio_update()
{
	float curVol = 0.0;

	if (ofGetKeyPressed('v'))
		curVol = ofRandom(0.25);

	curVol -= 0.01;
	if (curVol < 0.0)
		curVol = 0.0;

	smoothedVol *= 0.93;

	smoothedVol += Globals::micSensitivity * curVol;
}

void SoundThing::audioIn(ofSoundBuffer& input)
{
	ofLog() << "audioIn";
    float curVol = 0.0;
    int numCounted = 0;
    
    for (int i = 0; i < input.getNumFrames(); i++)
    {
        left[i] = input[i*2] * 0.5;
        right[i] = input[i*2+1] * 0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted += 2;
    }
    
    curVol /= (float)numCounted;
    curVol = sqrt(curVol);
    
    smoothedVol *= 0.93;

    smoothedVol += Globals::micSensitivity * curVol;
}