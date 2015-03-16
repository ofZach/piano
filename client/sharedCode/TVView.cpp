//
//  TVView.cpp
//  tvDebug
//
//  Created by dantheman on 3/16/15.
//
//

#include "TVView.h"

TVView::TVView(){
    
}

TVView::~TVView(){
    
}


void TVView::setup(ofRectangle tv, ofRectangle main){
    tvViewPort = tv;
    mainViewPort = main;
    setupGUI();
    TV.setup(tvViewPort, &tvParameters);
    TV.setStagePos(0, 0, 0, 100);
    bMainView = false;
}

void TVView::setupGUI(){
    tvParameters.setName("TV Controls");
    tvParameters.add(cameraAngle.set("cameraAngle", 0, 0, TWO_PI));
    tvParameters.add(cameraRadius.set("cameraRadius", 0, 0, 1000));
    tvParameters.add(cameraHeight.set("cameraHeight", 0, 0, 1000));
    tvParameters.add(swayCamera.set("swayCamera", true));
    tvParameters.add(swayRate.set("swayRate", 0.1, 0.01, 10));
    tvParameters.add(swayAmount.set("swayAmount", 0.1, 0, 1));
    tvParameters.add(bDrawHairyMan.set("bDrawHairyMan", false));
    
    gui.setup("TV Controls", 0, mainViewPort.height - 400, 300, 400, true, false);
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addGroup(tvParameters);
    gui.loadSettings("tv-settings.xml");
}

void TVView::update(kinectBody *kinectBodyOne, kinectBody *kinectBodyTwo){
    gui.update();
    TV.update(kinectBodyOne, kinectBodyTwo);
}
void TVView::draw(){
    TV.draw(mainViewPort);
}

void TVView::drawTV(){
    TV.draw(tvViewPort);
}
void TVView::drawControlPanel(){
    gui.draw();
}

void TVView::setMainView(bool main){
    bMainView = main;
    
}
bool TVView::isMain(){
    return bMainView;
}