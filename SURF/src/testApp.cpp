#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(80,80,80);
    w = 640;
    h = 480;
    screenDisplay = 0;    
    myfont.loadFont("Helvetica Bold.Ttf", 62);

    /*zeroCenterW = (ofGetWidth()/2)-(w/2);
    zeroCenterH = (ofGetHeight()/2)-(h/2);
    screenDisplay = 0;
    
    myfont.loadFont("verdana.ttf", 62);
   */
    video.initGrabber(w,h, true);
    
    //reserve memory for cv images
    colorImg.allocate(w, h);
    grayImage.allocate(w, h);
    grayDiff.allocate(w, h);
    grayBg.allocate(w, h);
    grayFlowInput.allocate(320,240);
    optflow.setup(320,240);
    bLearnBakground = true;
	threshold = 40;
    ofEnableSmoothing();
    //updateVectorField();

}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    
    if (video.isFrameNew()) {
        

        colorImg.setFromPixels(video.getPixels(),w,h);
        
        grayImage = colorImg;
        if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
        // vector of keypoints
        // Construct the SURF feature detector object
        cv::SurfFeatureDetector surf(2500.); // threshold
        // Detect the SURF features
        surf.detect(grayImage.getCvImage(),keypoints);
        // Detect the SURF features
        surf.detect(grayBg.getCvImage(),keypointsBg);
        
        // Construction of the SURF descriptor extractor
        cv::SurfDescriptorExtractor surfDesc;
        // Extraction of the SURF descriptors
        cv::Mat descriptors1;
        surfDesc.compute(grayImage.getCvImage(),keypoints,descriptors1);
        
        // Extraction of the SURF descriptors
        cv::Mat descriptors2;
        surfDesc.compute(grayBg.getCvImage(),keypointsBg,descriptors2);

        // Construction of the matcher
        cv::BruteForceMatcher<cv::L2<float> > matcher;
        // Match the two image descriptors
        matcher.match(descriptors1,descriptors2, matches);
        
    }
         

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255,255);

    int zeroCenterW = (ofGetWidth()/2)-(w/2);
    int zeroCenterH = (ofGetHeight()/2)-(h/2);


    
    if(screenDisplay == 0)
    {
        grayImage.draw(zeroCenterW,zeroCenterH);
        myfont.drawString("Gray Image", 100,100);
    }
    else if (screenDisplay == 1)
    {
        ofNoFill();
        
        grayImage.draw(0,0,ofGetWidth(),ofGetHeight());
        for(int i=0; i< keypoints.size();++i)
        {
            ofSetColor(255,0,142);
            float xScale = ofMap(keypoints[i].pt.x,0,w,0,ofGetWidth());
            float yScale = ofMap(keypoints[i].pt.y, 0, h, 0, ofGetHeight());
            ofEllipse(xScale, yScale, keypoints[i].size*1.5, keypoints[i].size*1.5);
            //ofEllipse(keypoints[i].pt.x+zeroCenterW,keypoints[i].pt.y+zeroCenterH,keypoints[i].size,keypoints[i].size);
        }
        ofSetColor(51,255,255);
        myfont.drawString("SURF Image", 100,100);
    }
    else if (screenDisplay == 2)
    {
        std::nth_element(matches.begin(),    // initial position
                         matches.begin()+24, // position of the sorted element
                         matches.end());     // end position
        // remove all elements after the 25th
        matches.erase(matches.begin()+25, matches.end());
        
        ofNoFill();
        grayBg.draw(zeroCenterW-322,zeroCenterH);
        grayImage.draw(zeroCenterW+322,zeroCenterH);
        for(int i=0; i< keypoints.size();++i)
        {
            ofSetColor(255,0,142);
            ofEllipse(keypoints[i].pt.x+zeroCenterW+322,keypoints[i].pt.y+zeroCenterH,keypoints[i].size,keypoints[i].size);
            ofEllipse(keypointsBg[i].pt.x+zeroCenterW-322,keypointsBg[i].pt.y+zeroCenterH,keypointsBg[i].size,keypointsBg[i].size);


           /* float angle = keypoints[i].angle;
            float ptSize = (keypoints[i].size) / 3.0f;
            float r1 = floor(keypoints[i].pt.y+0.5);
            float c1 = floor(keypoints[i].pt.x+0.5);
            float c2 = floor((ptSize * cos(angle))+0.5) + c1;
            float r2 = floor((ptSize * sin(angle))+0.5) + r1;
            
            ofSetColor(0x00ff00);
            ofLine(c1+zeroCenterW+322,r1+zeroCenterH,c2+zeroCenterW+322,r2+zeroCenterH);
            */
        }
        
        for(int i=0; i< matches.size();++i)
        {
            ofSetColor(51,255,255);
           ofLine(keypointsBg[matches[i].trainIdx].pt.x+zeroCenterW-322,keypointsBg[matches[i].trainIdx].pt.y+zeroCenterH,keypoints[matches[i].queryIdx].pt.x+zeroCenterW+322,keypoints[matches[i].queryIdx].pt.y+zeroCenterH);
        }
        ofSetColor(255, 255, 255);
        myfont.drawString("SURF Match", 100,100);

    }
    
    myfont.drawString(">", ofGetWidth()-50,ofGetHeight()/2);

    
   /*
    for(int i = 0 ; i< optflow.points_nextPoints.size();++i)
    {
    //motion vel might be the vel between prev pts and next points? Yes!
    //cout << " " << optflow.getVelPixelAtIndex(i) << endl;
       ofCircle(optflow.points_nextPoints[i].x,optflow.points_nextPoints[i].y,2);
    
 //   motionVec = optflow.getVelPixelAtIndex(i);
    //ps.addtoVecField(motionVec.x/winWidth, motionVec.y/winHeight, motionVec, 0.15);
    }
   */ 
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key){
		case ' ':
			bLearnBakground = true;
            break;
		
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key == OF_KEY_RIGHT)
    {
        screenDisplay ++;
        screenDisplay = screenDisplay%3;
        ofSetColor(0, 0, 0);
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