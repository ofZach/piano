#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofRectangle tv(ofGetScreenWidth(), 0, 1920, 1080);
    ofRectangle main(0, 0, ofGetScreenHeight(), ofGetScreenWidth());
    tvView.setup(1, tv, main);
}

//--------------------------------------------------------------
void ofApp::update(){
//    tvView.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    tvView.draw(ofGetCurrentViewport());
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
