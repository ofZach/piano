#include "ofApp.h"



void ofApp::setup(){
	ofSetFrameRate(30);
    ofSetVerticalSync(true);
   	ofSetFrameRate(30);

	ofSetLogLevel(OF_LOG_NOTICE);
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
    
    if(key == ' '){
        views.setMainView(N_SCENE-2);
    }
    
    if(key == 'e'){
        views.bExpertMode = !views.bExpertMode;
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
