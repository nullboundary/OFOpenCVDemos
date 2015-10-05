#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxOpticalFlowLK.h"
#include "vectorField.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void updateVectorField();
        void addField(float xIndex, float yIndex, ofPoint vec);
        void addtoVecField(float x, float y, ofPoint vec, float radius = 1.0);
        ofPoint prevFieldVec(ofPoint p);


		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofVideoGrabber video;
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxOpticalFlowLK optflow;
    ofxCvGrayscaleImage grayFlowInput;
    ofxCvColorImage			colorImg;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;

    ofTrueTypeFont myfont;
    std::vector<cv::KeyPoint> keypoints,keypointsBg;
    std::vector<cv::DMatch> matches;

    vectorField vecField;
    
    int w,h;
    int 				threshold;
    bool				bLearnBakground;
    int screenDisplay;

		
};
