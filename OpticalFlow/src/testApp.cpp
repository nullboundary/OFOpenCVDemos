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
    
    //updateVectorField();

}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    
    if (video.isFrameNew()) {
        
        //updateVectorField();

        colorImg.setFromPixels(video.getPixels(),w,h);
        
        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
        
		// take the abs value of the difference between background and incoming and then threshold:
		//grayDiff.absDiff(grayBg, grayImage);
		//grayDiff.threshold(threshold);
        //grayImage.threshold(threshold);
        //grayImage.setFromPixels(video.getPixels(), w, h);
        
        //grayDiff.flagImageChanged();
        grayImage.flagImageChanged();
        //grayFlowInput.scaleIntoMe(grayDiff);
        grayFlowInput.scaleIntoMe(grayImage);
        grayFlowInput.threshold(threshold);
        optflow.setOpticalFlowSize(13);
        optflow.setOpticalFlowBlur(13);
        optflow.update(grayFlowInput);
        /*
        float scaleX = (float) ofGetWidth() / (float)grayDiff.width;
        float scaleY = (float) ofGetWidth() / (float)grayDiff.height;
        
        float vecScaleX = (float) 80 / grayDiff.getWidth();
        float vecScaleY = (float) 60 / grayDiff.getHeight();
        
        ofPoint motionVec ;
        
        int gImageH = grayDiff.getHeight();
        int gImageW = grayDiff.getWidth();
        
        // cout << optflow.points_nextPoints.size() << endl;
        ofPoint vecLoc;
        
        
        
        for(int i = 0 ; i< optflow.points_nextPoints.size();++i)
        {
            //motion vel might be the vel between prev pts and next points? Yes!
            //cout << " " << optflow.getVelPixelAtIndex(i) << endl;
            //ofCircle(optflow.points_nextPoints[i].x,optflow.points_nextPoints[i].y,2);
            
            vecLoc.x = ofNormalize(optflow.points_nextPoints[i].x,0,320);
            vecLoc.y = ofNormalize(optflow.points_nextPoints[i].y,0,240);
            //vecLoc.x = vecLoc.x * ofGetWidth();
            //vecLoc.y = vecLoc.y * ofGetHeight();
            motionVec = optflow.getVelPixelAtIndex(i);
            //cout << "vecLoc:" << vecLoc << " motionVec:" << motionVec << endl;
            
            
            addtoVecField(vecLoc.x, vecLoc.y, motionVec, 0.1);
        }
         */
        
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
        grayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(255, 0, 142);
        optflow.draw(ofGetWidth(),ofGetHeight(),30,15);
        myfont.drawString("Optical Flow", 100,100);
    }
    
    ofSetColor(255,255,255);

    myfont.drawString(">", ofGetWidth()-50,ofGetHeight()/2);

    
}

void testApp::updateVectorField()
{
   
    ofPoint motionVector;
    
	float xoff = 0;
	for(int yy = 0; yy < 60; yy++){  //60 vectorfield bin
		float yoff = 0;
		for(int xx = 0; xx < 80; xx++){ //80 vectorfield bin
            
			// Use perlin noise to get an angle between 0 and 2 PI
			//float theta = ofMap(ofNoise(xoff,yoff,z),0,1,0,TWO_PI);
			// Polar to cartesian coordinate transformation to get x and y components of the vector
			//motionVector.set(cos(theta),sin(theta),0);
            //motionVector.set(0,0,0);
            //addtoVecField(xx, yy, motionVector, -1.1);

            //vecField.setFieldVectorByIndex(xx, yy, motionVector);
            
            
            //Equalize or slowly flatten out the vector field
            vecField.field[xx][yy] = vecField.field[xx][yy] - (vecField.field[xx][yy] - 0.3);
			//vecField.setFieldVectorByIndex(xx, yy, motionVector*0.6);
			yoff += 0.1;
		}
		xoff += 0.1;
	}
    
}

void testApp::addtoVecField(float x, float y, ofPoint vec, float radius)
{
    vecField.addIntoField(x, y, vec, radius);
}

void testApp::addField(float xIndex, float yIndex, ofPoint vec)
{
    
    vecField.addToFieldVectorByIndex(xIndex, yIndex, vec);
}

ofPoint testApp::prevFieldVec(ofPoint p)
{
    return vecField.readFromField(p.x,p.y);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key){
		case ' ':
			bLearnBakground = true;
            updateVectorField();
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
        screenDisplay = screenDisplay%2;
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