#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    skelView.setup(1, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    skeletonGUI.setup(skelView.skeletonTransform);
    stageGUI.setup(skelView.stageParams);
    hiddenSettings.setup(skelView.hiddenSettings);
}

//--------------------------------------------------------------
void ofApp::update(){
    skelView.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    skelView.draw(ofGetCurrentViewport());

    skeletonGUI.draw();
    stageGUI.draw();
    hiddenSettings.draw();
    
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
