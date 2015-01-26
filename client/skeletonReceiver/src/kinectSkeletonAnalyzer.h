
#pragma once

#include "kinectSkeleton.h"
#include "ofxHistoryPlot.h"
class kinectSkeletonAnalyzer {
    
public:
    
    
    void setup();
    void analyze( kinectSkeleton & KS);
    void calculateVelocities();
    void distHands();
    void distFeet();
    void draw();
    void drawDebug();
    
    
    float armLeftExtendedPct;
    float armRightExtendedPct;
    float footLeftExtendedPct;
    float footRightExtendedPct;
    
    float leftFootSpan, rightFootSpan, maxFeet, minFeet;
    float leftHandSpan, rightHandSpan, maxHands, minHands;
    
    float leftHandVHip;
    float rightHandVHip;
    
    deque<vector<ofPoint> > ptsHistory;
    vector<ofPoint> velocity, oldVelocity;
    vector<ofPoint> acceleration;
    vector<ofPoint> dir;
    map<string, ofPoint> limbAcceleration;
    vector<float> mag;

    float maxMag, minMag;
    
    float dt;
    float diffCenter, minCenter, maxCenter;
    kinectSkeleton old;
    bool set;
    bool setV;
    
    ofxHistoryPlot * armleft;
    ofxHistoryPlot * armright;
    ofxHistoryPlot * footLeft;
    ofxHistoryPlot * footRight;
    ofxHistoryPlot * leftHandHip;
    ofxHistoryPlot * rightHandHip;
    ofxHistoryPlot * center;
    ofxHistoryPlot * footToFoot;
    
    
    
};