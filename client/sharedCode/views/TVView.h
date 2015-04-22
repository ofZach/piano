//
//  TVView.h
//  tvDebug
//
//  Created by dantheman on 3/16/15.
//
//
#ifndef TVVIEW_H
#define TVVIEW_H
#pragma once
#include "ofMain.h"
#include "tvScreen.h"
#include "ofxGui.h"
class TVView {
public:
    TVView();
    ~TVView();
    
    void setup(int numplayers, ofRectangle tv, ofRectangle main);
    void setupGUI();
    void update(kinectBody *kinectBodyOne, kinectBody *kinectBodyTwo);
    void draw(ofRectangle viewport);
    void drawTV();
    void setStageParameters(ofParameterGroup settings);
    void setMainView(bool main);
    bool isMain();
    
    void setPlayerOneMode(int mode);
    void setPlayerTwoMode(int mode);
    
    void addPulsePlayerOne();
    void addPlusePlayerTwo();
    
    ofParameterGroup    tvParameters;
    
    ofRectangle         mainViewPort;
    ofRectangle         tvViewPort;
    bool                bMainView;
    tvScreen            TV;
    int numplayers;
 
};
#endif