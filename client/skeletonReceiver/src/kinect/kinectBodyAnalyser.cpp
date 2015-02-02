//
//  kinectBodyAnalyzer.cpp
//  skeletonReceiver
//
//  Created by zach on 2/2/15.
//
//

#include "kinectBodyAnalyser.h"



void kinectBodyAnalyser::analyze ( kinectBody & KB ){

    kinectSkeleton & skeleton = KB.getLastSkeleton();
    
    vector < int > & leftLeg = skeleton.bones["leftLeg"].pointIds;
    vector < int > & rightLeg = skeleton.bones["rightLeg"].pointIds;
    vector < int > & leftArm = skeleton.bones["leftArm"].pointIds;
    vector < int > & rightArm = skeleton.bones["rightArm"].pointIds;
    vector < int > & torso = skeleton.bones["torso"].pointIds;

    ofPoint leftLegVel      = avgVel(leftLeg, KB);
    ofPoint rightLegVel     = avgVel(rightLeg, KB);
    ofPoint leftArmVel      = avgVel(leftArm, KB);
    ofPoint rightArmVel     = avgVel(rightArm, KB);
    ofPoint torsoVel        = avgVel(torso, KB);

    KB.rightLegVel  = rightLegVel.length();
    KB.leftLegVel   = leftLegVel.length();
    KB.leftArmVel = leftArmVel.length();
    KB.rightArmVel = rightArmVel.length();
    KB.torsoAvgVel = torsoVel.length();
   
    ofPoint vel (0,0,0);
    for (int i = 0; i < 25; i++){
        vel += KB.velocity[i];
    }
    vel /= 25.0;
    KB.totalAvgVel = vel.length();
    
    
}


ofPoint kinectBodyAnalyser::avgVel (vector < int > pts, kinectBody & KB){
    ofPoint avgVel = ofPoint(0,0,0);
    for (int i = 0; i < pts.size(); i++){
        avgVel += KB.velocity[ pts[i] ];
    }
    avgVel /= (float)pts.size();
    return avgVel;
}