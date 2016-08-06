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
    soundStream.printDeviceList();
    int bufferSize = 256;
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(numHistory, 0.0);
    soundStream.setInput(this);
    soundStream.setup(0, 1, 44100, bufferSize, 4);
    
    line.addVertices(getCircularPts(500, ofPoint(0, 0), numHistory));
    line.setClosed(true);
    origLine = line;
    
    kalman.init(1/100000., 1/10.);
    
    rot = 0.0;
}

void SoundThing::update()
{
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    volHistory.push_back(scaledVol);
    if (volHistory.size() >= numHistory)
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    
    volAccumlated += scaledVol * volInputScale;
    volAccumlated -= volLeakAmt;
    volAccumlated = ofClamp(volAccumlated, 0.3, 1.0);

//    ofVec2f filter(volAccumlated, volAccumlated);
//    kalman.update(filter);
//    volAccumlated = kalman.getEstimation().x;
    
    fillAlpha = ofMap(volAccumlated, 0.3, 1.0, 20, 150, true);
    rot += ofMap(volAccumlated, 0.3, 1.0, 0.05, 5.0, true);
    float intensity = ofMap(volAccumlated, 0.3, 1.0, 0.0, 1.0, true);
    ofNotifyEvent(Globals::intensityChangedEvent, intensity);
    
    for (int i = 0; i < line.getVertices().size(); i++)
    {
        ofVec2f dir = (line.getVertices().at(i) - ofVec2f::zero()).normalize();
        dir *= volHistory.at(i) * 300;
        line.getVertices().at(i) = origLine.getVertices().at(i) + dir;
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
    ofScale(volAccumlated, volAccumlated);
    ofPushStyle();
    path.draw();
    ofSetColor(ofColor::white, 180);
    ofDrawLine(ofVec2f::zero(), line.getVertices().front());
    ofSetColor(ofColor::white);
    line.draw();
    ofPopStyle();
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
}

void SoundThing::audioIn(float * input, int bufferSize, int nChannels)
{
    float curVol = 0.0;
    int numCounted = 0;
    
    for (int i = 0; i < bufferSize; i++)
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
    smoothedVol += 0.07 * curVol;
}