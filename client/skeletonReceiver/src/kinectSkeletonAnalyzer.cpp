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
    dt = ofGetElapsedTimef();
    
    velocity.assign(27, ofPoint());
    oldVelocity.assign(27, ofPoint());
    acceleration.assign(27, ofPoint());
    mag.assign(27, 0);
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-ext-lt", 100, false));
    nameToHistoryPlot["arm-ext-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-diff-lt", 100, false));
    nameToHistoryPlot["arm-diff-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-ext-rt", 100, false));
    nameToHistoryPlot["arm-ext-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "arm-diff-rt", 100, false));
    nameToHistoryPlot["arm-diff-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "leg-ext-lt", 100, false));
    nameToHistoryPlot["leg-ext-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "leg-ext-right", 100, false));
    nameToHistoryPlot["leg-ext-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "leg-diff-lt", 100, false));
    nameToHistoryPlot["leg-diff-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "leg-diff-rt", 100, false));
    nameToHistoryPlot["leg-diff-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "dist-hand-to-hip-lt", 100, false));
    nameToHistoryPlot["dist-hand-to-hip-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "dist-hand-to-hip-rt", 100, false));
    nameToHistoryPlot["dist-hand-to-hip-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "dist-foot-to-foot", 100, false));
    nameToHistoryPlot["dist-foot-to-foot"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "knee-angle-lt", 100, false));
    nameToHistoryPlot["knee-angle-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "knee-angle-rt", 100, false));
    nameToHistoryPlot["knee-angle-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "elbow-angle-lt", 100, false));
    nameToHistoryPlot["elbow-angle-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "elbow-angle-rt", 100, false));
    nameToHistoryPlot["elbow-angle-rt"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "elbow-diff-lt", 100, false));
    nameToHistoryPlot["elbow-diff-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "elbow-diff-rt", 100, false));
    nameToHistoryPlot["elbow-diff-rt"] = historyPlots.back();
    
    historyPlots.push_back(new ofxHistoryPlot( NULL, "dist-foot-head-rt", 100, false));
    nameToHistoryPlot["dist-foot-head-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "dist-foot-head-lt", 100, false));
    nameToHistoryPlot["dist-foot-head-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "dist-hand-to-hand", 100, false));
    nameToHistoryPlot["dist-hand-to-hand"] = historyPlots.back();
    
    
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
    
    nameToHistoryPlot["elbow-angle-lt"]->setRange(0, 180);
    nameToHistoryPlot["elbow-angle-rt"]->setRange(0, 180);
    nameToHistoryPlot["elbow-diff-lt"]->setRange(0, 180);
    nameToHistoryPlot["elbow-diff-rt"]->setRange(0, 180);
    
    nameToHistoryPlot["knee-angle-lt"]->setRange(0, 180);
    nameToHistoryPlot["knee-angle-rt"]->setRange(0, 180);
    nFrames = 30;
    
    anlaysisParams.setName("analysis");
    anlaysisParams.add(smoothing.set("smoothing", 0.9, 0, 1));
    anlaysisParams.add(scale.set("scale", 35, 2.0, 60.0));
    
    
    
    historyPlotsFBO.allocate(400,2000);
}
void kinectSkeletonAnalyzer::analyze( kinectSkeleton & KS){
    
    dt = dt - ofGetElapsedTimef();
    if(skeletons.size() > 0){
        
        //
        // calculates magnitute and direction of the velocity & acceleration vectors
        //
        
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
        
        
        
        ofPoint spineDiff = (KS.pts[KS.nameToIndex["SpineMid"]] - KS.pts[KS.nameToIndex["SpineBase"]]);
        ofPoint shoulderDiff = (KS.pts[KS.nameToIndex["ShoulderRight"]] - KS.pts[KS.nameToIndex["ShoulderLeft"]]);
        spineDiff.normalize();
        shoulderDiff.normalize();
        
        
        orientation = shoulderDiff.cross(spineDiff);
        //        cout << orientation << endl;
        
        
        angle = KS.pts[KS.nameToIndex["SpineMid"]].angle(KS.pts[KS.nameToIndex["ShoulderRight"]]+KS.pts[KS.nameToIndex["ShoulderLeft"]]);
        
        //
        // update arm values
        //
        calculateWingspan();
        //
        // update leg values
        //
        calculateStance();
        
        
        //
        // update History Plots
        //
        nameToHistoryPlot["arm-ext-lt"]->update(armLeftExtendedPct);;
        nameToHistoryPlot["leg-ext-lt"]->update(legLeftExtendedPct);
        nameToHistoryPlot["dist-hand-to-hip-lt"]->update(leftHandVHip);
        nameToHistoryPlot["elbow-angle-lt"]->update(angleLeftElbow);
        nameToHistoryPlot["knee-angle-lt"]->update(angleLeftKnee);
        nameToHistoryPlot["dist-foot-head-lt"]->update(distFootLeft);
        
        nameToHistoryPlot["dist-foot-to-foot"]->update((leftFootSpan+rightFootSpan)/2.0);
        
        nameToHistoryPlot["arm-ext-rt"]->update(armRightExtendedPct);
        nameToHistoryPlot["leg-ext-rt"]->update(legRightExtendedPct);
        nameToHistoryPlot["knee-angle-rt"]->update(angleRightKnee);
        nameToHistoryPlot["elbow-angle-rt"]->update(angleRightElbow);
        nameToHistoryPlot["dist-foot-head-rt"]->update(distFootRight);
        nameToHistoryPlot["dist-hand-to-hip-rt"]->update(rightHandVHip);
        nameToHistoryPlot["dist-hand-to-hand"]->update((leftHandSpan+rightHandSpan)/2.0);
        
        
        //
        // calculate diff for arms
        //
        ofxHistoryPlot * rf = nameToHistoryPlot["leg-ext-lt"];
        ofxHistoryPlot * lf = nameToHistoryPlot["leg-ext-rt"];
        
        ofxHistoryPlot * ra = nameToHistoryPlot["arm-ext-rt"];
        ofxHistoryPlot * la = nameToHistoryPlot["arm-ext-lt"];
        
        if (ra->getValues().size() > 1){
            
            float diffr = ra->getValues()[ra->getValues().size()-2] - ra->getValues()[ra->getValues().size()-1];
            float diffl = la->getValues()[la->getValues().size()-2] - la->getValues()[la->getValues().size()-1];
            
            diffl = ofClamp(fabs(diffl) * scale, 0, 1);
            diffr = ofClamp(fabs(diffr) * scale, 0, 1);
            
            if (nameToHistoryPlot["arm-diff-lt"]->getValues().size() > 0){
                float lastVall = nameToHistoryPlot["arm-diff-lt"]->getValues().back();
                float lastValr = nameToHistoryPlot["arm-diff-rt"]->getValues().back();
                
                diffl = (smoothing) * diffl + (1.0-smoothing)  *lastVall;
                diffr = (smoothing) * diffr + (1.0-smoothing)  *lastValr;
                
            }
            
            
            nameToHistoryPlot["arm-diff-lt"]->update(diffl);;
            nameToHistoryPlot["arm-diff-rt"]->update(diffr);;
            
        }
        
        //
        // calculate diff for legs
        //
        if (rf->getValues().size() > 1){
            
            float diffr = rf->getValues()[rf->getValues().size()-2] - rf->getValues()[rf->getValues().size()-1];
            float diffl = lf->getValues()[lf->getValues().size()-2] - lf->getValues()[lf->getValues().size()-1];
            
            diffl = ofClamp(fabs(diffl) * scale, 0, 1);
            diffr = ofClamp(fabs(diffr) * scale, 0, 1);
            
            if (nameToHistoryPlot["leg-diff-lt"]->getValues().size() > 0){
                float lastVall = nameToHistoryPlot["leg-diff-lt"]->getValues().back();
                float lastValr = nameToHistoryPlot["leg-diff-rt"]->getValues().back();
                
                diffl = (smoothing) * diffl + (1.0-smoothing)  *lastVall;
                diffr = (smoothing) * diffr + (1.0-smoothing)  *lastValr;
            }
            
            
            nameToHistoryPlot["leg-diff-lt"]->update(diffl);;
            nameToHistoryPlot["leg-diff-rt"]->update(diffr);;
            
        }
        
        //
        // calculate diff for elbow angles
        //
        ofxHistoryPlot * eAL = nameToHistoryPlot["elbow-angle-lt"];
        ofxHistoryPlot * eAR = nameToHistoryPlot["elbow-angle-rt"];
        
        
        if (eAL->getValues().size() > 1){
            float diffl = eAL->getValues()[eAL->getValues().size()-2] - eAL->getValues()[eAL->getValues().size()-1];
            float diffr = eAR->getValues()[eAR->getValues().size()-2] - eAR->getValues()[eAR->getValues().size()-1];
            
            
            diffl = ofClamp(fabs(diffl) * scale, 0, 180);
            diffr = ofClamp(fabs(diffr) * scale, 0, 180);
            
            if (nameToHistoryPlot["elbow-diff-lt"]->getValues().size() > 0){
                float lastVall = nameToHistoryPlot["elbow-diff-lt"]->getValues().back();
                float lastValr = nameToHistoryPlot["elbow-diff-rt"]->getValues().back();
                
                diffl = (smoothing) * diffl + (1.0-smoothing) *lastVall;
                diffr = (smoothing) * diffr + (1.0-smoothing) *lastValr;
            }
            
            
            nameToHistoryPlot["elbow-diff-lt"]->update(diffl);
            nameToHistoryPlot["elbow-diff-rt"]->update(diffr);
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
    
    ofPoint spineShoulder =  skeletons.back().pts[  skeletons.back().nameToIndex[ "SpineShoulder" ]];
    ofPoint spineBase =  skeletons.back().pts[  skeletons.back().nameToIndex[ "SpineBase" ]];
    float handtohandDist[2];
    float totalDistHands[2];
    
    for (int i = 0; i < 2; i++){
        
        handtohandDist[i] =   (hands[(i+1)%2] - hands[i]).length();
        totalDistHands[i] =  (hands[i] - elbows[i]).length()+(elbows[i]-shoulders[i]).length()+(shoulders[i]-spineShoulder).length()+(spineShoulder-shoulders[(i+i)%2]).length()+(elbows[(i+1)%2]-shoulders[(i+i)%2]).length()+(hands[(i+1)%2]-hands[(i+i)%2]).length();
    }
    
    leftHandSpan = handtohandDist[0]/totalDistHands[0];
    
    rightHandSpan = handtohandDist[1]/totalDistHands[1];
    
    
    
    float handSpineDist[2];
    float totalHipDist[2];
    
    for (int i = 0; i < 2; i++){
        
        handSpineDist[i] = (hips[i] - hands[i]).length();
        totalHipDist[i] =  (hips[i] - spineBase[i]).length() +
        (spineShoulder - spineBase).length()+
        (spineShoulder - shoulders[i]).length() +
        (shoulders[i] - elbows[i]).length() +
        (elbows[i] - hands[i]).length();
        
    }
    
    
    leftHandVHip = handSpineDist[0]/totalHipDist[0];
    rightHandVHip = handSpineDist[1]/totalHipDist[1];
    angleLeftElbow = (hands[0] - elbows[0]).angle(elbows[0]-shoulders[0]);
    angleRightElbow = (hands[1] - elbows[1]).angle(elbows[1]-shoulders[1]);
}

void kinectSkeletonAnalyzer::calculateStance(){
    ofPoint knee[2];
    ofPoint feet[2];
    ofPoint hips[2];
    ofPoint head = skeletons.back().pts[  skeletons.back().nameToIndex[ "Head" ]];
    
    ofPoint spineBase =  skeletons.back().pts[  skeletons.back().nameToIndex[ "SpineBase" ]];
    ofPoint spineShoulders =  skeletons.back().pts[  skeletons.back().nameToIndex[ "SpineShoulders" ]];
    
    for (int i = 0; i < 2; i++){
        hips[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "HipLeft" : "HipRight")]];
        knee[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "KneeLeft" : "KneeRight")]];
        feet[i] = skeletons.back().pts[  skeletons.back().nameToIndex[ (i == 0 ? "FootLeft" : "AnkleRight")]];
    }
    
    
    float feetDist[2];
    float totalFeetDist[2];
    
    for (int i = 0; i < 2; i++){
        feetDist[i] =   (hips[i] - feet[i]).length();
        totalFeetDist[i] = (hips[i] - knee[i]).length() +
        (knee[i] - feet[i]).length();
    }
    
    legLeftExtendedPct = feetDist[0] / totalFeetDist[0];
    legRightExtendedPct = feetDist[1] / totalFeetDist[1];
    
    angleLeftKnee = (feet[0]-knee[0]).angle(knee[0]-hips[0]);
    angleRightKnee = (feet[1]-knee[1]).angle(knee[1]-hips[1]);
    float footToFootDist[2];
    
    for (int i = 0; i < 2; i++){
        
        footToFootDist[i] =   (feet[0] - feet[1]).length();
        totalFeetDist[i] = (spineBase - hips[i]).length() +
        (hips[i] - knee[i]).length() +
        (knee[i] - feet[i]).length();
    }
    
    
    leftFootSpan = footToFootDist[0]/totalFeetDist[0];
    rightFootSpan = footToFootDist[1]/totalFeetDist[1];
    
    for (int i = 0; i < 2; i++){
        
        footToFootDist[i] =   (feet[i] - head).length();
        totalFeetDist[i] = (head-spineBase).length()+(spineBase - hips[i]).length() +
        (hips[i] - knee[i]).length() +
        (knee[i] - feet[i]).length();
    }
    
    distFootLeft = footToFootDist[0]/totalFeetDist[0];
    distFootRight = footToFootDist[1]/totalFeetDist[1];
}

void kinectSkeletonAnalyzer::calculateShoulderWidth(){
    shouldersWidth = skeletons.back().pts[skeletons.back().nameToIndex["ShoulderLeft"]-skeletons.back().nameToIndex["ShoulderLeft"]].length();
}


void kinectSkeletonAnalyzer::draw(){
    
    ofNode n;
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if(skeletons.size() > 0){
        for(int i = 0; i < velocity.size(); i++){
            //if(mag[i] > velocityThresh || velHistory.back()[i].lengthSquared()-velocity[i].lengthSquared() > diffThresh){
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
            //}
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
        
        
        ofPoint elbows[2];
        ofPoint hands[2];
        ofPoint shoulders[2];
        ofPoint hips[2];
        ofPoint spine =  skeletons.back().pts[  skeletons.back().nameToIndex[ "SpineShoulder" ]];
        
        
        ofSetColor(ofFloatColor(ofColor::orange, (leftHandSpan+rightHandSpan)/2.0));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "HandLeft"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "HandRight"]]);
        
        
        ofSetColor(ofFloatColor(ofColor::red, (armLeftExtendedPct)));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "HandLeft"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "ShoulderLeft"]]);
        
        ofSetColor(ofFloatColor(ofColor::red, (armRightExtendedPct)));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "HandRight"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "ShoulderRight"]]);
        
        
        ofSetColor(ofFloatColor(ofColor::green, leftHandVHip));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "HandLeft"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "HipLeft"]]);
        
        ofSetColor(ofFloatColor(ofColor::green, rightHandVHip));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "HandRight"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "HipRight"]]);
        
        ofSetColor(ofFloatColor(ofColor::blue, distFootLeft));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "Head"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "AnkleLeft"]]);
        
        ofSetColor(ofFloatColor(ofColor::blue, distFootRight));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "Head"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "AnkleRight"]]);
        
        ofSetColor(ofFloatColor(ofColor::yellowGreen, (leftFootSpan+rightFootSpan)/2.0));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "AnkleRight"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "AnkleLeft"]]);
        
        ofSetColor(ofFloatColor(ofColor::yellowGreen, (leftFootSpan+rightFootSpan)/2.0));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "AnkleLeft"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "AnkleRight"]]);
        
        
        ofSetColor(ofFloatColor(ofColor::blueViolet, legLeftExtendedPct));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "HipLeft"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "AnkleLeft"]]);
        
        ofSetColor(ofFloatColor(ofColor::blueViolet, legRightExtendedPct));
        ofLine(skeletons.back().pts[  skeletons.back().nameToIndex[ "HipRight"]], skeletons.back().pts[  skeletons.back().nameToIndex[ "AnkleRight"]]);
        
        
    
        ofPushMatrix();
        ofSetColor(255, 255, 255);
        ofTranslate(skeletons.back().pts[skeletons.back().nameToIndex["SpineMid"]]);
        
        
        ofRotate(0, orientation.x, orientation.y, orientation.z);
        
        //        ofBoxPrimitive p(100,100,100);
        //        p.drawWireframe();
        //
        //        ofDrawAxis(20);
        ofPopMatrix();
        
        n.setPosition( skeletons.back().pts[skeletons.back().nameToIndex["SpineMid"]]);
        ofQuaternion q;
        
        q.makeRotate( ofPoint(0,0,1), orientation);
        n.setOrientation(q);
        n.draw();
        
        ofSetLineWidth(1);
        ofPushMatrix();
        ofMultMatrix(n.getGlobalTransformMatrix());
        ofBoxPrimitive p(400,400,400);
        ofSetColor(255,255,255,50);
        p.drawWireframe();
        ofPopMatrix();
        ofSetLineWidth(2);
        ofSetColor(255);
        
    }
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    //
    // push back to history
    //
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
    
    
    historyPlotsFBO.begin();
    
    float height = 75;
    int count = 0;
    
    ofClear(127,127,127,50);
    
    if(skeletons.size() > 0){
        nameToHistoryPlot["arm-ext-lt"]->draw(200, height * count++, 190, height-5);
        nameToHistoryPlot["arm-diff-lt"]->draw(200, height * count++, 190, height-5);
        nameToHistoryPlot["elbow-angle-lt"]->draw(200, height * count++, 190, height-5);
        nameToHistoryPlot["elbow-diff-lt"]->draw(200, height * count++, 190, height-5);
        nameToHistoryPlot["dist-hand-to-hip-lt"]->draw(200, height * count++, 190, height-5);
        nameToHistoryPlot["knee-angle-lt"]->draw(200, height * count++, 190, height-5);
        nameToHistoryPlot["leg-ext-lt"]->draw(200, height * count++, 190, height-5);
        nameToHistoryPlot["leg-diff-lt"]->draw(200, height * count++, 190, height-5);
        nameToHistoryPlot["dist-foot-head-lt"]->draw(200, height * count++, 190, height-5);
        
        count = 0;
        nameToHistoryPlot["arm-ext-rt"]->draw(0, height * count++, 190, height-5);
        nameToHistoryPlot["arm-diff-rt"]->draw(0, height * count++, 190, height-5);
        nameToHistoryPlot["elbow-angle-rt"]->draw(0, height * count++, 190, height-5);
        nameToHistoryPlot["elbow-diff-rt"]->draw(0, height * count++, 190, height-5);
        nameToHistoryPlot["dist-hand-to-hip-rt"]->draw(0, height * count++, 190, height-5);
        nameToHistoryPlot["knee-angle-rt"]->draw(0, height * count++, 190, height-5);
        nameToHistoryPlot["leg-ext-rt"]->draw(0, height * count++, 190, height-5);
        nameToHistoryPlot["leg-diff-rt"]->draw(0, height * count++, 190, height-5);
        nameToHistoryPlot["dist-foot-head-rt"]->draw(0, height * count++, 190, height-5);
        
        
        
        nameToHistoryPlot["dist-foot-to-foot"]->draw(0, height * count++, 190, height-5);
    }
    
    
    
    
    historyPlotsFBO.end();
    
    historyPlotsFBO.draw(0,0);
    
    
    //    if(skeletons.size() > 0){
    //
    //
    //        ofPushMatrix();
    //        ofTranslate(ofVec3f(ofGetWidth()-160, 0, 0));
    //        nameToHistoryPlot["arm-left"]->draw(0, 0);
    //        nameToHistoryPlot["arm-left-accel"]->draw(-160, 0);
    //        nameToHistoryPlot["angleLeftElbow"]->draw(0, 120);
    //        nameToHistoryPlot["hand-to-hip-left"]->draw(0, 240);
    //        nameToHistoryPlot["Knee Angle Left"]->draw(0, 360);
    //        nameToHistoryPlot["foot-left"]->draw(0, 480);
    //        nameToHistoryPlot["foot-left-accel"]->draw(-160, 480);
    //        nameToHistoryPlot["leftFootToGround"]->draw(0, 600);
    //        ofPopMatrix();
    //
    //        ofPushMatrix();
    //        ofTranslate(ofVec3f(ofGetWidth()/2-80.0, 0, 0));
    //        nameToHistoryPlot["Foot To Foot"]->draw(0, 0);
    //        ofPopMatrix();
    //        ofPushMatrix();
    //        ofTranslate(ofVec3f(ofGetWidth()/2-80.0, ofGetHeight()-120.0, 0));
    //        nameToHistoryPlot["Center"]->draw(0, 0);
    //        ofPopMatrix();
    //
    //        ofPushMatrix();
    //        ofTranslate(ofVec3f(0, 0, 0));
    //        nameToHistoryPlot["arm-right"]->draw(0,0);
    //        nameToHistoryPlot["arm-right-accel"]->draw(160, 0);
    //        nameToHistoryPlot["angleRightElbow"]->draw(0, 120);
    //        nameToHistoryPlot["hand-to-hip-right"]->draw(0,240);
    //        nameToHistoryPlot["Knee Angle Right"]->draw(0, 360);
    //        nameToHistoryPlot["foot-right"]->draw(0, 480);
    //        nameToHistoryPlot["foot-right-accel"]->draw(160, 480);
    //        nameToHistoryPlot["rightFootToGround"]->draw(0, 600);
    //        ofPopMatrix();
    //    }
}
