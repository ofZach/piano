//
//  Floor.h
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#pragma once
#include "ofMain.h"
#include "Grid.h"

class Floor{
public:
    Floor();
    ~Floor();
    void setup();
    void update();
    void draw(float x, float y, float w, float h);
    void drawScene();
    void addLineTrace();
    void triggerTriangles();
    void setParamterGroup(ofParameterGroup* squareOptions);
    float getWidth();
    float getHeight();
    
    //Floor Grid vars
    
    ofParameterGroup *squareOptions;
    ofFbo floor;
    
    vector < ofPoint > pts;
    vector < connection > connections;
    
    map < int, vector < int >  > ptToConnections;
    
    vector < connectionMover > movers;
    vector < triangle > triangles;
    
    
    unsigned long long lastTrigger;
    
    
    map<int, vector<int> > buttonPtToConnections;
    vector<connectionMover> buttonMovers;
    vector<ofPoint> buttonPoints;
    vector<connection> buttonConnections;
    
};