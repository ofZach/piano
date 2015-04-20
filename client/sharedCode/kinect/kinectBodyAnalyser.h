

#ifndef KINECTBODYANALYSER_H
#define KINECTBODYANALYSER_H
#pragma once
#include "kinectBody.h"
#include "kinectSkeletonLayout.h"
class kinectBodyAnalyser {
    
public:
    
    void analyze ( kinectBody & KB );
    
    ofPoint avgVel (vector < int > pts, kinectBody & KB);
    
    
};
#endif