//
//  NowExibit.cpp
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#include "NowExibit.hpp"

void NowExibit::setup()
{
    ofLoadImage(exibitionTex, "imgs/nowExibit/tenji.png");
}

void NowExibit::update()
{
    rot.x += 0.05;
    rot.y += 0.05;
    rot.z += 0.05;
}

void NowExibit::draw()
{
    ofPushMatrix();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(ONESCRN_W/2 - 700, ONESCRN_H/2 + 450);
//    ofRotateX(rot.x);
//    ofRotateY(rot.y);
//    ofRotateZ(rot.z);
    exibitionTex.draw(0, 0);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
}