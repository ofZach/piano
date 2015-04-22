//
//  FloorView.h
//  visualsDebug
//
//  Created by dantheman on 3/14/15.
//
//

#ifndef FLOORVIEW_H
#define FLOORVIEW_H
#pragma once
#include "ofMain.h"
#include "Floor.h"
#include "ofxQuadWarp.h"
#include "ofxGui.h"
class FloorView{
public:
    FloorView();
    ~FloorView();
    
    void setup(int numplayers, ofRectangle projectorViewport, ofRectangle mainViewport=ofGetCurrentViewport());
    void update();
    void draw(ofRectangle viewport);
    void drawDebug();
    void drawProjections();
    void setupQuadWarp();
    void setupGUI();
    void setMainView(bool view);
    bool isMain();
    void drawGrid();
    void drawProjectorCalibration();
    void addLineTrace(int playerID);
    void setPlayerOne(bool populated);
    void setPlayerTwo(bool populated);
    
    ofPoint getPlayerOneButtonPos();
    ofPoint getPlayerTwoButtonPos();
    
    int numPlayers;
    
    vector<Floor*> floors;
    
//    ofPoint playerOneButton;
//    ofPoing playerTwoButton;
    
    ofxPanel floorGUI;
    
    ofFbo projectionFbo;
    ofRectangle projectionViewport;
    ofRectangle mainViewport;
    ofParameterGroup        projectionParameters;
	ofParameterGroup		floorControls;
    
	bool bMainView;

    ofParameter <float>     floorOffset;
    ofParameter <bool>      dualWarp;
    
    // Warp Parameters
    ofParameter<bool>       bDrawCalibration;
    ofParameter <bool>      bShowWarp;
    ofParameter <bool>      showWarpOne;
    ofParameter <bool>      showWarpTwo;

    ofTrueTypeFont font;
    
    ofxQuadWarp warpFloor;

//    ofxQuadWarp warpOne;
//    ofxQuadWarp warpTwo;
    
    ofMatrix4x4 mat, mat1, mat2;
    
    ofParameter <bool> bSaveWarp;

};
#endif