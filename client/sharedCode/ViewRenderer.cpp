//
//  ViewRenderer.cpp
//  sceneView
//
//  Created by dantheman on 3/14/15.
//
//

#include "ViewRenderer.h"


ViewRenderer::ViewRenderer(){
    
}
ViewRenderer::~ViewRenderer(){
    
}

void ViewRenderer::setupViewports(){
    int xOffset = ofGetWindowWidth() / 2;
    int yOffset = ofGetWindowHeight() / 2;
    int x = 0;
    int y = 0;
    
    for(int i = 0; i < N_SCENE; i++){
        viewGrid[i].x = x;
        viewGrid[i].y = y;
        viewGrid[i].width = xOffset;
        viewGrid[i].height = yOffset;
        
        if(i == 0){
            x = xOffset;
            y = 0;
        }
        if(i == 1){
            x = 0;
            y = yOffset;
        }
        if(i == 2){
            x = xOffset;
            y = yOffset;
        }
    }
    
    viewMain = ofRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}
void ViewRenderer::setup(int numPlayers){
    setupViewports();
    iMainView = 4;
    
    this->numPlayers = numPlayers;
    kinectSkeleton.setup(this->numPlayers);
    
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->openVirtualPort("OF Kinect");
    
    musicMaker.setup(this->numPlayers, midiOut);
    floor.setup(ofRectangle(ofGetScreenWidth(), 0, 1280, 800));
    
    skeletonView.allocate(1920, 1080, GL_RGBA, 4);
    midiView.allocate(1920, 1080, GL_RGBA, 4);
    floorView.allocate(1920, 1080, GL_RGBA, 4);
    tvView.allocate(1920, 1080, GL_RGBA, 4);
    
    skeletonView.begin();
    ofClear(0, 0, 0, 0);
    skeletonView.end();
    
    midiView.begin();
    ofClear(0, 0, 0, 0);
    midiView.end();
    
    floorView.begin();
    ofClear(0, 0, 0, 0);
    floorView.end();
    
    tvView.begin();
    ofClear(0, 0, 0, 0);
    tvView.end();
    
    views.push_back(&skeletonView);
    views.push_back(&midiView);
    views.push_back(&floorView);
    views.push_back(&tvView);
    
}
void ViewRenderer::update(){
    
    kinectSkeleton.update();
    floor.update();
    
    if(numPlayers == 1){
        musicMaker.update(kinectSkeleton.getBody(0), NULL);
    }else{
        musicMaker.update(kinectSkeleton.getBody(0), kinectSkeleton.getBody(1));
    }
    ofEnableAlphaBlending();
    skeletonView.begin();
    ofClear(0, 0, 0, 0);
    kinectSkeleton.draw();
    skeletonView.end();
    
    midiView.begin();
    ofClear(0, 0, 0);
    musicMaker.draw();
    midiView.end();
    
    floorView.begin();
    ofClear(0, 0, 0);
    floor.draw();
    floorView.end();
    
    tvView.begin();
    ofClear(0, 0, 0);
    ofSetColor(255, 255, 0);
    ofRect(ofRectangle(0, 0, tvView.getWidth(), tvView.getHeight()));
    tvView.end();
    ofDisableAlphaBlending();
}

void ViewRenderer::exit(){
    kinectSkeleton.exit();
    musicMaker.exit();
}


void ViewRenderer::draw(){
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
//    ofEnableDepthTest();
    
    if(iMainView == N_SCENE){
        ofSetColor(255, 255, 255, 255);
        for(int i = 0; i < views.size(); i++){
            views[i]->draw(viewGrid[i]);
        }
    }else{
        ofSetColor(255, 255, 255, 255);
        views[iMainView]->draw(viewMain);
    }
    floor.drawProjections();
    
//    ofDisableDepthTest();
    ofDisableAlphaBlending();
    
}
void ViewRenderer::setMainView(int i){
    iMainView = i;
    if(iMainView == 0){
        kinectSkeleton.setMainView(true);
    }else{
        kinectSkeleton.setMainView(false);
    }
    if(iMainView == 1){
        musicMaker.setMainView(true);
    }else{
        musicMaker.setMainView(false);
    }
    if(iMainView == 2){
        floor.setMainView(true);
    }else{
        floor.setMainView(false);
    }
}
ofRectangle* ViewRenderer::getViews(){
    return &viewGrid[0];
}