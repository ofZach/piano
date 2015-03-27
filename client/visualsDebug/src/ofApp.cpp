#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    //void FloorView::setup(ofRectangle projector, ofRectangle main){
    
    mainScreen.set(0,0, ofGetScreenWidth(), ofGetScreenHeight());
    projectionScreen.set(mainScreen.width,0, 1280, 768);
    
    floorView.setup(2,projectionScreen, mainScreen);
    floorView.setMainView(true);
    
    projectionGUI.setup(floorView.squareOptions);
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
    if(drawProjections){
        floorView.draw(ofGetCurrentViewport());
    }else{
        floorView.drawDebug();
    }
    
    projectionGUI.draw();
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
