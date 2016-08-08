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
    
    drone.play();
    
    ofAddListener(Globals::makeRandomMorphSoundEvent, this, &SE::makeRdmMorphSound);
    ofAddListener(Globals::makeRandomAttackSoundEvent, this, &SE::makeRdmAttackSound);
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