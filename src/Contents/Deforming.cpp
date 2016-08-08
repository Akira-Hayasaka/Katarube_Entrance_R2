//
//  Deforming.cpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#include "Deforming.hpp"

void Deforming::setup()
{
    masterDir.open("imgs/deform");
    masterDir.listDir();
    for (int i = 0; i < masterDir.size(); i++)
    {
        ofFile f = masterDir.getFile(i);
        if (isImgFile(f.getExtension()))
        {
            ofPtr<Deformer> p = ofPtr<Deformer>(new Deformer);
            Deformer::ContourFinderSettings cfs;
            cfs.minArea = Globals::minArea;
            cfs.maxArea = Globals::maxArea;
            cfs.threshold = Globals::threshold;
            cfs.holes = Globals::holes;
            p->setup(f.getAbsolutePath(), cfs);
            storedPuppets.push_back(p);
        }
    }
    
    deformTo = storedPuppets.at(getRdmIdx());
    restoreFrom = storedPuppets.at(getRdmIdx());
    
    deformTo->makeReadyToDeform(restoreFrom->getOutline());
    restoreFrom->makeReadyToRestore(deformTo->getOutline());
    
    deformTo->start();
    restoreFrom->start();
    ofAddListener(restoreFrom->finEvent, this, &Deforming::onFinishEvent);
}

void Deforming::update()
{
    deformTo->update();
    restoreFrom->update();
    
    rot.x -= 0.05;
    rot.y -= 0.05;
    rot.z -= 0.05;
}

void Deforming::draw()
{
    deformTo->draw(rot);
    restoreFrom->draw(rot);
}

int Deforming::getRdmIdx()
{
    if (imageHistory.size() > 100)
        imageHistory.pop_front();
    
    if (imageHistory.empty())
    {
        int rdm = ofRandom(masterDir.size());
        imageHistory.push_back(rdm);
        return rdm;
    }
    else
    {
        int lastImg = imageHistory.back();
        int nextImg = 0;
        
        // try 10 time
        for (int i = 0; i < 10; i++)
        {
            nextImg = ofRandom(masterDir.size());
            if (nextImg != lastImg)
            {
                imageHistory.push_back(nextImg);
                return nextImg;
            }
        }
    }
}

void Deforming::onFinishEvent()
{
    ofRemoveListener(restoreFrom->finEvent, this, &Deforming::onFinishEvent);
    
    deformTo = restoreFrom;
    restoreFrom.reset();
    restoreFrom = storedPuppets.at(getRdmIdx());
    
    deformTo->makeReadyToDeform(restoreFrom->getOutline());
    restoreFrom->makeReadyToRestore(deformTo->getOutline());
    
    deformTo->start();
    restoreFrom->start();
    
    ofNotifyEvent(Globals::makeRandomMorphSoundEvent);
    
    ofAddListener(restoreFrom->finEvent, this, &Deforming::onFinishEvent);
}