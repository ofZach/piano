//
//  FloorView.h
//  visualsDebug
//
//  Created by dantheman on 3/14/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxControlPanel.h"
#include "Floor.h"
#include "ofxQuadWarp.h"
#include "ofxCalibrate.h"
class FloorView{
public:
    FloorView();
    ~FloorView();
    
    void setup(ofRectangle projectorViewport, ofRectangle mainViewport=ofGetCurrentViewport());
    void update();
    void draw();
    void drawControlPanel();
    void drawProjections();
    void setupQuadWarp();
    void setupGUI();
    void setMainView(bool view);
    bool isMain();
    
    
    void drawStageCalibration();
    void drawProjectorCalibration();
    
    Floor p1Floor;
    Floor p2Floor;
    ofxControlPanel gui;
    
    ofFbo projectionFbo;
    ofRectangle projectionViewport;
    ofRectangle mainViewport;
    ofParameterGroup        squareOptions;
    bool bMainView;

    
    ofParameter <bool>      bShowDots;
    ofParameter <bool>      bShowGrid;
    ofParameter <bool>      bShowButton;
    
    
    
    ofParameter <int> buttonPos0;
    ofParameter <int> buttonPos1;
    
    
    ofParameter <float>     lightWeight;
    ofParameter <float>     lineDistance;
    ofParameter <float>     lineSpeed;  //lastSpeed
    
    ofParameter <bool>      bFadeLines;
    ofParameter <bool>      spawnLines0;
    ofParameter <bool>      spawnLines1;
    
    ofParameter <float>     speed;
    ofParameter <float>     floorOffset;
    ofParameter <bool>      dualWarp;
    
    ofParameter<bool>       bDrawCalibration;
    ofParameter <bool>      bShowWarp;
    ofParameter <bool>      showWarpOne;
    ofParameter <bool>      showWarpTwo;


    
    ofxQuadWarp warpFloor;

//    ofxQuadWarp warpOne;
//    ofxQuadWarp warpTwo;
    
    ofMatrix4x4 mat, mat1, mat2;
    
    ofParameter <bool> bSaveWarp;
};