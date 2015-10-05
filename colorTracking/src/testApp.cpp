#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(80,80,80);
    pixelColor.set(0,255,255);
    w = 640;
    h = 480;
    zeroCenterW = (ofGetWidth()/2)-(w/2);
    zeroCenterH = (ofGetHeight()/2)-(h/2);
    screenDisplay = 0;
    trailCount = 0;

    
    myfont.loadFont("Helvetica Bold.Ttf", 62);
    
    
    movie.initGrabber(w, h, true);
    
    //reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered.allocate(w, h);
}

//--------------------------------------------------------------
void testApp::update(){
    movie.update();
    
    if (movie.isFrameNew()) {
        
        //copy webcam pixels to rgb image
        rgb.setFromPixels(movie.getPixels(), w, h);
        
        //mirror horizontal
        rgb.mirror(false, true);
        
        //duplicate rgb
        hsb = rgb;
        
        //convert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for (int i=0; i<w*h; i++) {
            filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-2,findHue+2) ? 255 : 0;
        }
        filtered.flagImageChanged();
        
        //run the contour finder on the filtered image to find blobs with a certain hue
        contours.findContours(filtered, 10, w*h/3, 1, false);
        
        //for (int i=0; i<contours.nBlobs; i++) {
            trail.push_back(ofPoint(contours.blobs[0].centroid.x+zeroCenterW, contours.blobs[0].centroid.y+zeroCenterH,0));
            if(trail.size() > 10) trail.pop_front();
        //}
        //cout << trail.size() << endl;

        
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofEnableAlphaBlending();
    ofSetColor(255,255,255);
    zeroCenterW = (ofGetWidth()/2)-(w/2);
    zeroCenterH = (ofGetHeight()/2)-(h/2);
    
    if(screenDisplay == 0)
    {
        rgb.draw(zeroCenterW,zeroCenterH);
        myfont.drawString("Normal Image", 100,100);
        myfont.drawString("Click Tracking Color", 100,ofGetHeight()-60);

    }
    else if (screenDisplay == 1)
    {
        filtered.draw(zeroCenterW,zeroCenterH);
        contours.draw(zeroCenterW,zeroCenterH);
        myfont.drawString("Blob Tracking", 100,100);

    }
    else if (screenDisplay == 2)
    {
        hsb.draw(zeroCenterW,zeroCenterH);
        myfont.drawString("HSB Image", 100,100);

    }
    
    myfont.drawString(">", ofGetWidth()-50,ofGetHeight()/2);

    
    ofFill();
    
    //draw red circles for found blobs
    for (int i=0; i<contours.nBlobs; i++) {
        ofCircle(contours.blobs[i].centroid.x+zeroCenterW, contours.blobs[i].centroid.y+zeroCenterH, 20);
       
    }
    int count = 0; 
    for ( int i=0; i<trail.size(); ++i )
    {
        count++;
        ofColor trailColor =  pixelColor;
        trailColor.a = 25*count;
        ofSetColor(trailColor);

        ofCircle(trail[i].x,trail[i].y, 20);
     }
    

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
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    //calculate local mouse x,y in image
    int mx = x-zeroCenterW % w;
    int my = y-zeroCenterH % h;
    
    //get hue value on mouse position
    findHue = hue.getPixels()[my*w+mx];
    findSat = sat.getPixels()[my*w+mx];
    findBri = bri.getPixels()[my*w+mx];
    pixelColor.setHsb(abs(180-findHue),findSat,findBri);
    cout << "Hue" << " " << findHue << endl;
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