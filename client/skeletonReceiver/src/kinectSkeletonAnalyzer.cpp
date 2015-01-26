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
    set = false;
    min = FLT_MAX;
    max = FLT_MIN;
}

void kinectSkeletonAnalyzer::analyze( kinectSkeleton & KS){
    
    
    //    ptsHistory.push_back(KS.pts);
    //    if(ptsHistory.size() > 2){
    //        ptsHistory.pop_front();
    //    }
    
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
    
    
    //    cout << armLeftExtendedPct << " " << armRightExtendedPct << endl;
    
    
    
    ofPoint knee[2];
    ofPoint feet[2];
    ofPoint hips[2];
    
    
    spine =  KS.pts[  KS.nameToIndex[ "SpineBase" ]];
    
    for (int i = 0; i < 2; i++){
        hips[i] = KS.pts[  KS.nameToIndex[ (i == 0 ? "HipLeft" : "HipRight")]];
        knee[i] = KS.pts[  KS.nameToIndex[ (i == 0 ? "KneeLeft" : "KneeRight")]];
        feet[i] = KS.pts[  KS.nameToIndex[ (i == 0 ? "FootLeft" : "FootRight")]];
        
    }
    
    
    float feetDist[2];
    float totalFeetDist[2];
    
    for (int i = 0; i < 2; i++){
        
        feetDist[i] =   (spine - feet[i]).length();
        totalDist[i] =  (spine - hips[i]).length() +
        (hips[i] - knee[i]).length() +
        (knee[i] - feet[i]).length();
    }
    
    footLeftExtendedPct = feetDist[0] / totalDist[0];
    footRightExtendedPct = feetDist[1] / totalDist[1];
    
    
    
    float handSpineDist[2];
    float totalHipDist[2];
    
    for (int i = 0; i < 2; i++){
        
        handSpineDist[i] = (hips[i] - hands[i]).length();
        totalHipDist[i] =  (spine - shoulders[i]).length() +
        (shoulders[i] - elbows[i]).length() +
        (elbows[i] - hands[i]).length();
        ;
    }
    
    
    leftHandVHip = handSpineDist[0]/totalDist[0];
    rightHandVHip = handSpineDist[1]/totalDist[1];
    
    if(set){
        diffCenter = (old.pts[KS.nameToIndex["SpineMid"]] - KS.pts[  KS.nameToIndex[ "SpineMid" ]]).length();
        
        max = MAX(max, diffCenter);
        min = MIN(min, diffCenter);
        
        diffCenter = ofMap(diffCenter, min, max, 0, 1, true);
    }
    
    
    old = KS;
    set = true;
}

