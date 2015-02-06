//
//  tvSkeleton.h
//  skeletonReceiver
//
//  Created by dantheman on 2/5/15.
//
//

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
#include "kinectBody.h"
class lineChaseSkeleton{
public:
    
    void setup();
    void update(kinectBody * kinectBody, ofCamera mainViewCam);
    void draw(ofEventArgs &args);
    void keyPressed(ofKeyEventArgs &key);
    
    void addLineTrace();
    void triggerTriangles();
    
    //Floor Grid vars
    
    ofxControlPanel gui;
    
    ofFbo tv;
    
    vector < ofPoint > pts;
    vector < connection > connections;
    
    map < int, vector < int >  > ptToConnections;
    
    vector < connectionMover > movers;
    vector < triangle > triangles;
    
    kinectBody *KB;
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
    ofEasyCam cam;
    ofxSyphonServer tvTexture;
    
    unsigned long long lastTrigger;
};