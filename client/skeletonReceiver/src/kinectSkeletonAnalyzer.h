
#pragma once

#include "kinectSkeleton.h"
#include "ofxHistoryPlot.h"
#include "ofEvents.h"

// todo:

// (a) is leg on the ground
// (b) is this joint moving "fast" -- maybe a threshold ?
// (c) reset on no skeleton...
// (d) history of each joint for last n frames
// (e) smoothing for vel needed?
// (f) add a gui for thresholds and smoothing vals

// higher level:

// (a) did we just punch
//                          ( did our arm extension go under a threshold then over a threshold ) ?
//                          is it higher then our stomach?
// (b) did we just kick
// (c) did we jump
// (d) did we do a circular movement with our hands

// global:

// (a) how much overall movement is there
// (b) std dev on velocity
// (c) size of non axis aligned bounds (maybe the convex hull 3d of point set)



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

    
    float angleLeftKnee, angleRightKnee;
    float angleLeftElbow, angleRightElbow;
    
    float leftHandVHip;
    float rightHandVHip;
    
    float distFootLeft, distFootRight;
    float maxDistLeft, maxDistRight;
    float minDistLeft, minDistRight;
    
    deque<vector<ofPoint> > ptsHistory;
    vector<ofPoint> velocity, oldVelocity;
    vector<ofPoint> acceleration;
    vector<ofPoint> dir;
    map<string, ofPoint> limbAcceleration;
    vector<float> mag;

    float maxMag, minMag;
    
    ofVec3f groundPlane;
    ofVec3f orientation;
    float angle;
    
    float avgDiffCenter;
    float dt;
    float diffCenter, minCenter, maxCenter;
    kinectSkeleton old;
    bool set;
    bool setV;

    

    vector < ofxHistoryPlot * > historyPlots;
    map < string, ofxHistoryPlot * > nameToHistoryPlot;

    
    ofEvent<int> aboveThreshold;
};