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
    historyPlots.push_back(new ofxHistoryPlot( NULL, "hand-to-hip-left", 100, false));
    nameToHistoryPlot["hand-to-hip-left"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "hand-to-hip-right", 100, false));
    nameToHistoryPlot["hand-to-hip-right"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Foot To Foot", 100, false));
    nameToHistoryPlot["Foot To Foot"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot( NULL, "Center", 100, false));
    nameToHistoryPlot["Center"] = historyPlots.back();
    

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
    
    nameToHistoryPlot["arm-left"]->update(armLeftExtendedPct);;
    nameToHistoryPlot["arm-right"]->update(armRightExtendedPct);
    
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
    
    
    
    
    
    nameToHistoryPlot["foot-left"]->update(footLeftExtendedPct);
    nameToHistoryPlot["foot-right"]->update(footRightExtendedPct);
    nameToHistoryPlot["hand-to-hip-left"]->update(leftHandVHip);
    nameToHistoryPlot["hand-to-hip-right"]->update(rightHandVHip);
    nameToHistoryPlot["Foot To Foot"]->update((leftHandSpan+rightFootSpan)/2.0);
    nameToHistoryPlot["Center"]->update(diffCenter);
    
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
    
    int count = 0;
    for (auto hp : historyPlots){
        hp->draw((count >= 4 ? 300: 0), 400 + (count%4)*75, 200, 70);
        count++;
    }
    

}
