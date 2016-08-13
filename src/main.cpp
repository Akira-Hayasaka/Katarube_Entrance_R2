#ifdef WIN32
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#include "ofMain.h"
#include "ofApp.h"
#include "Constants.h"

int main()
{
    ofGLFWWindowSettings settings;
    settings.width = APP_W + 1920;
    settings.height = APP_H;
    settings.multiMonitorFullScreen = true;
    settings.windowMode = OF_FULLSCREEN;
    settings.setGLVersion(3,2);
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}

