#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->openVirtualPort("OF Kinect");
    midiView.setup(2, midiOut);
}


void ofApp::addLineTracePlayerOne(){

}
void ofApp::triggerTrianglePlayerOne(){
    
}
void ofApp::addImpulsePlayerOne(){

}

void ofApp::addLineTracePlayerTwo(){
 
}
void ofApp::triggerTrianglePlayerTwo(){
    
}
void ofApp::addImpulsePlayerTwo(){
 
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    midiView.draw(ofGetCurrentViewport());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::addLineTrace(){
    
}

void ofApp::addImpulse(){
    
}

void ofApp::triggerTriangle(){
    
}