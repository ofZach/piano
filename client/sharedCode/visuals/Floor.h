//
//  Floor.h
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#ifndef FLOOR_H
#define FLOOR_H
#pragma once
#include "ofMain.h"
#include "gridInfo.h"
#include "traveler.h"
#include "ofxBlur.h"



class Floor{
public:
    
    
    Floor();
    ~Floor();
    void setup(int i);
    void update();
    void draw(float x, float y, float w, float h);
    
    void drawScene();
    void addLineTrace();
    void triggerTriangles();
    float getWidth();
    float getHeight();
    ofVec3f getButtonPos();
    void spawnLines();
    
    ofParameterGroup squareOptions;
    
    ofParameter <float>     lineWeight;
    ofParameter <float>     trimLength;
    ofParameter <bool>     bPersonPresent;
    ofParameter <float>     lineSpeed;  //lastSpeed
    ofParameter <bool>      bFadeLines;
    ofParameter <bool>      bSpawnLines;
    ofParameter <float>     speed;
    ofParameter <bool>      bShowButton;
    ofParameter <int>       buttonPos;
    ofParameter <bool>      bShowDots;
    ofParameter <bool>      bShowGrid;
    
    
    ofPoint buttonPoint;

    ofFbo floor;
    gridInfo GI;
    vector <traveler > travelers;
    float lastTrimLength;
    float lastSpeed;
    int idNum;
    
    
    float personEnergyLevel;
    
    ofxBlur blur;
    vector < float > connectionEnergyLevel;
};
#endif