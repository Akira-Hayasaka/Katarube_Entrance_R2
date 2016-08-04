//
//  TexFlow.cpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/3/16.
//
//

#include "TexFlow.hpp"

void TexFlow::setup()
{
    cni = ofPtr<CurlNoiseInstanced>(new CurlNoiseInstanced());
    cni->setup(true, 32);
    
    ofDisableArbTex();
    ofLoadImage(Globals::testTex, "imgs/deform/23e.png");
    ofEnableArbTex();
}

void TexFlow::update()
{
    cni->update(0.15);
}

void TexFlow::draw()
{
    ofPushMatrix();
    ofTranslate(Globals::cniParams->offset);
    cni->draw(Globals::cam.get());
    ofPopMatrix();
}