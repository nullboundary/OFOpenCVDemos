#include "testApp.h"

//using namespace cv;

//--------------------------------------------------------------
void testApp::setup(){

    w = 640;
    h = 480;
    
	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(w,h);
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    screenDisplay = 0;
    myfont.loadFont("Helvetica Bold.Ttf", 62);
    
    colorImg.allocate(w,h);
	grayImage.allocate(w,h);

    
    //-- 1. Load the cascades
    //ofFile cascade;
    //cascade.open(face_cascade_name);
    
    if( !face_cascade.load( ofToDataPath(face_cascade_name) ) ){ printf("--(!)Error loading\n"); return -1; };
    if( !eyes_cascade.load( ofToDataPath(eyes_cascade_name) ) ){ printf("--(!)Error loading\n"); return -1; };
    if( !profile_cascade.load( ofToDataPath(profile_cascade_name) ) ){ printf("--(!)Error loading\n"); return -1; };

    ofEnableSmoothing();

}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(80,80,80);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame)
    {

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), w,h);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), w,h);
        #endif

        grayImage = colorImg;
        cv::Mat matCon(grayImage.getCvImage());
		videoFrame = matCon; 
      
	}

}

//--------------------------------------------------------------
void testApp::draw(){
    
    int zeroCenterW = (ofGetWidth()/2)-(w/2);
    int zeroCenterH = (ofGetHeight()/2)-(h/2);

	ofSetHexColor(0xffffff);
    ofSetLineWidth(2);
    vector<cv::Rect> faces;
    
    
    if(screenDisplay == 0)
    {
        colorImg.draw(zeroCenterW,zeroCenterH);
        myfont.drawString("Color Image", 100,100);
    }
    else if (screenDisplay == 1)
    {
        grayImage.draw(zeroCenterW,zeroCenterH);
        myfont.drawString("Gray Image", 100,100);
    }
    else if (screenDisplay == 2)
    {
        grayImage.draw(zeroCenterW,zeroCenterH);
        //-- Detect faces
        face_cascade.detectMultiScale( videoFrame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30) );
        
        for( int i = 0; i < faces.size(); i++ )
        {
            ofNoFill();
            ofSetColor(255,0,142);
            ofPoint center ((faces[i].x + faces[i].width*0.5)+zeroCenterW, (faces[i].y + faces[i].height*0.5)+zeroCenterH,0);
            //ofEllipse(center, faces[i].width, faces[i].height);
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofRect(center, faces[i].width, faces[i].height);
            ofSetRectMode(OF_RECTMODE_CORNER);
            cv::Mat faceROI = videoFrame( faces[i] );
            vector<cv::Rect> eyes;
            
            //-- In each face, detect eyes
       //     eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );
            
        //    for( int j = 0; j < eyes.size(); j++ )
       //     {
        //      ofPoint eyeCenter ((faces[i].x + eyes[j].x + eyes[j].width*0.5)+zeroCenterW,(faces[i].y + eyes[j].y + eyes[j].height*0.5)+zeroCenterH,0);
          
        //        ofEllipse(eyeCenter, eyes[j].width, eyes[j].height);
           
       //     }
        }
        
        ofSetColor(255,255,255);
        myfont.drawString("Face & Eye Tracking", 100,100);
        //myfont.drawString("Press Space", 100,ofGetHeight()-60);

	}
    
    
    myfont.drawString(">", ofGetWidth()-50,ofGetHeight()/2);

    
	

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

    if(key == OF_KEY_RIGHT)
    {
        screenDisplay ++;
        screenDisplay = screenDisplay%3;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
