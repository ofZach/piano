//
//  AppSettings.h
//  sceneView
//
//  Created by dantheman on 3/17/15.
//
//

#pragma once
#include "ofMain.h"

class AppSettings{
public:
    AppSettings();
    ~AppSettings();
    
    ofParameterGroup globalSettings;

    
    ofParameter<float> projectorViewWidth;
    ofParameter<float> tvViewWidth;
    
    ofParameter<float> projectorViewHeight;
    ofParameter<float> tvViewHeight;
    
    ofParameter<float> projectorViewX;
    ofParameter<float> tvViewX;
    
    ofParameter<float> projectorViewY;
    ofParameter<float> tvViewY;
    
    ofParameter<int> numPlayers;
};
