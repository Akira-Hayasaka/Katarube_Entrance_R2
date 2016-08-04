//
//  Utils.h
//  Entrance2
//
//  Created by Akira Hayasaka on 8/2/16.
//
//

#ifndef Utils_h
#define Utils_h

#include "ofMain.h"

static bool isImgFile(string ext)
{
    if (ext == "jpg" || ext == "JPG" ||
        ext == "png" || ext == "PNG")
    {
        return true;
    }
    else
        return false;
}

// if shaper is 1, you get linear output like ofMap.
// shaper > 1 means non linear steps weighted towards the start.
// 0 < shaper < 1 means non linear steps weighted towards the end.
static float shapedMap(float val,
                       float inMin, float inMax,
                       float outMin, float outMax,
                       float shaper){
    // (1) convert to pct (0-1)
    float pct = ofMap (val, inMin, inMax, 0, 1, true);
    // raise this number to a power
    pct = powf(pct, shaper);
    float rval = ofMap(pct, 0,1, outMin, outMax, true);
    return rval;
}

static vector<ofPoint> getCircularPts(float pRadius, ofPoint orig, int devide)
{
    float angle = PI * 2 / devide;
    
    vector<ofPoint> rtn;
    for (int i = 0; i < devide; i++)
    {
        ofPoint p(orig.x + pRadius * cos(angle * i),
                  orig.y + pRadius * sin(angle * i));
        rtn.push_back(p);
    }
    return rtn;
}

#endif /* Utils_h */
