//
//  kinectSkeletonAnalyser.h
//  skeletonReceiver
//
//  Created by dantheman on 2/2/15.
//
//
#ifndef KINECTSKELETONANALYSER_H
#define KINECTSKELETONANALYSER_H
#pragma once
#include "ofMain.h"
#include "kinectskeleton.h"
class kinectSkeletonAnalyser{
public:
    ofCamera normCam;
    void analyze(kinectSkeleton & skeleton);
};
#endif