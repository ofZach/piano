

#include "kinectBody.h"


//
//  body();
//  void addSkeleton( kinectSkeleton & KS);
//


kinectBody::kinectBody(){
    
    nFramesHistory = 50; // ?
    velocity.resize(25);                // ?
    velLen.resize(25);
    accel.resize(25);
    
    historyPlots.push_back(new ofxHistoryPlot(NULL, "arm-ext-lt", 100, false));
    nameToHistoryPlot["arm-ext-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "arm-diff-lt", 100, false));
    nameToHistoryPlot["arm-diff-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "arm-ext-rt", 100, false));
    nameToHistoryPlot["arm-ext-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "arm-diff-rt", 100, false));
    nameToHistoryPlot["arm-diff-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "leg-ext-lt", 100, false));
    nameToHistoryPlot["leg-ext-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "leg-ext-right", 100, false));
    nameToHistoryPlot["leg-ext-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "leg-diff-lt", 100, false));
    nameToHistoryPlot["leg-diff-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "leg-diff-rt", 100, false));
    nameToHistoryPlot["leg-diff-rt"] = historyPlots.back();
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "dist-hand-to-hip-lt", 100, false));
    nameToHistoryPlot["dist-hand-to-hip-lt"] = historyPlots.back();
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "dist-hand-to-hip-rt", 100, false));
    nameToHistoryPlot["dist-hand-to-hip-rt"] = historyPlots.back();
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "dist-foot-to-foot", 100, false));
    nameToHistoryPlot["dist-foot-to-foot"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "knee-angle-lt", 100, false));
    nameToHistoryPlot["knee-angle-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "knee-angle-rt", 100, false));
    nameToHistoryPlot["knee-angle-rt"] = historyPlots.back();
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "elbow-angle-lt", 100, false));
    nameToHistoryPlot["elbow-angle-lt"] = historyPlots.back();
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "elbow-angle-rt", 100, false));
    nameToHistoryPlot["elbow-angle-rt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "elbow-diff-lt", 100, false));
    nameToHistoryPlot["elbow-diff-lt"] = historyPlots.back();
    historyPlots.push_back(new ofxHistoryPlot(NULL, "elbow-diff-rt", 100, false));
    nameToHistoryPlot["elbow-diff-rt"] = historyPlots.back();
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "dist-foot-head-rt", 100, false));
    nameToHistoryPlot["dist-foot-head-rt"] = historyPlots.back();
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "dist-foot-head-lt", 100, false));
    nameToHistoryPlot["dist-foot-head-lt"] = historyPlots.back();
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "dist-hand-to-hand", 100, false));
    nameToHistoryPlot["dist-hand-to-hand"] = historyPlots.back();
    
    historyPlots.push_back(
                           new ofxHistoryPlot(NULL, "angle-hand-to-hand", 100, false));
    nameToHistoryPlot["angle-hand-to-hand"] = historyPlots.back();
    
    for (auto hp : historyPlots) {
        hp->setRange(0, 1); // hard range, will not adapt to values off-scale
        // plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom
        // reference guides
        hp->setColor(ofColor(0, 255, 0)); // color of the plot line
        hp->setShowNumericalInfo(
                                 true); // show the current value and the scale in the plot
        hp->setRespectBorders(true); // dont let the plot draw on top of text
        hp->setLineWidth(1); // plot line width
        hp->setBackgroundColor(ofColor(0, 220)); // custom bg color
        // custom grid setup
        hp->setDrawGrid(true);
        hp->setGridColor(ofColor(30)); // grid lines color
        hp->setGridUnit(14);
    }
    
    nameToHistoryPlot["elbow-angle-lt"]->setRange(0, 180);
    nameToHistoryPlot["elbow-angle-rt"]->setRange(0, 180);
    nameToHistoryPlot["elbow-diff-lt"]->setRange(0, 180);
    nameToHistoryPlot["elbow-diff-rt"]->setRange(0, 180);
    nameToHistoryPlot["angle-hand-to-hand"]->setRange(0, 180);
    nameToHistoryPlot["knee-angle-lt"]->setRange(0, 180);
    nameToHistoryPlot["knee-angle-rt"]->setRange(0, 180);
    
    historyPlotsFBO.allocate(400, 2000);
    normFbo.allocate(400, 400);
    
    gestureFBO.allocate(400, 2000);
    
    smoothing.set(0.9);
    scale.set(35);
    twoDSkelCamDistance.set(1000);
    
    
    drawSkeletonDebug = true;
}


kinectSkeleton & kinectBody::getLastSkeleton(){
    
    if (history.size() > 0){
        return history.back();
    } else {
        ofLogError() << "get last skeleton is empty ??" << endl;
        return;
    }
}


bool kinectBody::addSkeleton( kinectSkeleton & KS){
    
    bool bNewFrame = true;
    
    if (history.size() > 0){
        
        vector < ofPoint > & ptsA = KS.pts;
        vector < ofPoint > & ptsB = history.back().pts;
        
        float diff = 0;
        for (int i = 0; i < ptsA.size(); i++){
            diff += (ptsA[i] - ptsB[i]).lengthSquared();
        }
        if (diff > 0.1*0.1){
            bNewFrame = true;
        } else {
            bNewFrame = false;
        }
    }
    
    if (bNewFrame == false) {
        return false;
    }
    
    // calculate velocity;
    if (history.size() > 0){
        
        // -------------------------------------------------- acc setup
        // if we have 2 points, do ACCEL
        vector < ofPoint > oldVel;
        if (history.size() > 1){
            oldVel = velocity;
        }
        
        // -------------------------------------------------- vel
        vector < ofPoint > & ptsA = KS.pts;
        vector < ofPoint > & ptsB = history.back().pts;
        for (int i = 0; i < ptsA.size(); i++){
            velocity[i] = (ptsA[i] - ptsB[i]);
            velLen[i] = velocity[i].length();
        }
        
        // -------------------------------------------------- acc
        // if we have 2 points, do ACCEL
        if (history.size() > 1){
            for (int i = 0; i < velocity.size(); i++){
                accel[i] = (velocity[i] - oldVel[i]);
            }
        }
    }
    
    // store history
    history.push_back(KS);
    if (history.size() > nFramesHistory){
        history.erase(history.begin());
    }
    
    gestureHistory.push_back(KS.gestures);
    if(gestureHistory.size() > nFramesHistory){
        gestureHistory.erase((gestureHistory.begin()));
    }
    
    return true;
    
}


void kinectBody::update(){
    //
    // update History Plots
    //
    nameToHistoryPlot["arm-ext-lt"]->update(history.back().armLeftExtendedPct);
    ;
    nameToHistoryPlot["leg-ext-lt"]->update(history.back().legLeftExtendedPct);
    nameToHistoryPlot["dist-hand-to-hip-lt"]->update(history.back().leftHandVHip);
    nameToHistoryPlot["elbow-angle-lt"]->update(history.back().angleLeftElbow);
    nameToHistoryPlot["knee-angle-lt"]->update(history.back().angleLeftKnee);
    nameToHistoryPlot["dist-foot-head-lt"]->update(history.back().distFootLeft);
    nameToHistoryPlot["angle-hand-to-hand"]->update(history.back().angleHandToHand);
    
    nameToHistoryPlot["dist-foot-to-foot"]->update(
                                                   (history.back().leftFootSpan + history.back().rightFootSpan) / 2.0);
    
    nameToHistoryPlot["arm-ext-rt"]->update(history.back().armRightExtendedPct);
    nameToHistoryPlot["leg-ext-rt"]->update(history.back().legRightExtendedPct);
    nameToHistoryPlot["knee-angle-rt"]->update(history.back().angleRightKnee);
    nameToHistoryPlot["elbow-angle-rt"]->update(history.back().angleRightElbow);
    nameToHistoryPlot["dist-foot-head-rt"]->update(history.back().distFootRight);
    nameToHistoryPlot["dist-hand-to-hip-rt"]->update(history.back().rightHandVHip);
    nameToHistoryPlot["dist-hand-to-hand"]->update(
                                                   (history.back().leftHandSpan + history.back().rightHandSpan) / 2.0);
    
    
    if(gesturePlots.size() == 0){
        // TO DO REMOVE THIS
        // setup gestureNames from file to map names to gestures
        //
        int count = 0;
        for(map<string, Gesture>::iterator iter = gestureHistory.back().begin(); iter != gestureHistory.back().end(); ++iter){
            gesturePlots.push_back(new ofxHistoryPlot(NULL, iter->first, 100, false));
            count++;
        }
    }else{
        int count = 0;
        for(map<string, Gesture>::iterator iter = gestureHistory.back().begin(); iter != gestureHistory.back().end(); ++iter){
            gesturePlots[count]->update(iter->second.value);
            count++;
        }
    }
    
    
    
    //---------------------------------------------------------------------------------------
    //
    // calculate diff for arms
    //
    //---------------------------------------------------------------------------------------
    
    ofxHistoryPlot* rf = nameToHistoryPlot["leg-ext-lt"];
    ofxHistoryPlot* lf = nameToHistoryPlot["leg-ext-rt"];
    
    ofxHistoryPlot* ra = nameToHistoryPlot["arm-ext-rt"];
    ofxHistoryPlot* la = nameToHistoryPlot["arm-ext-lt"];
    
    if (ra->getValues().size() > 1) {
        float diffr = ra->getValues()[ra->getValues().size() - 2] - ra->getValues()[ra->getValues().size() - 1];
        float diffl = la->getValues()[la->getValues().size() - 2] - la->getValues()[la->getValues().size() - 1];
        
        diffl = ofClamp(fabs(diffl) * scale, 0, 1);
        diffr = ofClamp(fabs(diffr) * scale, 0, 1);
        
        if (nameToHistoryPlot["arm-diff-lt"]->getValues().size() > 0) {
            float lastVall = nameToHistoryPlot["arm-diff-lt"]->getValues().back();
            float lastValr = nameToHistoryPlot["arm-diff-rt"]->getValues().back();
            
            diffl = (smoothing)*diffl + (1.0 - smoothing) * lastVall;
            diffr = (smoothing)*diffr + (1.0 - smoothing) * lastValr;
        }
        
        nameToHistoryPlot["arm-diff-lt"]->update(diffl);
        ;
        nameToHistoryPlot["arm-diff-rt"]->update(diffr);
        ;
    }
    
    //---------------------------------------------------------------------------------------
    //
    // calculate diff for legs
    //
    //---------------------------------------------------------------------------------------
    
    if (rf->getValues().size() > 1) {
        float diffr = rf->getValues()[rf->getValues().size() - 2] - rf->getValues()[rf->getValues().size() - 1];
        float diffl = lf->getValues()[lf->getValues().size() - 2] - lf->getValues()[lf->getValues().size() - 1];
        
        diffl = ofClamp(fabs(diffl) * scale, 0, 1);
        diffr = ofClamp(fabs(diffr) * scale, 0, 1);
        
        if (nameToHistoryPlot["leg-diff-lt"]->getValues().size() > 0) {
            float lastVall = nameToHistoryPlot["leg-diff-lt"]->getValues().back();
            float lastValr = nameToHistoryPlot["leg-diff-rt"]->getValues().back();
            
            diffl = (smoothing)*diffl + (1.0 - smoothing) * lastVall;
            diffr = (smoothing)*diffr + (1.0 - smoothing) * lastValr;
        }
        
        nameToHistoryPlot["leg-diff-lt"]->update(diffl);
        ;
        nameToHistoryPlot["leg-diff-rt"]->update(diffr);
        ;
    }
    
    //---------------------------------------------------------------------------------------
    //
    // calculate diff for elbow angles
    //
    //---------------------------------------------------------------------------------------
    
    ofxHistoryPlot* eAL = nameToHistoryPlot["elbow-angle-lt"];
    ofxHistoryPlot* eAR = nameToHistoryPlot["elbow-angle-rt"];
    
    if (eAL->getValues().size() > 1) {
        float diffl = eAL->getValues()[eAL->getValues().size() - 2] - eAL->getValues()[eAL->getValues().size() - 1];
        float diffr = eAR->getValues()[eAR->getValues().size() - 2] - eAR->getValues()[eAR->getValues().size() - 1];
        
        diffl = ofClamp(fabs(diffl) * scale, 0, 180);
        diffr = ofClamp(fabs(diffr) * scale, 0, 180);
        
        if (nameToHistoryPlot["elbow-diff-lt"]->getValues().size() > 0) {
            float lastVall = nameToHistoryPlot["elbow-diff-lt"]->getValues().back();
            float lastValr = nameToHistoryPlot["elbow-diff-rt"]->getValues().back();
            
            diffl = (smoothing)*diffl + (1.0 - smoothing) * lastVall;
            diffr = (smoothing)*diffr + (1.0 - smoothing) * lastValr;
        }
        
        nameToHistoryPlot["elbow-diff-lt"]->update(diffl);
        nameToHistoryPlot["elbow-diff-rt"]->update(diffr);
    }
    
}



void kinectBody::drawHistory(){
    
    if(history.size() > 0){
        normCam.setPosition(history.back().centerPoint.getPosition() + ofPoint(0, 0, -twoDSkelCamDistance/2.0));
        normCam.lookAt(history.back().centerPoint, history.back().centerPoint.getUpDir());
        //        normCam.draw();
        
        normFbo.begin();
        ofClear(255, 255, 255, 255);
        ofSetColor(255, 255, 255);
        ofRect(0, 0, 100, 100);
        
        // normCam.setOrientation( KSA.centerNode.getGlobalOrientation());
        
        //    //normCam.dolly( mouseX * 10);
        //    //normCam.lookAt(KSA.centerNode);
        //
        //
        normCam.begin(ofRectangle(0, 0, normFbo.getWidth(), normFbo.getHeight()));
        history.back().draw();
        normCam.end();
        normFbo.end();
    }
    historyPlotsFBO.begin();
    ofPushMatrix();
    ofClear(127, 127, 127, 50);
    
    if(history.size() > 0){
        ofSetColor(255);
        normFbo.draw(0, 0);
        
    }
    
    ofTranslate(0, 400 * (400 / 400));
    float height = 75;
    int count = 0;
    
    nameToHistoryPlot["arm-ext-lt"]->draw(200, height * count++, 190,
                                          height - 5);
    nameToHistoryPlot["arm-diff-lt"]->draw(200, height * count++, 190,
                                           height - 5);
    nameToHistoryPlot["elbow-angle-lt"]->draw(200, height * count++, 190,
                                              height - 5);
    nameToHistoryPlot["elbow-diff-lt"]->draw(200, height * count++, 190,
                                             height - 5);
    nameToHistoryPlot["dist-hand-to-hip-lt"]->draw(200, height * count++, 190,
                                                   height - 5);
    nameToHistoryPlot["knee-angle-lt"]->draw(200, height * count++, 190,
                                             height - 5);
    nameToHistoryPlot["leg-ext-lt"]->draw(200, height * count++, 190,
                                          height - 5);
    nameToHistoryPlot["leg-diff-lt"]->draw(200, height * count++, 190,
                                           height - 5);
    nameToHistoryPlot["dist-foot-head-lt"]->draw(200, height * count++, 190,
                                                 height - 5);
    nameToHistoryPlot["dist-hand-to-hand"]->draw(200, height * count++, 190,
                                                 height - 5);
    nameToHistoryPlot["angle-hand-to-hand"]->draw(200, height * count++, 190,
                                                  height - 5);
    count = 0;
    nameToHistoryPlot["arm-ext-rt"]->draw(0, height * count++, 190, height - 5);
    nameToHistoryPlot["arm-diff-rt"]->draw(0, height * count++, 190,
                                           height - 5);
    nameToHistoryPlot["elbow-angle-rt"]->draw(0, height * count++, 190,
                                              height - 5);
    nameToHistoryPlot["elbow-diff-rt"]->draw(0, height * count++, 190,
                                             height - 5);
    nameToHistoryPlot["dist-hand-to-hip-rt"]->draw(0, height * count++, 190,
                                                   height - 5);
    nameToHistoryPlot["knee-angle-rt"]->draw(0, height * count++, 190,
                                             height - 5);
    nameToHistoryPlot["leg-ext-rt"]->draw(0, height * count++, 190, height - 5);
    nameToHistoryPlot["leg-diff-rt"]->draw(0, height * count++, 190,
                                           height - 5);
    nameToHistoryPlot["dist-foot-head-rt"]->draw(0, height * count++, 190,
                                                 height - 5);
    
    nameToHistoryPlot["dist-foot-to-foot"]->draw(0, height * count++, 190,
                                                 height - 5);
    ofPopMatrix();
    historyPlotsFBO.end();
    
    
    
    gestureFBO.begin();
    ofClear(127, 127, 127, 50);
    for(int i = 0; i < gesturePlots.size(); i++){
        gesturePlots[i]->draw(0, height*i, 190, height-5);
    }
    gestureFBO.end();
    
    
    if(drawSkeletonDebug){
        historyPlotsFBO.draw(0, 0);
    }else{
        gestureFBO.draw(0, 0);
    }
}
