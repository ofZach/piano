#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    //void FloorView::setup(ofRectangle projector, ofRectangle main){
    
    mainScreen.set(0,0, ofGetWindowWidth(), ofGetWindowHeight());
    projectionScreen.set(mainScreen.width,0, 1280, 768);
    
    floorView.setup(2,projectionScreen, mainScreen);
    floorView.setMainView(true);
    
    
    drawProjections = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    floorView.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);

    floorView.drawDebug();
    
    floorView.drawProjections();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    floorView.addLineTrace(0);
    floorView.addLineTrace(1);
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
