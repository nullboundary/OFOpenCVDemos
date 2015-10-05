#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

        ofTrueTypeFont myfont;
        int screenDisplay;
    
        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif

        ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage;

        int w,h;
    
 
    
    /** Function Headers */
    void detectAndDisplay( cv::Mat frame );
    cv::Mat videoFrame;
    /** Global variables */
    string face_cascade_name = "haarcascade_frontalface_alt.xml";
    string eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
    string profile_cascade_name = "haarcascade_profileface.xml";
    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eyes_cascade;
     cv::CascadeClassifier profile_cascade;
  



};

