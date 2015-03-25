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
    
    
    settings.globalSettings.setName("Microsoft Music Box");
    settings.globalSettings.add(settings.numPlayers.set("Number of Players", 2, 1, 2));
    
    settings.globalSettings.add(settings.projectorViewX.set("Projector X Pos", ofGetScreenWidth(), ofGetScreenWidth(), ofGetScreenWidth()*2));
    settings.globalSettings.add(settings.projectorViewY.set("Projector Y Pos", 0, 0, ofGetScreenHeight()));
    settings.globalSettings.add(settings.projectorViewWidth.set("Projector Screen Width In Pixels", 1280, 640, 1920));
    settings.globalSettings.add(settings.projectorViewHeight.set("Projector Screen Height In Pixel", 768, 480, 1080));
    
    settings.globalSettings.add(settings.tvViewX.set("TV X Pos", settings.projectorViewX+settings.projectorViewWidth, ofGetScreenWidth(), ofGetScreenWidth()*3));
    settings.globalSettings.add(settings.tvViewY.set("TV Y Pos", 0, 0, ofGetScreenHeight()));
    settings.globalSettings.add(settings.tvViewWidth.set("TV Screen Width In Pixels", 1920, 1280, 1920));
    settings.globalSettings.add(settings.tvViewHeight.set("TV Screen Height In Pixel", 1080, 720, 1200));
    
    
    
    appGUI.setup(settings.globalSettings);
    appGUI.setSize(350, 250);
    appGUI.setWidthElements(350);
    appGUI.loadFromFile("app-settings.xml");
    
    projectorView = ofRectangle(settings.projectorViewX, settings.projectorViewY, settings.projectorViewWidth, settings.projectorViewHeight);
    tvView = ofRectangle(settings.tvViewX, settings.tvViewY, settings.tvViewWidth, settings.tvViewHeight);
    
    setupViewports();
    iMainView = 4;
    
    numPlayers = settings.numPlayers;
    
    
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->listPorts();
    midiOut->openPort(0);
    
    
    kinectSkeleton.setup(numPlayers, viewMain);
    musicMaker.setup(numPlayers, midiOut, viewMain);
    floor.setup(ofRectangle(ofGetScreenWidth(), 0, 1280, 768), viewMain);
    tv.setup(ofRectangle(ofGetScreenWidth()+1280, 0, 1920, 1080), viewMain);
    
    
    skeletonGUI.setup(kinectSkeleton.skeletonTransform);
    stageGUI.setup(kinectSkeleton.stageParams);
    hiddenSettings.setup(kinectSkeleton.hiddenSettings);
    projectionGUI.setup(floor.squareOptions);
    tvGUI.setup(tv.tvParameters);
    midiGUI.setup(musicMaker.midiGroup);
    
    midiGUI.setSize(250, 400);
    projectionGUI.setSize(250, 400);
    projectionGUI.setWidthElements(250);
    projectionGUI.setPosition(ofGetScreenWidth()-250, 0);
    
    skeletonGUI.setSize(250, 400);
    skeletonGUI.setWidthElements(250);
    skeletonGUI.setPosition(ofGetScreenWidth()-250, 0);
    
    stageGUI.setSize(250, 400);
    stageGUI.setWidthElements(250);
    stageGUI.setPosition(ofGetScreenWidth()-500, 0);
    

    stageGUI.loadFromFile("stage.xml");
    skeletonGUI.loadFromFile("skeleton.xml");
    hiddenSettings.loadFromFile("hiddensettings.xml");
    projectionGUI.loadFromFile("projection.xml");
    tvGUI.loadFromFile("tv.xml");
    midiGUI.loadFromFile("midi.xml");
    
    midiGUI.setWidthElements(250);
    
}
void ViewRenderer::update(){
    
    kinectSkeleton.update();
    floor.update();
    
    if(numPlayers <= 1){
        kinectBody * kb1 = kinectSkeleton.getBody(0);
        musicMaker.update(kb1, NULL);
        tv.update(kb1, NULL);
        if(kb1 != NULL){
            floor.setPlayerOne(true);
        }else{
            floor.setPlayerOne(false);
        }
        
        kinectSkeleton.setButtonPos(floor.getPlayerOneButtonPos(), floor.getPlayerOneButtonPos());
        
    }else{
        kinectBody * kb1 = kinectSkeleton.getBody(0);
        kinectBody * kb2 = kinectSkeleton.getBody(1);
        musicMaker.update(kb1, kb2);
        tv.update(kb1, kb2);
        
        
        kinectSkeleton.setButtonPos(floor.getPlayerOneButtonPos(), floor.getPlayerTwoButtonPos());
        
        
        if (musicMaker.playerOne.getBool("ableToChangeModeManually") == false){
            musicMaker.setPlayerOneMode(kinectSkeleton.getPlayerOneMode());
        }
        
        if (musicMaker.playerTwo.getBool("ableToChangeModeManually") == false){
            musicMaker.setPlayerTwoMode(kinectSkeleton.getPlayerTwoMode());
        }
        
        tv.setPlayerOneMode(musicMaker.playerOne.getInt("Output Mode"));
        tv.setPlayerTwoMode(musicMaker.playerTwo.getInt("Output Mode"));
        
        if(kb1 != NULL){
            floor.setPlayerOne(true);
        }else{
            floor.setPlayerOne(false);
        }
        
        if(kb2 != NULL){
            floor.setPlayerTwo(true);
        }else{
            floor.setPlayerTwo(false);
        }
        

        
    }
    
    tv.setStageParameters(kinectSkeleton.stageParams);
    
    
}

void ViewRenderer::exit(){
    skeletonGUI.saveToFile("skeleton.xml");
    stageGUI.saveToFile("stage.xml");
    projectionGUI.saveToFile("projection.xml");
    tvGUI.saveToFile("tv.xml");
    midiGUI.saveToFile("midi.xml");
    appGUI.saveToFile("app-settings.xml");
    hiddenSettings.saveToFile("hiddensettings.xml");
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
        
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetScreenWidth()-50, ofGetScreenHeight()-50);
    }
    
    if(kinectSkeleton.isMain()){
        kinectSkeleton.draw(viewMain);
        skeletonGUI.draw();
        stageGUI.draw();
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
        stageGUI.draw();
    }
    
    floor.drawProjections();
    tv.drawTV();
    ofDisableAlphaBlending();
    
    if(iMainView == 4){
        ofSetColor(255, 0, 0);
        ofRect(viewMain);
        ofSetColor(0, 255, 0);
        ofRect(projectorView);
        ofSetColor(0, 0, 255);
        ofRect(tvView);
        ofSetColor(255, 255, 255);
        ofLine(viewMain.x, viewMain.y, viewMain.x+viewMain.width, viewMain.y+viewMain.height);
        ofLine(viewMain.x+viewMain.width, viewMain.y, viewMain.x, viewMain.y+viewMain.height);
        
        ofLine(tvView.x, tvView.y, tvView.x+tvView.width, tvView.y+tvView.height);
        ofLine(tvView.x+tvView.width, tvView.y, tvView.x, tvView.y+tvView.height);
        
        ofLine(projectorView.x, projectorView.y, projectorView.x+projectorView.width, projectorView.y+projectorView.height);
        ofLine(projectorView.x+projectorView.width, projectorView.y, projectorView.x, projectorView.y+projectorView.height);
    }
    
}

void ViewRenderer::addLineTracePlayerOne(){
    floor.p1Floor.addLineTrace();
}
void ViewRenderer::triggerTrianglePlayerOne(){
    floor.p1Floor.triggerTriangles();
}
void ViewRenderer::addImpulsePlayerOne(){
    tv.addPulsePlayerOne();
}

void ViewRenderer::addLineTracePlayerTwo(){
    floor.p2Floor.addLineTrace();
}
void ViewRenderer::triggerTrianglePlayerTwo(){
    floor.p2Floor.triggerTriangles();
    
}
void ViewRenderer::addImpulsePlayerTwo(){
    tv.addPlusePlayerTwo();
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