#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include <deque>



class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofVideoGrabber movie;
    
        ofxCvColorImage rgb,hsb;
        ofxCvGrayscaleImage hue,sat,bri,filtered;
        ofxCvContourFinder contours;
    
        ofTrueTypeFont myfont;
        deque <ofPoint> trail;
        int trailCount;

    
        int screenDisplay;
        int w,h;
        float findHue,findSat,findBri;
        int zeroCenterW , zeroCenterH;
        ofColor pixelColor;
};
