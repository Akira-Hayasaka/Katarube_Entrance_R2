#include "ofAppGlutWindow.h"
#include "ofApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800, 800, OF_WINDOW);
	ofRunApp(new ofApp());
}
