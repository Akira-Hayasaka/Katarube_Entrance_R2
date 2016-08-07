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
    ofDirectory dir("sounds");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        Sound snd;
        snd.load(dir.getFile(i).getAbsolutePath());
        snd.setMultiPlay(true);
        snd.setLoop(false);
        snds.push_back(snd);
    }
    dir.close();
    
    ofAddListener(Globals::makeRandomSoundEvent, this, &SE::makeRdmSound);
}

void SE::makeSound(string sndName)
{
    for (auto snd : snds)
    {
        if (ofIsStringInString(snd.getFileName(), sndName))
        {
            snd.play();
            break;
        }
    }
}

void SE::makeRdmSound()
{
    int rdm = ofRandom(snds.size());
    snds.at(rdm).setSpeed(ofRandom(0.2, 0.8));
    snds.at(rdm).play();
}