


#pragma once

#include "kinectSkeletonAnalyzerInterpreter.h"
#include "kinectSkeletonAnalyzer.h"
#include "kinectSkeleton.h"

// look at the kinect skeleton analyzer and make some decisions

typedef struct {
    ofPoint eventPt;
    float energy;
} hitEvent;

class kinectSkeletonAnalyzerInterpreter {
public:
    
    ofParameterGroup interpreterParams;
    ofParameter <float> armExtendedThreshold;
    ofParameter <float> armNotExtendedThreshold;
    ofParameter <float> armNotBelowThreshold;
    
    ofParameter <float> velOverThresh;
    ofParameter <float> velBelowThresh;
    ofParameter <float> pelvisBlockoutLen;
    
    
    
    
    
    
    void setup();
    void analyze( kinectSkeletonAnalyzer & KSA, kinectSkeleton & KS);
    
    
    bool bAbleToTrigger[2];
    
    void drawEvents(ofFbo & fbo);

    vector < hitEvent > events;
    
    
    
    
    
};