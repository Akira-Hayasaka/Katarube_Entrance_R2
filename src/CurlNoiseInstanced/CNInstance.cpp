//
//  CNInstance.cpp
//  emptyExample
//
//  Created by Akira on 8/9/15.
//
//

#include "CNInstance.h"

void CNInstance::setup(int instanceID)
{
    this->instanceID = instanceID;
    scale = 0.0;
    destScale = ofRandom(0.5, 1.0);
    runState = NONE;
}

void CNInstance::update()
{
    if (runState == GROWUP)
    {
        float dur = 0.1f;
        float t = Globals::ELAPSED_TIME - growupTime;
        float f = ofMap(t, 0.0, dur, 0.0, 1.0, true);
        scale = ofLerp(0.0, destScale, f);
        if (f >= 1.0)
            runState = NONE;
    }
    else if (runState == SHRINK)
    {
        float dur = 0.4f;
        float t = Globals::ELAPSED_TIME - shrinkTime;
        float f = ofMap(t, 0.0, dur, 0.0, 1.0, true);
        scale = ofLerp(destScale, 0.0, f);
        if (f >= 1.0)
            runState = NONE;
    }
}

void CNInstance::growup()
{
    if (scale != destScale && runState != GROWUP)
    {
        growupTime = Globals::ELAPSED_TIME;
        runState = GROWUP;
    }
}

void CNInstance::shrink()
{
    if (scale == destScale && runState != SHRINK)
    {
        shrinkTime = Globals::ELAPSED_TIME;
        runState = SHRINK;
    }
}
