//
//  kinectSkeletonAnalyzer.cpp
//  skeletonReceiver
//
//  Created by zach on 1/26/15.
//
//

#include "kinectSkeletonAnalyzer.h"


void kinectSkeletonAnalyzer::setup(){
    armLeftExtendedPct = 0;
    armRightExtendedPct = 0;
    
    
}
void kinectSkeletonAnalyzer::analyze( kinectSkeleton & KS){
    
    
    ofPoint elbows[2];
    ofPoint hands[2];
    ofPoint shoulders[2];
    
    ofPoint spine =  KS.pts[  KS.nameToIndex[ "SpineShoulder" ]];
    
    
    for (int i = 0; i < 2; i++){
        elbows[i] = KS.pts[  KS.nameToIndex[ (i == 0 ? "ElbowLeft" : "ElbowRight")]];
        shoulders[i] = KS.pts[  KS.nameToIndex[ (i == 0 ? "ShoulderLeft" : "ShoulderRight")]];
        hands[i] = KS.pts[  KS.nameToIndex[ (i == 0 ? "HandLeft" : "HandRight")]];
        
    }
    
    float handDist[2];
    float totalDist[2];
    
    for (int i = 0; i < 2; i++){
        
        handDist[i] =   (spine - hands[i]).length();
        totalDist[i] =  (spine - shoulders[i]).length() +
                        (shoulders[i] - elbows[i]).length() +
                        (elbows[i] - hands[i]).length();
        
    
    }
    
    armLeftExtendedPct = handDist[0] / totalDist[0];
    armRightExtendedPct = handDist[1] / totalDist[1];
    
    
    cout << armLeftExtendedPct << " " << armRightExtendedPct << endl;
    
    
}

