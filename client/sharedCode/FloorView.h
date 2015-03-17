//
//  FloorView.h
//  visualsDebug
//
//  Created by dantheman on 3/14/15.
//
//

#pragma once
#include "ofMain.h"
#include "Floor.h"
#include "ofxQuadWarp.h"
class FloorView{
public:
    FloorView();
    ~FloorView();
    
    void setup(ofRectangle projectorViewport, ofRectangle mainViewport=ofGetCurrentViewport());
    void update();
    void draw(ofRectangle viewport);
    void drawDebug();
    void drawProjections();
    void setupQuadWarp();
    void setupGUI();
    void setMainView(bool view);
    bool isMain();
    void drawStageCalibration();
    void drawProjectorCalibration();
    
    ofPoint getPlayerOneButtonPos();
    ofPoint getPlayerTwoButtonPos();
    
    Floor p1Floor;
    Floor p2Floor;
    
//    ofPoint playerOneButton;
//    ofPoing playerTwoButton;
    
    ofFbo projectionFbo;
    ofRectangle projectionViewport;
    ofRectangle mainViewport;
    ofParameterGroup        squareOptions;
    bool bMainView;

    ofParameter <float>     floorOffset;
    ofParameter <bool>      dualWarp;
    
    // Warp Parameters
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