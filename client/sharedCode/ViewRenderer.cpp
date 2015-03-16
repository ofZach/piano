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
    int xOffset = viewMain.width / 2;
    int yOffset = viewMain.height / 2;
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
//    
//    viewMain = ofRectangle(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
}
void ViewRenderer::setup(int numPlayers, ofRectangle mainview){
    viewMain = mainview;
    setupViewports();
    iMainView = 4;
    
    this->numPlayers = numPlayers;
    kinectSkeleton.setup(this->numPlayers, mainview);
    
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->listPorts();
    midiOut->openPort(0);
    
    musicMaker.setup(this->numPlayers, midiOut, viewMain);
    floor.setup(ofRectangle(ofGetScreenWidth(), 0, 1280, 768), viewMain);
    tv.setup(ofRectangle(ofGetScreenWidth()+1280, 0, 1920, 1080), viewMain);
    
    skeletonView.allocate(mainview.width, mainview.height, GL_RGBA, 4);
    midiView.allocate(mainview.width, mainview.height, GL_RGBA, 4);
    floorView.allocate(mainview.width, mainview.height, GL_RGBA, 4);
    tvView.allocate(mainview.width, mainview.height, GL_RGBA, 4);
    
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
        tv.update(kinectSkeleton.getBody(0), NULL);
    }else{
        musicMaker.update(kinectSkeleton.getBody(0), kinectSkeleton.getBody(1));
        tv.update(kinectSkeleton.getBody(0), kinectSkeleton.getBody(1));
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
    tv.draw();
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

    
    if(kinectSkeleton.isMain()){
        kinectSkeleton.drawControlPanel();
    }
    if(musicMaker.isMain()){
        musicMaker.drawControlPanel();
    }
    if(floor.isMain()){
        floor.drawControlPanel();
    }
    if(tv.isMain()){
        tv.drawControlPanel();
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
    if(iMainView == 3){
        tv.setMainView(true);
    }else{
        tv.setMainView(false);
    }
}
ofRectangle* ViewRenderer::getViews(){
    return &viewGrid[0];
}