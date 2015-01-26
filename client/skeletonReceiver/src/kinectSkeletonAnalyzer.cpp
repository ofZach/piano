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
    
    armleft = new ofxHistoryPlot( NULL, "arm-left", 100, false);
    armright = new ofxHistoryPlot( NULL, "arm-right", 100, false);
    footLeft = new ofxHistoryPlot( NULL, "foot-left", 100, false);
    footRight = new ofxHistoryPlot( NULL, "foot-right", 100, false);
    leftHandHip = new ofxHistoryPlot( NULL, "hand-to-hip-left", 100, false);
    rightHandHip = new ofxHistoryPlot( NULL, "hand-to-hip-right", 100, false);
    footToFoot = new ofxHistoryPlot( NULL, "Foot To Foot", 100, false);
    
    armleft->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    armleft->setColor( ofColor(0,255,0) ); //color of the plot line
    armleft->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    armleft->setRespectBorders(true);	   //dont let the plot draw on top of text
    armleft->setLineWidth(1);				//plot line width
    armleft->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    armleft->setDrawGrid(true);
    armleft->setGridColor(ofColor(30)); //grid lines color
    armleft->setGridUnit(14);
    
    
    armright->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    armright->setColor( ofColor(0,255,0) ); //color of the plot line
    armright->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    armright->setRespectBorders(true);	   //dont let the plot draw on top of text
    armright->setLineWidth(1);				//plot line width
    armright->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    armright->setDrawGrid(true);
    armright->setGridColor(ofColor(30)); //grid lines color
    armright->setGridUnit(14);
    
    
    footLeft->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    footLeft->setColor( ofColor(0,255,0) ); //color of the plot line
    footLeft->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    footLeft->setRespectBorders(true);	   //dont let the plot draw on top of text
    footLeft->setLineWidth(1);				//plot line width
    footLeft->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    footLeft->setDrawGrid(true);
    footLeft->setGridColor(ofColor(30)); //grid lines color
    footLeft->setGridUnit(14);
    
    
    
    footRight->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    footRight->setColor( ofColor(0,255,0) ); //color of the plot line
    footRight->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    footRight->setRespectBorders(true);	   //dont let the plot draw on top of text
    footRight->setLineWidth(1);				//plot line width
    footRight->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    footRight->setDrawGrid(true);
    footRight->setGridColor(ofColor(30)); //grid lines color
    footRight->setGridUnit(14);
    
    
    leftHandHip->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    leftHandHip->setColor( ofColor(0,255,0) ); //color of the plot line
    leftHandHip->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    leftHandHip->setRespectBorders(true);	   //dont let the plot draw on top of text
    leftHandHip->setLineWidth(1);				//plot line width
    leftHandHip->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    leftHandHip->setDrawGrid(true);
    leftHandHip->setGridColor(ofColor(30)); //grid lines color
    leftHandHip->setGridUnit(14);
    
    rightHandHip->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    rightHandHip->setColor( ofColor(0,255,0) ); //color of the plot line
    rightHandHip->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    rightHandHip->setRespectBorders(true);	   //dont let the plot draw on top of text
    rightHandHip->setLineWidth(1);				//plot line width
    rightHandHip->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    rightHandHip->setDrawGrid(true);
    rightHandHip->setGridColor(ofColor(30)); //grid lines color
    rightHandHip->setGridUnit(14);
    
    
    center = new ofxHistoryPlot( NULL, "Center", 100, false);
    
    center->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    center->setColor( ofColor(0,255,0) ); //color of the plot line
    center->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    center->setRespectBorders(true);	   //dont let the plot draw on top of text
    center->setLineWidth(1);				//plot line width
    center->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    center->setDrawGrid(true);
    center->setGridColor(ofColor(30)); //grid lines color
    center->setGridUnit(14);
    
    
    footToFoot->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    footToFoot->setColor( ofColor(0,255,0) ); //color of the plot line
    footToFoot->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    footToFoot->setRespectBorders(true);	   //dont let the plot draw on top of text
    footToFoot->setLineWidth(1);				//plot line width
    footToFoot->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    footToFoot->setDrawGrid(true);
    footToFoot->setGridColor(ofColor(30)); //grid lines color
    footToFoot->setGridUnit(14);

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
        }
        setV = true;
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
    
    armright->update(armRightExtendedPct);
    armleft->update(armLeftExtendedPct);
    footLeft->update(footLeftExtendedPct);
    footRight->update(footRightExtendedPct);
    
    leftHandHip->update(leftHandVHip);
    rightHandHip->update(rightHandVHip);
    center->update(diffCenter);
    footToFoot->update((leftHandSpan+rightFootSpan)/2.0);
    
}

void kinectSkeletonAnalyzer::distFeet(){
    
}

void kinectSkeletonAnalyzer::distHands(){
    
}


void kinectSkeletonAnalyzer::draw(){
    if(set && setV){
        for(int i = 0; i < velocity.size(); i++){
            if(mag[i] > 0){
                ofSetColor(ofColor::green, abs(mag[i]));
            }else{
                ofSetColor(ofColor::orange, abs(mag[i]));
            }
            ofPushMatrix();
            ofTranslate(velocity[i]+old.pts[i]);
            ofDrawSphere(0, 0, 10);
            ofPopMatrix();
            
            
            if(acceleration[i].length() > 0){
                ofSetColor(ofColor::green, abs(mag[i]));
            }else{
                ofSetColor(ofColor::orange, abs(mag[i]));
            }
            
            ofPushMatrix();
            ofTranslate(acceleration[i]+old.pts[i]);
            ofDrawSphere(0, 0, 10);
            ofPopMatrix();
        }
    }
}


void kinectSkeletonAnalyzer::drawDebug(){
    armright->draw(0, 400);
    armleft->draw(0, 500);
    footLeft->draw(0, 600);
    footRight->draw(0, 700);
    
    leftHandHip->draw(200, 400);
    rightHandHip->draw(200, 500);
    center->draw(200, 600);
    footToFoot->draw(200, 700);
}
