//
//  TVView.h
//  tvDebug
//
//  Created by dantheman on 3/16/15.
//
//

#pragma once
#include "ofMain.h"
#include "tvScreen.h"
class TVView {
public:
    TVView();
    ~TVView();
    
    void setup(ofRectangle tv, ofRectangle main);
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
};
