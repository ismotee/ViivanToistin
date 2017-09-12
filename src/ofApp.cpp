#include "ofApp.h"


std::string getFilename() {
    //hae uuden framen kuvatiedoston nimi. huom: ei tarkisteta mitenkään meneekö vanhan päälle
    static int n = 0;
    string result = "frames/frame"+ofToString(n)+".png";
    n++;
    return result;
}


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(ofColor::black);    
    Ohjain::setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    Ohjain::update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    Multimonitori::draw();
    
    //kuvan tallennus:
    /*
    ofImage screenshot;
    screenshot.grabScreen(0,0,ofGetWidth(), ofGetHeight() );
    screenshot.save(getFilename() );
     */
    
    Ohjain::debugDraw(20,20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    Ohjain::keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}