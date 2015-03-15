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
    
    void setup(ofRectangle viewport);
    void update();
    void draw();
    
    void setupQuadWarp();
    
    void setupGUI();
    
    Floor p1Floor;
    Floor p2Floor;
    ofxControlPanel gui;
    
    ofFbo projectionFbo;
    ofRectangle projectionViewport;
    ofParameterGroup        squareOptions;

    
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