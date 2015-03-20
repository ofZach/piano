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
    setupGUI();
    tvViewPort = tv;
    mainViewPort = main;
    TV.setup(tvViewPort);
    TV.setStagePos(0, 0, 0, 100);
    bMainView = false;
}

void TVView::setStageParameters(ofParameterGroup settings){
    TV.setStageParameters(settings);
}

void TVView::setupGUI(){
    tvParameters.setName("TV Controls");
    TV.addParameters(tvParameters);
}

void TVView::addPulsePlayerOne(){
     TV.addImplusePlayerOne();
}
void TVView::addPlusePlayerTwo(){
    TV.addImplusePlayerTwo();
}

void TVView::update(kinectBody *kinectBodyOne, kinectBody *kinectBodyTwo){
    TV.update(kinectBodyOne, kinectBodyTwo);
}
void TVView::draw(ofRectangle viewport){
    TV.draw(viewport);
}

void TVView::setPlayerOneMode(int mode){
    if(mode == 0){
        TV.bDrawHairyManP1 = true;
    }
    if(mode == 1){
        TV.bDrawHairyManP1 = false;
    }
}

void TVView::setPlayerTwoMode(int mode){
    if(mode == 0){
        TV.bDrawHairyManP2 = true;
    }
    if(mode == 1){
        TV.bDrawHairyManP2 = false;
    }
}

void TVView::drawTV(){
    TV.draw(tvViewPort);
}

void TVView::setMainView(bool main){
    bMainView = main;
    
}
bool TVView::isMain(){
    return bMainView;
}