//
//  SE.cpp
//  Entrance_r2
//
//  Created by Akira Hayasaka on 8/7/16.
//
//

#include "SE.hpp"

bool Sound::loadSound(string fileName, bool stream)
{
    this->fileName = fileName;
    return ofSoundPlayer::load(fileName, stream);
}

void SE::setup()
{
    drone.load("sounds/pad/katarube_pad.wav");
    drone.setMultiPlay(false);
    drone.setLoop(OF_LOOP_NORMAL);
    drone.play();
    
    okimochi.load("sounds/okimochi/katarube_okimochi_all_tweak.wav", true);
    okimochi.setMultiPlay(false);
    okimochi.setLoop(OF_LOOP_NORMAL);
    okimochi.play();
    okimochi.setPosition(ofRandomuf());
    okimochiVol = 0.0;
    
    ofDirectory dir("sounds/morph");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        Sound snd;
        snd.load(dir.getFile(i).getAbsolutePath());
        snd.setMultiPlay(true);
        snd.setLoop(false);
        morph.push_back(snd);
    }
    dir.close();
    
    dir.open("sounds/attack");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        Sound snd;
        snd.load(dir.getFile(i).getAbsolutePath());
        snd.setMultiPlay(true);
        snd.setLoop(false);
        attack.push_back(snd);
    }
    dir.close();
    
    dir.open("sounds/piano");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        Sound snd;
        snd.load(dir.getFile(i).getAbsolutePath());
        snd.setMultiPlay(true);
        snd.setLoop(false);
        piano.push_back(snd);
    }
    dir.close();
    
    dir.open("sounds/okimochi_sigh");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        Sound snd;
        snd.load(dir.getFile(i).getAbsolutePath());
        snd.setMultiPlay(true);
        snd.setLoop(false);
        sigh.push_back(snd);
    }
    dir.close();
    
    ofAddListener(Globals::makeRandomMorphSoundEvent, this, &SE::makeRdmMorphSound);
    ofAddListener(Globals::makeRandomAttackSoundEvent, this, &SE::makeRdmAttackSound);
    ofAddListener(Globals::makeRandomPianoSoundEvent, this, &SE::makeRdmPianoSound);
    ofAddListener(Globals::makeRandomOkimochiSoundEvent, this, &SE::makeRdmOkimochiSound);
}

void SE::update()
{
    float freq = 0.25;
    float mag = 1.0;
    float vol = ofMap(ofNoise(Globals::ELAPSED_TIME * freq, PI) * mag, 0.0, 1.0, 0.0, 0.3, true);
    okimochi.setVolume(vol);
    if (!okimochi.isPlaying())
    {
        okimochi.play();
        okimochi.setPosition(ofRandomuf());
    }
}

void SE::makeRdmMorphSound()
{
    int rdm = ofRandom(morph.size());
    morph.at(rdm).play();
}

void SE::makeRdmAttackSound()
{
    int rdm = ofRandom(attack.size());
    attack.at(rdm).play();
}

void SE::makeRdmPianoSound()
{
    int rdm = ofRandom(piano.size());
    piano.at(rdm).play();
}

void SE::makeRdmOkimochiSound()
{
    int rdm = ofRandom(sigh.size());    
    sigh.at(rdm).play();
}