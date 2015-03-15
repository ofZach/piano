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
class FloorView{
public:
    FloorView();
    ~FloorView();
    
    void setup();
    void update();
    void draw();
    
    void setupGUI();
    
    Floor floorTexture;
    ofxControlPanel gui;
    //connectionMover  C;
    ofParameterGroup        squareOptions;
    ofParameter <float>     scale;
    ofParameter <float>     horizOffset;
    ofParameter <float>     verticalOffset;
    ofParameter <float>     verticalScaleFactor;
    ofParameter <bool>      bShowDots;
    ofParameter <float>     lightWeight;
    ofParameter <float>     lineDistance;
    ofParameter <float>     speed;
    
};