
#pragma once

#include "kinectSkeleton.h"

class kinectSkeletonAnalyzer {
    
    
public:
    
    
    void setup();
    void analyze( kinectSkeleton & KS);
    
    
    float armLeftExtendedPct;
    float armRightExtendedPct;
    
    
    
    
    
};