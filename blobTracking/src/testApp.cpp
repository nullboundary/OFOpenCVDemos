#include "testApp.h"

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
	grayBg.allocate(w,h);
	grayDiff.allocate(w,h);

    ofEnableSmoothing();
	bLearnBakground = true;
	threshold = 80;
  
    
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

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), w,h);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), w,h);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (640*480)/3, 10, true);	// find holes
	}

}

//--------------------------------------------------------------
void testApp::draw(){
    
    int zeroCenterW = (ofGetWidth()/2)-(w/2);
    int zeroCenterH = (ofGetHeight()/2)-(h/2);

	ofSetHexColor(0xffffff);
    
    if(screenDisplay == 0)
    {
        colorImg.draw(0,0,ofGetWidth(),ofGetHeight());
        myfont.drawString("Color Image", 100,100);
    }
    else if (screenDisplay == 1)
    {
        grayImage.draw(0,0,ofGetWidth(),ofGetHeight());
        myfont.drawString("Gray Image", 100,100);
    }
    else if (screenDisplay == 2)
    {

        grayBg.draw(zeroCenterW,zeroCenterH);
        myfont.drawString("Background Image", 100,100);
        myfont.drawString("Press Space", 100,ofGetHeight()-60);

	}
    else if (screenDisplay == 3)
    {
        grayImage.draw(zeroCenterW-190,zeroCenterH,160,120);
        grayDiff.draw(zeroCenterW,zeroCenterH);
        grayBg.draw(zeroCenterW+670,zeroCenterH,160,120);
        myfont.drawString("Difference Image", 100,100);
        char thresholdStr[255]; // an array of chars
        sprintf(thresholdStr, "+/- Threshold: %i", threshold);
        myfont.drawString(thresholdStr, 100,ofGetHeight()-60);
    }
    else if (screenDisplay == 4)
    {
         // then draw the contours:
        myfont.drawString("Contours", 100,100);
        
        //scaleFullscreen.begin(); //draw into fbo
        ofSetColor(255,0,0,255);
        grayImage.draw(0,0,ofGetWidth() ,ofGetHeight());
        ofSetHexColor(0xffffff);
       
        
        for (int i = 0; i < contourFinder.nBlobs; i++){
            
            // draw over the centroid if the blob is a hole
            ofSetLineWidth(2);
            ofSetColor(255);
            ofNoFill();
            
            float rectX = ofMap(contourFinder.blobs[i].boundingRect.x, 0, w, 0, ofGetWidth());
            float rectY = ofMap(contourFinder.blobs[i].boundingRect.y, 0, h, 0, ofGetHeight());
            float rectW = ofMap(contourFinder.blobs[i].boundingRect.width, 0, w, 0, ofGetWidth());
            float rectH = ofMap(contourFinder.blobs[i].boundingRect.height, 0, w, 0, ofGetHeight());
            
            ofRect(rectX,rectY,rectW,rectH);
            
            
            ofBeginShape();
            for( int j=0; j<contourFinder.blobs[i].nPts; j++ ) {
                
                float mapX = ofMap(contourFinder.blobs[i].pts[j].x, 0, w, 0, ofGetWidth());
                float mapY = ofMap(contourFinder.blobs[i].pts[j].y, 0, h, 0, ofGetHeight());
                ofVertex(mapX, mapY);
            }
            ofEndShape();
        }
     
	    // we could draw the whole contour finder
	    //contourFinder.draw(zeroCenterW,zeroCenterH);
        //  contourFinder.draw(0, 0, ofGetWidth(), ofGetHeight());
        
        char thresholdStr[255]; // an array of chars
        sprintf(thresholdStr, "+/- Threshold: %i", threshold);
        myfont.drawString(thresholdStr, 100,ofGetHeight()-60);
    }
    myfont.drawString(">", ofGetWidth()-50,ofGetHeight()/2);

    
	

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '=':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

    if(key == OF_KEY_RIGHT)
    {
        screenDisplay ++;
        screenDisplay = screenDisplay%5;
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
