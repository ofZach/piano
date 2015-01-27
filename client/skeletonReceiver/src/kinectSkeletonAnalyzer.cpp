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
    setV = false;
    minCenter = FLT_MAX;
    maxCenter = FLT_MIN;
    maxFeet = FLT_MIN;
    minFeet = FLT_MAX;
    minMag = FLT_MAX;
    maxMag = FLT_MIN;
    minDistLeft = FLT_MAX;
    maxDistLeft = FLT_MIN;
    minDistRight = FLT_MAX;
    maxDistRight = FLT_MIN;
    dt = ofGetElapsedTimef();
    
    velocity.assign(27, ofPoint());
    oldVelocity.assign(27, ofPoint());
    acceleration.assign(27, ofPoint());
    mag.assign(27, 0);
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-left", 100, false));
    nameToHistoryPlot["arm-left"] = historyPlots.back();
    
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-left-accel", 100, false));
    nameToHistoryPlot["arm-left-accel"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-right", 100, false));
    nameToHistoryPlot["arm-right"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-right-accel", 100, false));
    nameToHistoryPlot["arm-right-accel"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "foot-left", 100, false));
    nameToHistoryPlot["foot-left"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "foot-right", 100, false));
    nameToHistoryPlot["foot-right"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "foot-left", 100, false));
    nameToHistoryPlot["foot-left-accel"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "foot-right", 100, false));
    nameToHistoryPlot["foot-right-accel"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "hand-to-hip-left", 100, false));
    nameToHistoryPlot["hand-to-hip-left"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "hand-to-hip-right", 100, false));
    nameToHistoryPlot["hand-to-hip-right"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Foot To Foot", 100, false));
    nameToHistoryPlot["Foot To Foot"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Center", 100, false));
    nameToHistoryPlot["Center"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Knee Angle Left", 100, false));
    nameToHistoryPlot["Knee Angle Left"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Knee Angle Right", 100, false));
    nameToHistoryPlot["Knee Angle Right"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Angle Left Elbow", 100, false));
    nameToHistoryPlot["angleLeftElbow"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Angle Right Elbow", 100, false));
    nameToHistoryPlot["angleRightElbow"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Right Foot to Ground", 100, false));
    nameToHistoryPlot["rightFootToGround"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Left Foot to Ground", 100, false));
    nameToHistoryPlot["leftFootToGround"] = historyPlots.back();
    
    
    for (auto hp : historyPlots){
        
        hp->setRange(0, 1); //hard range, will not adapt to values off-scale
        //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
        hp->setColor( ofColor(0,255,0) ); //color of the plot line
        hp->setShowNumericalInfo(true);  //show the current value and the scale in the plot
        hp->setRespectBorders(true);	   //dont let the plot draw on top of text
        hp->setLineWidth(1);				//plot line width
        hp->setBackgroundColor(ofColor(0,220)); //custom bg color
        //custom grid setup
        hp->setDrawGrid(true);
        hp->setGridColor(ofColor(30)); //grid lines color
        hp->setGridUnit(14);
        
    }
    nFrames = 30;
}

void kinectSkeletonAnalyzer::analyze( kinectSkeleton & KS){
    dt = dt - ofGetElapsedTimef();
    if(skeletons.size() > 0){
        
        diffCenter = (skeletons.back().pts[KS.nameToIndex["SpineMid"]] - KS.pts[  KS.nameToIndex[ "SpineMid" ]]).length();
        
        maxCenter = MAX(maxCenter, diffCenter);
        minCenter = MIN(minCenter, diffCenter);
        
        diffCenter = ofMap(diffCenter, minCenter, maxCenter, 0, 1, true);
        avgDiffCenter +=diffCenter;
        avgDiffCenter = avgDiffCenter/2.0;
        
        mag.resize(KS.pts.size());
        dir.resize(KS.pts.size());
        velocity.resize((KS.pts.size()));
        acceleration.resize(KS.pts.size());
        for(int i = 0; i < KS.pts.size(); i++){
            dir[i] = (skeletons.back().pts[i] - KS.pts[i]).normalized();
            mag[i] = (skeletons.back().pts[i] - KS.pts[i]).length();
            velocity[i] = (dir[i]*mag[i]);
            acceleration[i] = velocity[i]-velHistory.back()[i];
        }
        
        for(int i = 0; i < KS.bonesList.size(); i++){
            
            for(int j = 0; j < KS.bones[KS.bonesList[i]].size(); j++){
                limbVelocity[KS.bonesList[i]]+=velocity[KS.bones[KS.bonesList[i]][j]];
            }
            limbVelocity[KS.bonesList[i]] = limbVelocity[KS.bonesList[i]]/KS.bones[KS.bonesList[i]].size();
        }
        

        
        
        orientation = KS.pts[KS.nameToIndex["SpineMid"]].crossed(KS.pts[KS.nameToIndex["ShoulderRight"]]+KS.pts[KS.nameToIndex["ShoulderLeft"]]);
        
        angle = KS.pts[KS.nameToIndex["SpineMid"]].angle(KS.pts[KS.nameToIndex["ShoulderRight"]]+KS.pts[KS.nameToIndex["ShoulderLeft"]]);
        
        calculateWingspan();
        calculateStance();
        
        
        nameToHistoryPlot["arm-left"]->update(armLeftExtendedPct);;
        nameToHistoryPlot["foot-left"]->update(legLeftExtendedPct);
        nameToHistoryPlot["hand-to-hip-left"]->update(leftHandVHip);
        nameToHistoryPlot["angleLeftElbow"]->update(angleLeftElbow);
        nameToHistoryPlot["Knee Angle Left"]->update(angleLeftKnee);
        nameToHistoryPlot["leftFootToGround"]->update(distFootLeft);
        
        nameToHistoryPlot["Foot To Foot"]->update((leftHandSpan+rightFootSpan)/2.0);
        nameToHistoryPlot["Center"]->update(diffCenter);
        
        nameToHistoryPlot["arm-right"]->update(armRightExtendedPct);
        nameToHistoryPlot["foot-right"]->update(legRightExtendedPct);
        nameToHistoryPlot["Knee Angle Right"]->update(angleRightKnee);
        nameToHistoryPlot["angleRightElbow"]->update(angleRightElbow);
        nameToHistoryPlot["rightFootToGround"]->update(distFootRight);
        nameToHistoryPlot["hand-to-hip-right"]->update(rightHandVHip);
        
        
        
        
        ofxHistoryPlot * rf = nameToHistoryPlot["foot-left"];
        ofxHistoryPlot * lf = nameToHistoryPlot["foot-right"];
        
        ofxHistoryPlot * ra = nameToHistoryPlot["arm-right"];
        ofxHistoryPlot * la = nameToHistoryPlot["arm-left"];
        
        if (ra->getValues().size() > 1){
            
            float diffr = ra->getValues()[ra->getValues().size()-2] - ra->getValues()[ra->getValues().size()-1];
            float diffl = la->getValues()[la->getValues().size()-2] - la->getValues()[la->getValues().size()-1];
            
            
            // todo: parametize accel scale
            diffl = ofClamp(fabs(diffl) * 35.0, 0, 1);
            diffr = ofClamp(fabs(diffr) * 35.0, 0, 1);
            
            if (nameToHistoryPlot["arm-left-accel"]->getValues().size() > 0){
                float lastVall = nameToHistoryPlot["arm-left-accel"]->getValues().back();
                float lastValr = nameToHistoryPlot["arm-right-accel"]->getValues().back();
                
                // todo: parametize accel smoothing
                
                diffl = 0.1f * diffl + 0.9 *lastVall;
                diffr = 0.1f * diffr + 0.9 *lastValr;
                
            }
            
            
            nameToHistoryPlot["arm-left-accel"]->update(diffl);;
            nameToHistoryPlot["arm-right-accel"]->update(diffr);;
            
        }

        
        if (rf->getValues().size() > 1){
            
            float diffr = rf->getValues()[rf->getValues().size()-2] - rf->getValues()[rf->getValues().size()-1];
            float diffl = lf->getValues()[lf->getValues().size()-2] - lf->getValues()[lf->getValues().size()-1];
            
            
            // todo: parametize accel scale
            diffl = ofClamp(fabs(diffl) * 35.0, 0, 1);
            diffr = ofClamp(fabs(diffr) * 35.0, 0, 1);
            
            if (nameToHistoryPlot["foot-left-accel"]->getValues().size() > 0){
                float lastVall = nameToHistoryPlot["foot-left-accel"]->getValues().back();
                float lastValr = nameToHistoryPlot["foot-right-accel"]->getValues().back();
                
                // todo: parametize accel smoothing
                
                diffl = 0.1f * diffl + 0.9 *lastVall;
                diffr = 0.1f * diffr + 0.9 *lastValr;
                
            }
            
            
            nameToHistoryPlot["foot-left-accel"]->update(diffl);;
            nameToHistoryPlot["foot-right-accel"]->update(diffr);;
            
        }
        
    }
    
    skeletons.push_back(KS);
    
    
    
}

void kinectSkeletonAnalyzer::calculateWingspan(){
    ofPoint elbows[2];
    ofPoint hands[2];
    ofPoint shoulders[2];
    ofPoint hips[2];
    ofPoint spine =  skeletons.back().pts[  skeletons.back().nameToIndex[ "SpineShoulder" ]];
    
    
    for (int i = 0; i < 2; i++){
        elbows[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "ElbowLeft" : "ElbowRight")]];
        shoulders[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "ShoulderLeft" : "ShoulderRight")]];
        hands[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "HandLeft" : "HandRight")]];
        hips[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "HipLeft" : "HipRight")]];
        
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
        
        handtohandDist[i] =   (hands[i%2].normalized() - hands[i%2].normalized()).length();
    }
    
    leftHandSpan = handtohandDist[0];
    rightHandSpan = handtohandDist[1];
    
    //    cout << armLeftExtendedPct << " " << armRightExtendedPct << endl;
    
    float handSpineDist[2];
    float totalHipDist[2];
    
    for (int i = 0; i < 2; i++){
        
        handSpineDist[i] = (hips[i] - hands[i]).length();
        totalHipDist[i] =  (spine - shoulders[i]).length() +
        (shoulders[i] - elbows[i]).length() +
        (elbows[i] - hands[i]).length();
        
    }
    
    
    leftHandVHip = handSpineDist[0]/totalDist[0];
    rightHandVHip = handSpineDist[1]/totalDist[1];
    angleLeftElbow = (hands[0] - elbows[0]).angle(elbows[0]-shoulders[0])/180;
    angleRightElbow = (hands[1] - elbows[1]).angle(elbows[1]-shoulders[1])/180;
    
    
}

void kinectSkeletonAnalyzer::calculateStance(){
    ofPoint knee[2];
    ofPoint feet[2];
    ofPoint hips[2];
    
    
    ofPoint spine =  skeletons.back().pts[  skeletons.back().nameToIndex[ "SpineBase" ]];
    
    for (int i = 0; i < 2; i++){
        hips[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "HipLeft" : "HipRight")]];
        knee[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "KneeLeft" : "KneeRight")]];
        feet[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "FootLeft" : "FootRight")]];
        
    }
    
    
    float feetDist[2];
    float totalFeetDist[2];
    
    for (int i = 0; i < 2; i++){
        
        feetDist[i] =   (spine - feet[i]).length();
        totalFeetDist[i] =  (spine - hips[i]).length() +
        (hips[i] - knee[i]).length() +
        (knee[i] - feet[i]).length();
    }
    
    legLeftExtendedPct = feetDist[0] / totalFeetDist[0];
    legRightExtendedPct = feetDist[1] / totalFeetDist[1];
    
    angleLeftKnee = (feet[0]-knee[0]).angle(knee[0]-hips[0])/180;
    angleRightKnee = (feet[1]-knee[1]).angle(knee[1]-hips[1])/180;
    
    float footToFootDist[2];
    float totalDistFeet[2];
    
    for (int i = 0; i < 2; i++){
        
        footToFootDist[i] =   (feet[0] - feet[1]).length();
        maxFeet = MAX(maxFeet, footToFootDist[i]);
        minFeet = MIN(minFeet, footToFootDist[i]);
    }
    
    
    leftFootSpan = ofMap(footToFootDist[0], minFeet, maxFeet, 0, 1, true);
    rightFootSpan = ofMap(footToFootDist[1], minFeet, maxFeet, 0, 1, true);
    
    distFootLeft = (feet[0] - ofVec3f(feet[0].x, groundPlane.y, feet[0].z)).length();
    distFootRight = (feet[1] - ofVec3f(feet[1].x, groundPlane.y, feet[1].z)).length();
    
    minDistLeft = MIN(minDistLeft, distFootLeft);
    maxDistLeft = MAX(maxDistLeft, distFootLeft);
    
    minDistRight = MIN(minDistRight, distFootRight);
    maxDistRight = MAX(maxDistRight, distFootRight);
    
    distFootLeft = ofMap(distFootLeft, minDistLeft, maxDistLeft, 0, 1);
    distFootRight = ofMap(distFootRight, minDistRight, maxDistRight, 0, 1);
    
}

void kinectSkeletonAnalyzer::calculateShoulderWidth(){
    shouldersWidth = skeletons.back().pts[skeletons.back().nameToIndex["ShoulderLeft"]-skeletons.back().nameToIndex["ShoulderLeft"]].length();
}


void kinectSkeletonAnalyzer::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if(skeletons.size() > 0){
        for(int i = 0; i < velocity.size(); i++){
            if(mag[i] > velocityThresh || velHistory.back()[i].lengthSquared()-velocity[i].lengthSquared() > diffThresh){
                ofSetColor(ofColor::red, ofClamp(mag[i], 0., 255.0));
                ofPushMatrix();
                ofTranslate(velocity[i]+skeletons.back().pts[i]);
                ofDrawSphere(0, 0, 10);
                ofPopMatrix();
                
                ofSetColor(ofColor::blue, ofClamp(acceleration[i].length(), 0., 255.0));
                ofPushMatrix();
                ofTranslate(acceleration[i]+skeletons.back().pts[i]);
                ofDrawSphere(0, 0, 10);
                ofPopMatrix();
            }
        }
        
        
        for(int i = 0; i < skeletons.back().bonesList.size() > 0; i++){
            for(int i = 0; i < skeletons.back().bones.size() > 0; i++){
                ofPoint p;
                for(int j = 0; j < skeletons.back().bones[skeletons.back().bonesList[i]].size(); j++){
                    if(limbVelocity[skeletons.back().bonesList[i]].length() > 10){
                        ofSetColor(ofColor::blue, ofClamp(limbVelocity[skeletons.back().bonesList[i]].length(), 0.0, 255.0));
                        ofPushMatrix();
                        ofTranslate(limbVelocity[skeletons.back().bonesList[i]]+skeletons.back().pts[skeletons.back().bones[skeletons.back().bonesList[i]][j]]);
                        ofDrawSphere(0, 0, 10);
                        ofPopMatrix();
                    }
                }
            }
        }
        
        ofPushMatrix();
        ofSetColor(255, 255, 255);
        ofTranslate(skeletons.back().pts[skeletons.back().nameToIndex["SpineMid"]]);
        ofRotate(0, orientation.x, orientation.y, orientation.z);
        ofDrawAxis(20);
        ofPopMatrix();
    }
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    velHistory.push_back(velocity);
    accHistory.push_back(acceleration);
    if(ptsHistory.size() > nFrames){
        ptsHistory.pop_front();
        velHistory.pop_front();
        accHistory.pop_front();
        skeletons.pop_front();
    }
    
}



void kinectSkeletonAnalyzer::drawDebug(){
    if(skeletons.size() > 0){
        ofPushMatrix();
        ofTranslate(ofVec3f(ofGetWidth()-160, 0, 0));
        nameToHistoryPlot["arm-left"]->draw(0, 0);
        nameToHistoryPlot["arm-left-accel"]->draw(-160, 0);
        nameToHistoryPlot["angleLeftElbow"]->draw(0, 120);
        nameToHistoryPlot["hand-to-hip-left"]->draw(0, 240);
        nameToHistoryPlot["Knee Angle Left"]->draw(0, 360);
        nameToHistoryPlot["foot-left"]->draw(0, 480);
        nameToHistoryPlot["foot-left-accel"]->draw(-160, 480);
        nameToHistoryPlot["leftFootToGround"]->draw(0, 600);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(ofVec3f(ofGetWidth()/2-80.0, 0, 0));
        nameToHistoryPlot["Foot To Foot"]->draw(0, 0);
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(ofVec3f(ofGetWidth()/2-80.0, ofGetHeight()-120.0, 0));
        nameToHistoryPlot["Center"]->draw(0, 0);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(ofVec3f(0, 0, 0));
        nameToHistoryPlot["arm-right"]->draw(0,0);
        nameToHistoryPlot["arm-right-accel"]->draw(160, 0);
        nameToHistoryPlot["angleRightElbow"]->draw(0, 120);
        nameToHistoryPlot["hand-to-hip-right"]->draw(0,240);
        nameToHistoryPlot["Knee Angle Right"]->draw(0, 360);
        nameToHistoryPlot["foot-right"]->draw(0, 480);
        nameToHistoryPlot["foot-right-accel"]->draw(160, 480);
        nameToHistoryPlot["rightFootToGround"]->draw(0, 600);
        ofPopMatrix();
    }
}
