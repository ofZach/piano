
#pragma once

#include "kinectSkeleton.h"

class kinectSkeletonAnalyzer {
    
    
public:
    
    
    void setup();
    void analyze( kinectSkeleton & KS);
    
    
    float armLeftExtendedPct;
    float armRightExtendedPct;
    float footLeftExtendedPct;
    float footRightExtendedPct;
    
    float leftHandVHip;
    float rightHandVHip;
    
    deque<vector<ofPoint> > ptsHistory;
    vector<ofPoint> diff;
    
    float diffCenter;
    kinectSkeleton old;
    bool set;
};