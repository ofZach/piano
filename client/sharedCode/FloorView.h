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
class FloorView{
public:
    FloorView();
    ~FloorView();
    
    void setup(ofRectangle projectorViewport, ofRectangle mainViewport=ofGetCurrentViewport());
    void update();
    void draw();
    void drawDebug();
    void drawProjections();
    void setupQuadWarp();
    void setupGUI();
    void setMainView(bool view);
    
    Floor p1Floor;
    Floor p2Floor;
    ofxControlPanel gui;
    
    ofFbo projectionFbo;
    ofRectangle projectionViewport;
    ofRectangle mainViewport;
    ofParameterGroup        squareOptions;
    bool bMainView;

    
    ofParameter <float>     scale;
    ofParameter <float>     horizOffset;
    ofParameter <float>     verticalOffset;
    ofParameter <float>     verticalScaleFactor;
    ofParameter <bool>      bShowDots;
    ofParameter <float>     lightWeight;
    ofParameter <float>     lineDistance;
    ofParameter <float>     speed;
    

    ofxQuadWarp warpOne;
    ofxQuadWarp warpTwo;
    
    ofMatrix4x4 mat, mat2;
    
    ofParameter <bool> bSaveWarp;
};