//
//  Floor.h
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxSyphon.h"
#include "Grid.h"
#include "ofxControlPanel.h"
class Floor{
public:
    Floor();
    ~Floor();
    void setup();
    void update(ofEventArgs &args);
    void draw(ofEventArgs &args);
    void keyPressed(ofKeyEventArgs &key);

    void addLineTrace();
    void triggerTriangles();
    
    //Floor Grid vars
    
    ofxControlPanel gui;
    
    ofFbo floor;
    
    vector < ofPoint > pts;
    vector < connection > connections;
    
    map < int, vector < int >  > ptToConnections;
    
    vector < connectionMover > movers;
    vector < triangle > triangles;
    
    
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
    
    ofxSyphonServer floorServer;
    
    unsigned long long lastTrigger;
};