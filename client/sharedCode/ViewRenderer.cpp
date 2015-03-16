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

    
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->listPorts();
    midiOut->openPort(0);
    
    
//    stageParams.setName("Stage Settings");
//    stageParams.add(stageLeftX.set("Left Stage X", 0, -500, 500));
//    stageParams.add(stageLeftY.set("Left Stage Y", 0, -500, 500));
//    stageParams.add(stageLeftZ.set("Left Stage Z", 0, -500, 500));
//    stageParams.add(stageRightX.set("Right Stage X", 0, -500, 500));
//    stageParams.add(stageRightY.set("Right Stage Y", 0, -500, 500));
//    stageParams.add(stageRightZ.set("Right Stage Z", 0, -500, 500));
//    stageParams.add(stageSize.set("Stage Size", 300, 1, 500));
    
    
    kinectSkeleton.setup(this->numPlayers, viewMain);
    musicMaker.setup(this->numPlayers, midiOut, viewMain);
    floor.setup(ofRectangle(ofGetScreenWidth(), 0, 1280, 768), viewMain);
    tv.setup(ofRectangle(ofGetScreenWidth()+1280, 0, 1920, 1080), viewMain);
    
//    skeletonView.allocate(mainview.width, mainview.height, GL_RGBA, 4);
//    midiView.allocate(mainview.width, mainview.height, GL_RGBA, 4);
//    floorView.allocate(mainview.width, mainview.height, GL_RGBA, 4);
//    tvView.allocate(mainview.width, mainview.height, GL_RGBA, 4);
//    
//    skeletonView.begin();
//    ofClear(0, 0, 0, 0);
//    skeletonView.end();
//    
//    midiView.begin();
//    ofClear(0, 0, 0, 0);
//    midiView.end();
//    
//    floorView.begin();
//    ofClear(0, 0, 0, 0);
//    floorView.end();
//    
//    tvView.begin();
//    ofClear(0, 0, 0, 0);
//    tvView.end();
    
//    views.push_back(&skeletonView);
//    views.push_back(&midiView);
//    views.push_back(&floorView);
//    views.push_back(&tvView);
    
}
void ViewRenderer::update(){
    
    kinectSkeleton.update();
    floor.update();
    
    if(numPlayers <= 1){
        musicMaker.update(kinectSkeleton.getBody(0), NULL);
        tv.update(kinectSkeleton.getBody(0), NULL);
    }else{
        musicMaker.update(kinectSkeleton.getBody(0), kinectSkeleton.getBody(1));
        tv.update(kinectSkeleton.getBody(0), kinectSkeleton.getBody(1));
    }
}

void ViewRenderer::exit(){
    kinectSkeleton.exit();
    musicMaker.exit();
}


void ViewRenderer::draw(){
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    
    if(iMainView == N_SCENE){
        ofSetColor(255, 255, 255, 255);
        kinectSkeleton.draw(viewGrid[0]);
        musicMaker.draw(viewGrid[1]);
        floor.draw(viewGrid[2]);
        tv.draw(viewGrid[3]);
    }

    
    if(kinectSkeleton.isMain()){
        kinectSkeleton.draw(viewMain);
        kinectSkeleton.drawControlPanel();
    }else if(musicMaker.isMain()){
        musicMaker.draw(viewMain);
        musicMaker.drawControlPanel();
    }else if(floor.isMain()){
        floor.draw(viewMain);
        floor.drawControlPanel();
    }else if(tv.isMain()){
        tv.draw(viewMain);
        tv.drawControlPanel();
    }
    
    floor.drawProjections();
    tv.drawTV();
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