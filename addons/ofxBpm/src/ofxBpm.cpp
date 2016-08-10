//
//  ofxBpm.cpp
//  ofxBpm
//
//  Created by mirrorboy on 2013/08/14.
//
//


#include "ofxBpm.h"
#include "ofMain.h"

ofxBpm::ofxBpm(float bpm,int beatPerBar):_beatPerBar(beatPerBar)
{
    _isPlaying = false;
    setBpm(bpm);
};


void ofxBpm::start()
{
    stop();
    _isPlaying = true;
    reset();
    startThread();
}

void ofxBpm::stop()
{
    waitForThread(true);
    _isPlaying = false;
}

void ofxBpm::reset()
{
//    ofScopedLock lock(mutex);
    _isPlaying = true;
    _remainderOfTick = 0;
    _countOfTick = -1;
    _preTime = ofGetElapsedTimeMicros();
    _totalTime = 0;
}


void ofxBpm::threadedFunction()
{
    while (isThreadRunning() != 0 )
    {
//        if (lock())
//        {
            if (_isPlaying == true)
            {
                unsigned long long nowTime = ofGetElapsedTimeMicros();
                unsigned long long deltaTime = nowTime - _preTime;
                _preTime = nowTime;
                
                if (_totalTime + _remainderOfTick >= _durationOfTick)
                {
                    if ((_countOfTick % (OFX_BPM_TICK / _beatPerBar)) == 0)
                    {
                        ofNotifyEvent(beatEvent);
                    }
                    
                    _remainderOfTick = (_totalTime + _remainderOfTick) % _durationOfTick;
                    
                    _totalTime = 0.;

                    _countOfTick++;
                    _countOfTick %= OFX_BPM_TICK;
                }
                
                _totalTime += deltaTime;
            }
            
//            unlock();
//        }
    }
    
}

void ofxBpm::setBeatPerBar(int beatPerBar)
{
//    ofScopedLock lock(mutex);
    _beatPerBar = beatPerBar;
}

void ofxBpm::setBpm(float bpm)
{
//    ofScopedLock lock(mutex);
    
    if(bpm < OFX_BPM_MIN)
    {
        _bpm = OFX_BPM_MIN;
    }
    else if(bpm >= OFX_BPM_MAX)
    {
        _bpm = OFX_BPM_MAX;
    }
    else
    {
        _bpm = bpm;
    }
    
    _durationOfTick = 60. * 1000. * 1000. / (bpm * (OFX_BPM_TICK >> 2));
}

float ofxBpm::getBpm() const
{
//    ofScopedLock lock(mutex);
    return _bpm;
}

bool ofxBpm::isPlaying() const
{
//    ofScopedLock lock(mutex);
    return _isPlaying;
}
