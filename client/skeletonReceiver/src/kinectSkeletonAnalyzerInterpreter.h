


#pragma once

#include "kinectSkeletonAnalyzerInterpreter.h"
#include "kinectSkeletonAnalyzer.h"
#include "kinectSkeleton.h"

// look at the kinect skeleton analyzer and make some decisions

class kinectSkeletonAnalyzerInterpreter {
public:
    float       armExtendedThreshold;
    float       armNotExtendedThreshold;
    bool        bArmLow;
    ofParameterGroup interpreterParams;
    void setup();
    void analyze( kinectSkeletonAnalyzer & KSA, kinectSkeleton & KS);
    
    
};