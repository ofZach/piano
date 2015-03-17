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
}
void ViewRenderer::setup(){
    viewMain = ofRectangle(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
    setupViewports();
    iMainView = 4;
    
    numPlayers = 2;

    
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->listPorts();
    midiOut->openPort(0);
    
    
    kinectSkeleton.setup(numPlayers, viewMain);
    musicMaker.setup(numPlayers, midiOut, viewMain);
    floor.setup(ofRectangle(ofGetScreenWidth(), 0, 1280, 768), viewMain);
    tv.setup(ofRectangle(ofGetScreenWidth()+1280, 0, 1920, 1080), viewMain);
    
    
    skeletonGUI.setup(kinectSkeleton.skeletonTransform);
    skeletonGUI.add(kinectSkeleton.stageParams);
    projectionGUI.setup(floor.squareOptions);
    tvGUI.setup(tv.tvParameters);
    midiGUI.setup(musicMaker.midiGroup);
    
    midiGUI.setSize(250, 400);
    
    skeletonGUI.loadFromFile("skeleton.xml");
    projectionGUI.loadFromFile("projection.xml");
    tvGUI.loadFromFile("tv.xml");
    midiGUI.loadFromFile("midi.xml");
    
    midiGUI.setWidthElements(250);
    
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
    
    tv.setStageParameters(kinectSkeleton.stageParams);
    
    if(numPlayers == 1){
        
    }else{
        floor.getPlayerOneButtonPos();
        floor.getPlayerTwoButtonPos();
    }
}

void ViewRenderer::exit(){
    skeletonGUI.saveToFile("skeleton.xml");
    projectionGUI.saveToFile("projection.xml");
    tvGUI.saveToFile("tv.xml");
    midiGUI.saveToFile("midi.xml");
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
        skeletonGUI.draw();
    }else if(musicMaker.isMain()){
        musicMaker.draw(viewMain);
        midiGUI.draw();
    }else if(floor.isMain()){
        floor.drawDebug();
        projectionGUI.draw();
    }else if(tv.isMain()){
        tv.draw(viewMain);
        tvGUI.draw();
        skeletonGUI.draw();

    }
    
    floor.drawProjections();
    tv.drawTV();
    ofDisableAlphaBlending();
    
}

void ViewRenderer::triggerFloor(){

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