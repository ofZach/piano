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
    void update();
    void draw();
    void drawControlPanel();
    
    void setMainScreen(bool main);
    bool isMain();
    
    
    ofRectangle mainViewPort;
    ofRectangle tvViewPort;
    bool bMainView;
    tvScreen TV;
};
