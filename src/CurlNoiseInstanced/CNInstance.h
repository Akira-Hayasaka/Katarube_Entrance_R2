//
//  CNInstance.h
//  emptyExample
//
//  Created by Akira on 8/9/15.
//
//

#ifndef __emptyExample__CNInstance__
#define __emptyExample__CNInstance__

#include "ofMain.h"

#include "Globals.hpp"

class CNInstance
{
public:
    
    enum RUNSTATE
    {
        NONE,
        GROWUP,
        SHRINK
    };
    
    CNInstance(){};
    ~CNInstance(){};
    
    void setup(int instanceID);
    void update();
    
    void growup();
    void shrink();
    
    void setColor(ofFloatColor col) { this->col = col; }
    ofFloatColor getColor() { return col; }
    void setScale(float scale) { this->scale = scale; }
    float getScale() { return scale; }
    
private:
    
    RUNSTATE runState;
    
    int instanceID; // idx
    ofFloatColor col;
    float scale;
    float destScale;
    
    float growupTime;
    float shrinkTime;
};

#endif /* defined(__emptyExample__CNInstance__) */
