//
//  SE.hpp
//  Entrance_r2
//
//  Created by Akira Hayasaka on 8/7/16.
//
//

#ifndef SE_hpp
#define SE_hpp

#include "ofMain.h"
#include "Globals.hpp"

class Sound : public ofSoundPlayer
{
public:
    
    bool loadSound(string fileName, bool stream = false);
    
    string getFileName() { return fileName; }
    
private:
    
    string fileName;
};

class SE
{
public:
    
    void setup();
    void makeSound(string sndName);
    void makeRdmSound();
    
private:
    
    vector<Sound> snds;
};

#endif /* SE_hpp */
