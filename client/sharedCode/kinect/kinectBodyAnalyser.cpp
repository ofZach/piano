//
//  kinectBodyAnalyzer.cpp
//  skeletonReceiver
//
//  Created by zach on 2/2/15.
//
//

#include "kinectBodyAnalyser.h"



void kinectBodyAnalyser::analyze ( kinectBody & KB ){
    KB.update();
    kinectSkeleton & skeleton = KB.getLastSkeleton();
    
    vector < int > & leftLeg = SKELETOR::Instance()->bones["leftLeg"].pointIds;
    vector < int > & rightLeg = SKELETOR::Instance()->bones["rightLeg"].pointIds;
    vector < int > & leftArm = SKELETOR::Instance()->bones["leftArm"].pointIds;
    vector < int > & rightArm = SKELETOR::Instance()->bones["rightArm"].pointIds;
    vector < int > & torso = SKELETOR::Instance()->bones["torso"].pointIds;
    
    
    
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