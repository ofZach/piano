
#pragma once

#include "kinectSkeleton.h"

class kinectSkeletonAnalyzer {
    
    
public:
    
    
    void setup();
    void analyze( kinectSkeleton & KS);
    void draw();
    
    
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
    vector<float> mag;

    float maxMag, minMag;
    
    float dt;
    float diffCenter, minCenter, maxCenter;
    kinectSkeleton old;
    bool set;
};