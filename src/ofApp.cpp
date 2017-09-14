#include "ofApp.h"


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
    Ohjain::draw();
    
    Ohjain::debugDraw(20,20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    Ohjain::keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    Ohjain::keyReleased(key);
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