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
    dt = ofGetElapsedTimef();
    
    velocity.assign(27, ofPoint());
    oldVelocity.assign(27, ofPoint());
    acceleration.assign(27, ofPoint());
    mag.assign(27, 0);
    
    
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-left", 100, false));
    nameToHistoryPlot["arm-left"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-right", 100, false));
    nameToHistoryPlot["arm-right"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "foot-left", 100, false));
    nameToHistoryPlot["foot-left"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "foot-right", 100, false));
    nameToHistoryPlot["foot-right"] = historyPlots.back();
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
        
    }
    
    
    leftHandVHip = handSpineDist[0]/totalDist[0];
    rightHandVHip = handSpineDist[1]/totalDist[1];
    
    if(set){
        diffCenter = (old.pts[KS.nameToIndex["SpineMid"]] - KS.pts[  KS.nameToIndex[ "SpineMid" ]]).length();
        
        maxCenter = MAX(maxCenter, diffCenter);
        minCenter = MIN(minCenter, diffCenter);
        
        diffCenter = ofMap(diffCenter, minCenter, maxCenter, 0, 1, true);
        avgDiffCenter +=diffCenter;
        avgDiffCenter = avgDiffCenter/2.0;
        oldVelocity = velocity;
        
        if(setV){
            mag.resize(KS.pts.size());
            dir.resize(KS.pts.size());
            velocity.resize((KS.pts.size()));
            acceleration.resize(KS.pts.size());
            for(int i = 0; i < KS.pts.size(); i++){
                dir[i] = (old.pts[i] - KS.pts[i]).normalized();
                mag[i] = (old.pts[i] - KS.pts[i]).length();
                velocity[i] = (dir[i]*mag[i]);
                acceleration[i] = velocity[i]-oldVelocity[i];
            }
            
            
            for(int i = 0; i < KS.bonesList.size(); i++){
                
                for(int j = 0; j < KS.bones[KS.bonesList[i]].size(); j++){
                    limbAcceleration[KS.bonesList[i]]+=acceleration[KS.bones[KS.bonesList[i]][j]];
                }
                limbAcceleration[KS.bonesList[i]] = limbAcceleration[KS.bonesList[i]]/KS.bones[KS.bonesList[i]].size();
            }
        }
        setV = true;
    }
    
    
    angleLeftKnee = (feet[0]-knee[0]).angle(knee[0]-hips[0])/180;
    angleRightKnee = (feet[1]-knee[1]).angle(knee[1]-hips[1])/180;
    angleLeftElbow = (hands[0] - elbows[0]).angle(elbows[0]-shoulders[0])/180;
    angleRightElbow = (hands[1] - elbows[1]).angle(elbows[1]-shoulders[1])/180;
    
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
    
    nameToHistoryPlot["arm-left"]->update(armLeftExtendedPct);;
    nameToHistoryPlot["arm-right"]->update(armRightExtendedPct);
    nameToHistoryPlot["foot-left"]->update(footLeftExtendedPct);
    nameToHistoryPlot["foot-right"]->update(footRightExtendedPct);
    nameToHistoryPlot["hand-to-hip-left"]->update(leftHandVHip);
    nameToHistoryPlot["hand-to-hip-right"]->update(rightHandVHip);
    nameToHistoryPlot["Foot To Foot"]->update((leftHandSpan+rightFootSpan)/2.0);
    nameToHistoryPlot["Center"]->update(diffCenter);
    nameToHistoryPlot["Knee Angle Left"]->update(angleLeftKnee);
    nameToHistoryPlot["Knee Angle Right"]->update(angleRightKnee);
    nameToHistoryPlot["angleLeftElbow"]->update(angleLeftElbow);
    nameToHistoryPlot["angleRightElbow"]->update(angleRightElbow);
    
}

void kinectSkeletonAnalyzer::distFeet(){
    
}

void kinectSkeletonAnalyzer::distHands(){
    
}


void kinectSkeletonAnalyzer::draw(){
    if(set && setV){
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for(int i = 0; i < velocity.size(); i++){
            //if(mag[i] > 100){
                ofSetColor(ofColor::red);
                ofPushMatrix();
                ofTranslate(velocity[i]+old.pts[i]);
                ofDrawSphere(0, 0, 10);
                ofPopMatrix();
                
                ofSetColor(ofColor::blue, (acceleration[i].length()));
                ofPushMatrix();
                ofTranslate(acceleration[i]+old.pts[i]);
                ofDrawSphere(0, 0, 10);
                ofPopMatrix();
            //s}
        }
        
        
        for(int i = 0; i < old.bonesList.size() > 0; i++){
            for(int i = 0; i < old.bones.size() > 0; i++){
                ofPoint p;
                for(int j = 0; j < old.bones[old.bonesList[i]].size(); j++){
                    if(limbAcceleration[old.bonesList[i]].length() > 10){
                        ofSetColor(ofColor::blue);
                        ofPushMatrix();
                        ofTranslate(limbAcceleration[old.bonesList[i]]+old.pts[old.bones[old.bonesList[i]][j]]);
                        ofDrawSphere(0, 0, 10);
                        ofPopMatrix();
                    }
                }
            }
        }
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }
    
   
    
}



void kinectSkeletonAnalyzer::drawDebug(){
    
    int count = 0;
    for (auto hp : historyPlots){
        hp->draw((count >= 6 ? 300: 0), 400 + (count%6)*75, 200, 70);
        count++;
    }
    
//    if(diffCenter > avgDiffCenter){
//        ofSetColor(ofColor::red);
//        ofRect(ofGetWidth()/2, ofGetHeight()/2, 10, 10);
//    }
}
