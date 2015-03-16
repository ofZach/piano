//
//  TVView.h
//  tvDebug
//
//  Created by dantheman on 3/16/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxControlPanel.h"
#include "tvScreen.h"
class TVView {
public:
    TVView();
    ~TVView();
    
    void setup(ofRectangle tv, ofRectangle main);
    void setupGUI();
    void update(kinectBody *kinectBodyOne, kinectBody *kinectBodyTwo);
    void draw();
    void drawTV();
    void drawControlPanel();
    
    void setMainView(bool main);
    bool isMain();
    
    ofxControlPanel     gui;
    
    ofParameterGroup    tvParameters;
    ofParameter<float>  cameraRadius;
    ofParameter<float>  cameraAngle;
    ofParameter<float>  cameraHeight;
    ofParameter<bool>   swayCamera;
    ofParameter<float>  swayRate;
    ofParameter<float>  swayAmount;
    ofParameter<bool>   bDrawHairyMan;
    
    ofRectangle         mainViewPort;
    ofRectangle         tvViewPort;
    bool                bMainView;
    tvScreen            TV;
};
