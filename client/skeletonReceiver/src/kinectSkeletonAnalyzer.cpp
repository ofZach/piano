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
    minCenter = FLT_MAX;
    maxCenter = FLT_MIN;
    maxFeet = FLT_MIN;
    minFeet = FLT_MAX;
    minMag = FLT_MAX;
    maxMag = FLT_MIN;
    dt = ofGetElapsedTimef();
    
}
void kinectSkeletonAnalyzer::analyze( kinectSkeleton & KS){
    dt = dt - ofGetElapsedTimef();
    
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
    
    
    
    float handtohandDist[2];
    float totalDistHands[2];
    
    for (int i = 0; i < 2; i++){
        
        handtohandDist[i] =   (hands[0].normalized() - hands[1].normalized()).length();
    }
    
    leftHandSpan = handtohandDist[0];
    rightHandSpan = handtohandDist[1];
    
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
        
        maxCenter = MAX(maxCenter, diffCenter);
        minCenter = MIN(minCenter, diffCenter);
        
        diffCenter = ofMap(diffCenter, minCenter, maxCenter, 0, 1, true);
        
        mag.resize(KS.pts.size());
        dir.resize(KS.pts.size());
        velocity.resize((KS.pts.size()));
        for(int i = 0; i < KS.pts.size(); i++){
            dir[i] = (old.pts[i] - KS.pts[i]).normalized();
            mag[i] = (old.pts[i] - KS.pts[i]).length();
            velocity[i] = (dir[i]*mag[i])+KS.pts[i];
        }
    }
    

    float footToFootDist[2];
    float totalDistFeet[2];
    
    for (int i = 0; i < 2; i++){
        
        footToFootDist[i] =   (feet[0] - feet[1]).length();
        maxFeet = MAX(maxFeet, footToFootDist[i]);
        minFeet = MIN(minFeet, footToFootDist[i]);
    }
    
    

    
    leftFootSpan = ofMap(footToFootDist[0], minFeet, maxFeet, 0, 1, true);
    rightFootSpan = ofMap(footToFootDist[1], minFeet, maxFeet, 0, 1, true);
    
    

    
    
    old = KS;
    set = true;
    

}


void kinectSkeletonAnalyzer::draw(){
    for(int i = 0; i < velocity.size(); i++){
        if(mag[i] > 0){
            ofSetColor(ofColor::green, abs(mag[i]));
        }else{
            ofSetColor(ofColor::orange, abs(mag[i]));
        }
        ofPushMatrix();
        ofTranslate(velocity[i]);
        ofDrawSphere(0, 0, 10);
        ofPopMatrix();
    }
}
