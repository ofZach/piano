#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    //ofSetEscapeQuitsApp(false);
    views.setup();
    views.setMainView(5);
}


//--------------------------------------------------------------
void ofApp::update(){
   views.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    views.draw();
    
}

void ofApp::exit(){
    views.exit();
}


void ofApp::addLineTracePlayerOne(){
    views.addLineTracePlayerOne();
}
void ofApp::triggerTrianglePlayerOne(){

}
void ofApp::addImpulsePlayerOne(){
    views.addImpulsePlayerOne();
}

void ofApp::addLineTracePlayerTwo(){
    views.addLineTracePlayerTwo();
}
void ofApp::triggerTrianglePlayerTwo(){
    
}
void ofApp::addImpulsePlayerTwo(){
    views.addImpulsePlayerTwo();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == OF_KEY_LEFT){
        int foo = views.iMainView;
        foo--;
        if(foo < 0){
            views.setMainView(N_SCENE);
        }else{
            views.setMainView(foo);
        }
    }
    
    if(key == OF_KEY_RIGHT){
        int foo = views.iMainView;
        foo++;
        if(foo > N_SCENE){
            views.setMainView(0);
        }else{
            views.setMainView(foo);
        }
    }
    
    if(key == OF_KEY_ESC){
        views.setMainView(N_SCENE);
    }
    
    
    if (key == 'f'){
        ofToggleFullscreen();
    }
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
//    ofVec2f fooMousePoint(x, y);
//    if(views.iMainView == N_SCENE){
//        for(int i = 0; i < N_SCENE; i++){
//            if(views.getViews()[i].inside(fooMousePoint)){
//                views.setMainView(i);
//            }
//        }
//    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //views.setupViewports();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
