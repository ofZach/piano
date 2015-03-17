//
//  Floor.h
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#pragma once
#include "ofMain.h"
#include "gridInfo.h"
#include "traveler.h"
#include "ofxBlur.h"



class Floor{
public:
    

    Floor();
    ~Floor();
    void setup();
    void update();
    void draw(float x, float y, float w, float h);
    
    void realDraw();
    
    void drawScene();
    void addLineTrace();
    void triggerTriangles();
    float getWidth();
    float getHeight();
    
    void spawnLines();

    ofParameterGroup *squareOptions;
    ofFbo floor;
    gridInfo GI;
    vector <traveler > travelers;
    float lastTrimLength;
    float lastSpeed;
    int idNum;
    void setParamterGroup(ofParameterGroup* g){
        this->squareOptions = g;
    }
    
    float personEnergyLevel;
    
    ofxBlur blur;
    
    vector < float > connectionEnergyLevel;
    
    
    
    
};