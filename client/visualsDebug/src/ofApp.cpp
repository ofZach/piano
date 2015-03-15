#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    view.setup(ofRectangle(0, 0, 1280, 800));
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    view.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    view.draw();
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
