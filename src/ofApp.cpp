#include "ofApp.h"

void ofApp::setup()
{
#ifdef WIN32
	setLogToFile("logs");
#endif

    ofEnableAlphaBlending();
    ofBackground(ofColor::black);
    ofSetVerticalSync(true);

    Globals::ELAPSED_TIME = ofGetElapsedTimef();
    //Globals::kinect.setup();
    Globals::projMats.resize(NUM_PROJ);
    Globals::cniParams = ofPtr<CNIParams>(new CNIParams);
    Globals::cam = ofPtr<ofCamera>(new ofCamera);
    Globals::cam->setupPerspective();
    for (int i = 0; i < 10; i++)
    {
        ofFbo f;
        Globals::morphSequence.push_back(f);
        Globals::morphSequence.back().allocate(ONESCRN_W * MORPH_SEQ_RATIO,
                                               ONESCRN_H * MORPH_SEQ_RATIO);
        Globals::morphSequence.back().begin();
        ofClear(0);
        Globals::morphSequence.back().end();
    }
    
    Tweenzor::init();
    
    gui.setup();
    gui.hide();
	ofHideCursor();
    
    se.setup();
    bpm.setBpm(50);
    bpm.start();
    
    lMgmt.setup(APP_W, APP_H);
    deforming = lMgmt.createLayer<Deforming>(1.0);
    nowExibit = lMgmt.createLayer<NowExibit>(1.0);
    texFlow = lMgmt.createLayer<TexFlow>(1.0);
    soundThing = lMgmt.createLayer<SoundThing>(1.0);
    
    numMaxRunningGlitch = 1;
    glitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    glitch.setFx(OFXPOSTGLITCH_GLOW, false);
    glitch.setFx(OFXPOSTGLITCH_SHAKER, false);
    glitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    glitch.setFx(OFXPOSTGLITCH_TWIST, false);
    glitch.setFx(OFXPOSTGLITCH_OUTLINE, false);
    glitch.setFx(OFXPOSTGLITCH_NOISE, false);
    glitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    glitch.setFx(OFXPOSTGLITCH_SWELL, false);
    glitch.setFx(OFXPOSTGLITCH_INVERT, false);
    glitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    glitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    glitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    glitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    glitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    glitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    glitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
    glitchCandidtates.push_back(OFXPOSTGLITCH_CONVERGENCE);
    glitchCandidtates.push_back(OFXPOSTGLITCH_SHAKER);
    glitchCandidtates.push_back(OFXPOSTGLITCH_CUTSLIDER);
    glitchCandidtates.push_back(OFXPOSTGLITCH_TWIST);
    glitchCandidtates.push_back(OFXPOSTGLITCH_OUTLINE);
    glitchCandidtates.push_back(OFXPOSTGLITCH_INVERT);
    glitchCandidtates.push_back(OFXPOSTGLITCH_SLITSCAN);
    glitchCandidtates.push_back(OFXPOSTGLITCH_SWELL);
//    glitchCandidtates.push_back(OFXPOSTGLITCH_CR_REDINVERT);
//    glitchCandidtates.push_back(OFXPOSTGLITCH_CR_BLUEINVERT);
//    glitchCandidtates.push_back(OFXPOSTGLITCH_CR_GREENINVERT);
    glitch.setFbo(&lMgmt.getFramebuffer());
    
    ofAddListener(Globals::intensityChangedEvent, this, &ofApp::onIntensityChanged);
    ofAddListener(bpm.beatEvent, this, &ofApp::onBeat);
    
    // debug
    bDrawTiny = false;
    bDrawBigRight = false;
    beatAlpha = 0.0;
}

void ofApp::update()
{
    Tweenzor::update(ofGetElapsedTimeMillis());
    Globals::ELAPSED_TIME = ofGetElapsedTimef();
    //Globals::kinect.update();
    
    gui.update();
    lMgmt.update();
    se.update();
    updateGlitch();
    glitch.generateFx();
    
    // debug
    beatAlpha -= 5;
    beatAlpha = ofClamp(beatAlpha, 0, 255);
}

void ofApp::draw()
{
    ofStyle s = ofGetStyle();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    if (bDrawTiny)
    {
        lMgmt.getFramebuffer().draw(0, 0, APP_W * 0.5, APP_H * 0.5);
    }
    else
    {
        ofPushMatrix();
        ofMultMatrix(Globals::projMats.at(0));
        if (bDrawBigRight)
            drawRight();
        else
            drawLeft();
        ofPopMatrix();
        
        ofPushMatrix();
        ofMultMatrix(Globals::projMats.at(1));
        drawRight();
        ofPopMatrix();
    }
    glDisable(GL_BLEND);
    ofSetStyle(s);
    
    gui.draw();
    
    // debug
    ofPushStyle();
    ofSetColor(ofColor::white, beatAlpha);
    ofDrawCircle(20, APP_H - 100, 10);
    ofPopStyle();
    ofDrawBitmapStringHighlight("tw:" + ofToString(Tweenzor::getSize()), 10, APP_H - 40);
    ofDrawBitmapStringHighlight("fps:" + ofToString(ofGetFrameRate(), 2), 10, APP_H - 20);
}

void ofApp::keyPressed(int key)
{
    if (key == ' ')
    {
        gui.toggleVisible();

		if (gui.isVisible())
			ofShowCursor();
		else
			ofHideCursor();
    }
    if (key == 't')
    {
        bDrawTiny = !bDrawTiny;
    }
    if (key == OF_KEY_RIGHT)
    {
        if (!bDrawTiny)
            bDrawBigRight = !bDrawBigRight;
    }
	if (key == 'f')
	{
		ofToggleFullscreen();
	}
    if (key == 'w')
    {
        gui.saveProjWarp();
    }
}

void ofApp::drawLeft()
{
    lMgmt.getFramebuffer().getTexture().drawSubsection(0, 0, ONESCRN_W, ONESCRN_H, 0, 0);
}

void ofApp::drawRight()
{
    lMgmt.getFramebuffer().getTexture().drawSubsection(0, 0, ONESCRN_W, ONESCRN_H, ONESCRN_W, 0);
}

void ofApp::updateGlitch()
{
    float rdm = ofRandomuf();
    
    if (rdm < glitchIntensity)
    {
        // push
        if (curGlitch.size() <= numMaxRunningGlitch)
        {
            bool bContain = true;
            ofxPostGlitchType tgt = glitchCandidtates.at(ofRandom(glitchCandidtates.size()));
            while (bContain)
            {
                bContain = ofContains(curGlitch, tgt);
                if (!bContain)
                {
                    curGlitch.push_back(tgt);
                    glitch.setFx(curGlitch.back(), true);
                    lastPushTime = Globals::ELAPSED_TIME;
                }
                else
                {
                    tgt = glitchCandidtates.at(ofRandom(glitchCandidtates.size()));
                }
            }
        }
    }
    
    if (Globals::ELAPSED_TIME - lastPushTime > ofRandom(0.1, 0.3))
    {
        // pop
        if (!curGlitch.empty())
        {
            glitch.setFx(curGlitch.front(), false);
            curGlitch.erase(curGlitch.begin());
        }
    }
}

void ofApp::mouseMoved(int x, int y)
{
    float intensity = ofMap(ofGetMouseX(), 0, 1920, 0, 1.0, true);
    ofNotifyEvent(Globals::intensityChangedEvent, intensity);
}

void ofApp::onIntensityChanged(float& intensity)
{
    if (intensity > 0.75)
    {
        glitchIntensity = ofMap(intensity, 0.5, 1.0, 0.0, 1.0);
    }
    else
    {
        glitchIntensity = 0.0;
    }
}

void ofApp::onBeat()
{
    beatAlpha = 255;
    float rdm = ofRandomuf();
    if (rdm < 0.3)
        ofNotifyEvent(Globals::makeRandomPianoSoundEvent);
    
    rdm = ofRandomuf();
    if (rdm < 0.5)
        ofNotifyEvent(Globals::makeRandomOkimochiSoundEvent);
}

void ofApp::keyReleased(int key){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}