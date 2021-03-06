//
//  kinectSkeletonAnalyzer.cpp
//  skeletonReceiver
//
//  Created by zach on 1/26/15.
//
//

#include "kinectSkeletonAnalyzer.h"

void kinectSkeletonAnalyzer::setup()
{
    armLeftExtendedPct = 0;
    armRightExtendedPct = 0;
    set = false;
    setV = false;
    dt = ofGetElapsedTimef();

    velocity.assign(27, ofPoint());
    oldVelocity.assign(27, ofPoint());
    acceleration.assign(27, ofPoint());
    mag.assign(27, 0);

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
    nFrames = 30;

    anlaysisParams.setName("analysis");
    anlaysisParams.add(smoothing.set("smoothing", 0.9, 0, 1));
    anlaysisParams.add(scale.set("scale", 35, 2.0, 60.0));
    anlaysisParams.add(
        twoDSkelCamDistance.set("twoDSkelCamDistance", 1000, 50, 2000));

    historyPlotsFBO.allocate(400, 2000);

    // ofAddListener(gui.guiEvent, this, &ofApp::eventsIn);

    normFbo.allocate(640, 480, GL_RGBA, 4);
    normFbo.begin();
    ofClear(0, 0, 0, 0);
    normFbo.end();
}

void kinectSkeletonAnalyzer::analyze(kinectSkeleton& KS)
{
    // this skeleton recording logic is weird;

    dt = dt - ofGetElapsedTimef();
    if (skeletons.size() > 0) {
        //---------------------------------------------------------------------------------------
        //
        // calculates magnitute and direction of the velocity & acceleration vectors
        //
        //---------------------------------------------------------------------------------------

        mag.resize(KS.pts.size());
        dir.resize(KS.pts.size());
        velocity.resize((KS.pts.size()));
        acceleration.resize(KS.pts.size());
        for (int i = 0; i < KS.pts.size(); i++) {
            dir[i] = (skeletons.back().pts[i] - KS.pts[i]).normalized();
            mag[i] = (skeletons.back().pts[i] - KS.pts[i]).length();
            velocity[i] = (dir[i] * mag[i]);
            acceleration[i] = velocity[i] - velHistory.back()[i];
        }

        for (int i = 0; i < KS.bonesList.size(); i++) {
            for (int j = 0; j < KS.bones[KS.bonesList[i]].size(); j++) {
                limbVelocity[KS.bonesList[i]] += velocity[KS.bones[KS.bonesList[i]][j]];
            }
            limbVelocity[KS.bonesList[i]] = limbVelocity[KS.bonesList[i]] / KS.bones[KS.bonesList[i]].size();
        }

        ofPoint spineDiff = (KS.pts[KS.nameToIndex["SpineMid"]] - KS.pts[KS.nameToIndex["SpineBase"]]);
        ofPoint shoulderDiff = (KS.pts[KS.nameToIndex["ShoulderRight"]] - KS.pts[KS.nameToIndex["ShoulderLeft"]]);
        spineDiff.normalize();
        shoulderDiff.normalize();
        orientation = shoulderDiff.cross(spineDiff);
        centerNode.setPosition(KS.pts[skeletons.back().nameToIndex["SpineMid"]]);
        ofQuaternion q;
        q.makeRotate(ofPoint(0, 0, 1), orientation);
        centerNode.setOrientation(q);

        //        cout << orientation << endl;

        angle = spineDiff.angle(shoulderDiff);

        //---------------------------------------------------------------------------------------
        //
        // update arm values
        //
        //---------------------------------------------------------------------------------------

        calculateWingspan();

        //---------------------------------------------------------------------------------------
        //
        // update leg values
        //
        //---------------------------------------------------------------------------------------

        calculateStance();

        //
        // update History Plots
        //
        nameToHistoryPlot["arm-ext-lt"]->update(armLeftExtendedPct);
        ;
        nameToHistoryPlot["leg-ext-lt"]->update(legLeftExtendedPct);
        nameToHistoryPlot["dist-hand-to-hip-lt"]->update(leftHandVHip);
        nameToHistoryPlot["elbow-angle-lt"]->update(angleLeftElbow);
        nameToHistoryPlot["knee-angle-lt"]->update(angleLeftKnee);
        nameToHistoryPlot["dist-foot-head-lt"]->update(distFootLeft);
        nameToHistoryPlot["angle-hand-to-hand"]->update(angleHandToHand);

        nameToHistoryPlot["dist-foot-to-foot"]->update(
            (leftFootSpan + rightFootSpan) / 2.0);

        nameToHistoryPlot["arm-ext-rt"]->update(armRightExtendedPct);
        nameToHistoryPlot["leg-ext-rt"]->update(legRightExtendedPct);
        nameToHistoryPlot["knee-angle-rt"]->update(angleRightKnee);
        nameToHistoryPlot["elbow-angle-rt"]->update(angleRightElbow);
        nameToHistoryPlot["dist-foot-head-rt"]->update(distFootRight);
        nameToHistoryPlot["dist-hand-to-hip-rt"]->update(rightHandVHip);
        nameToHistoryPlot["dist-hand-to-hand"]->update(
            (leftHandSpan + rightHandSpan) / 2.0);

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

    skeletons.push_back(KS);

    velHistory.push_back(velocity);
    accHistory.push_back(acceleration);

    if (ptsHistory.size() > nFrames) {
        ptsHistory.pop_front();
        velHistory.pop_front();
        accHistory.pop_front();
        skeletons.pop_front();
    }

    // 2d stuff:

    normCam.setPosition(centerNode.getPosition() + ofPoint(0, 0, -twoDSkelCamDistance));
    normCam.lookAt(centerNode, centerNode.getUpDir());
    normCam.draw();

    normFbo.begin();
    ofClear(255, 255, 255, 255);
    ofSetColor(255, 255, 255);
    ofRect(0, 0, 100, 100);

    // normCam.setOrientation( KSA.centerNode.getGlobalOrientation());

    //    //normCam.dolly( mouseX * 10);
    //    //normCam.lookAt(KSA.centerNode);
    //
    //
    normCam.begin();
    KS.draw();
    normCam.end();
    normFbo.end();
    // ofSetColor(255,255,255);
    // normFbo.draw(0,0);

    ks2d.clear();
    for (int i = 0; i < KS.pts.size(); i++) {
        ks2d.push_back(
            normCam.worldToScreen(KS.pts[i], ofRectangle(0, 0, 640, 480)));
    }

    normFbo.begin();

    //    for (int i= 0; i < ks2d.size(); i++){
    //        ofCircle(ks2d[i], 20);
    //    }

    normFbo.end();
}



void kinectSkeletonAnalyzer::draw(bool boundingbox)
{
    // ofPushStyle();

    if (skeletons.size() > 0) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        //        for(int i = 0; i < velocity.size(); i++){
        //            //if(mag[i] > velocityThresh ||
        //            velHistory.back()[i].lengthSquared()-velocity[i].lengthSquared()
        //            > diffThresh){
        //            ofSetColor(ofColor::red, ofClamp(mag[i], 100., 255.0));
        //            ofPushMatrix();
        //            ofTranslate(velocity[i]+skeletons.back().pts[i]);
        //            ofDrawSphere(0, 0, 10);
        //            ofPopMatrix();
        //
        //            ofSetColor(ofColor::blue, ofClamp(acceleration[i].length(),
        //            0., 255.0));
        //            ofPushMatrix();
        //            ofTranslate(acceleration[i]+skeletons.back().pts[i]);
        //            ofDrawSphere(0, 0, 10);
        //            ofPopMatrix();
        //            //}
        //        }
        ofSpherePrimitive primitve(10, 10);
        ofVboMesh mesh = primitve.getMesh();
        //        for (int i = 0; i < velocity.size(); i++){
        //            if (std::find(skeletons.back().skipList.begin(),
        //            skeletons.back().skipList.end(),
        //            skeletons.back().indexToName[i]
        //            )!=skeletons.back().skipList.end()){
        //                continue;
        //            }
        //            ofSetColor(ofColor::red, ofClamp(mag[i], 100., 255.0));
        //            ofPushMatrix();
        //            ofTranslate(skeletons.back().pts[i]+velocity[i]);
        //            mesh.draw();
        //            ofPopMatrix();
        //
        //            ofSetColor(ofColor::blue, ofClamp(acceleration[i].length(),
        //            100., 255.0));
        //            ofPushMatrix();
        //            ofTranslate(skeletons.back().pts[i]+acceleration[i]);
        //            mesh.draw();
        //            ofPopMatrix();
        //        }
        //
        //
        //        for(int i = 0; i < skeletons.back().bonesList.size() > 0; i++){
        //            for(int i = 0; i < skeletons.back().bones.size() > 0; i++){
        //                ofPoint p;
        //                for(int j = 0; j <
        //                skeletons.back().bones[skeletons.back().bonesList[i]].size();
        //                j++){
        //                    //if(limbVelocity[skeletons.back().bonesList[i]].length()
        //                    > 10){
        //                        ofSetColor(ofColor::blue,
        //                        ofClamp(limbVelocity[skeletons.back().bonesList[i]].length(),
        //                        0.0, 255.0));
        //                        ofPushMatrix();
        //                        ofTranslate(limbVelocity[skeletons.back().bonesList[i]]+skeletons.back().pts[skeletons.back().bones[skeletons.back().bonesList[i]][j]]);
        //                        mesh.draw();
        //                        ofPopMatrix();
        //                    //}
        //                }
        //            }
        //        }

        // draw HandToHandVector
        ofSetColor(
            ofFloatColor(ofColor::orange, (leftHandSpan + rightHandSpan) / 2.0));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HandLeft"]],
               skeletons.back().pts[skeletons.back().nameToIndex["HandRight"]]);

        // Draw Left Arm Extended
        ofSetColor(ofFloatColor(ofColor::red, (armLeftExtendedPct)));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HandLeft"]],
               skeletons.back().pts[skeletons.back().nameToIndex["ShoulderLeft"]]);

        // Draw Right Arm Extended
        ofSetColor(ofFloatColor(ofColor::red, (armRightExtendedPct)));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HandRight"]],
               skeletons.back().pts[skeletons.back().nameToIndex["ShoulderRight"]]);

        // Draw Left Arm To Hip
        ofSetColor(ofFloatColor(ofColor::green, leftHandVHip));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HandLeft"]],
               skeletons.back().pts[skeletons.back().nameToIndex["HipLeft"]]);

        // Draw Left Arm To Hip
        ofSetColor(ofFloatColor(ofColor::green, rightHandVHip));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HandRight"]],
               skeletons.back().pts[skeletons.back().nameToIndex["HipRight"]]);

        // Draw Head to Feet
        ofSetColor(ofFloatColor(ofColor::darkBlue, 1.0 - distFootLeft));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["Head"]],
               skeletons.back().pts[skeletons.back().nameToIndex["AnkleLeft"]]);

        // Draw Head to Feet
        ofSetColor(ofFloatColor(ofColor::darkBlue, 1.0 - distFootRight));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["Head"]],
               skeletons.back().pts[skeletons.back().nameToIndex["AnkleRight"]]);

        // Draw Foot to Foot
        ofSetColor(
            ofFloatColor(ofColor::violet, (leftFootSpan + rightFootSpan) / 2.0));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["AnkleRight"]],
               skeletons.back().pts[skeletons.back().nameToIndex["AnkleLeft"]]);

        // Draw Left Hip to Foot
        ofSetColor(ofFloatColor(ofColor::blueViolet, legLeftExtendedPct));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HipLeft"]],
               skeletons.back().pts[skeletons.back().nameToIndex["AnkleLeft"]]);

        // Draw Right Hip to Foot
        ofSetColor(ofFloatColor(ofColor::blueViolet, legRightExtendedPct));
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HipRight"]],
               skeletons.back().pts[skeletons.back().nameToIndex["AnkleRight"]]);

        // Draw Angle Right Knee
        primitve.set(5, 5);
        mesh = primitve.getMesh();
        ofPushMatrix();
        ofSetColor(
            ofFloatColor(ofColor::blue, ofMap(angleRightKnee, 0, 180, 0, 1, true)));
        ofTranslate(
            skeletons.back().pts[skeletons.back().nameToIndex["KneeRight"]]);
        mesh.draw();
        ofPopMatrix();

        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["KneeRight"]],
               skeletons.back().pts[skeletons.back().nameToIndex["AnkleRight"]]);
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HipRight"]],
               skeletons.back().pts[skeletons.back().nameToIndex["KneeRight"]]);

        // Draw Angle Left Knee
        ofPushMatrix();
        ofSetColor(
            ofFloatColor(ofColor::blue, ofMap(angleLeftKnee, 0, 180, 0, 1, true)));
        ofTranslate(skeletons.back().pts[skeletons.back().nameToIndex["KneeLeft"]]);
        mesh.draw();
        ofPopMatrix();

        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["KneeLeft"]],
               skeletons.back().pts[skeletons.back().nameToIndex["AnkleLeft"]]);
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HipLeft"]],
               skeletons.back().pts[skeletons.back().nameToIndex["KneeLeft"]]);

        // Draw Elbow Right Knee
        ofPushMatrix();
        ofSetColor(ofFloatColor(ofColor::blue,
                                ofMap(angleRightElbow, 0, 180, 0, 1, true)));
        ofTranslate(
            skeletons.back().pts[skeletons.back().nameToIndex["ElbowRight"]]);
        mesh.draw();
        ofPopMatrix();

        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HandRight"]],
               skeletons.back().pts[skeletons.back().nameToIndex["ElbowRight"]]);
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["ElbowRight"]],
               skeletons.back().pts[skeletons.back().nameToIndex["ShoulderRight"]]);

        // Draw Elbow Left Knee
        ofPushMatrix();
        ofSetColor(
            ofFloatColor(ofColor::blue, ofMap(angleLeftElbow, 0, 180, 0, 1, true)));
        ofTranslate(
            skeletons.back().pts[skeletons.back().nameToIndex["ElbowLeft"]]);
        mesh.draw();
        ofPopMatrix();

        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["HandLeft"]],
               skeletons.back().pts[skeletons.back().nameToIndex["ElbowLeft"]]);
        ofLine(skeletons.back().pts[skeletons.back().nameToIndex["ElbowLeft"]],
               skeletons.back().pts[skeletons.back().nameToIndex["ShoulderLeft"]]);

        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPushMatrix();
        ofSetColor(255, 255, 255, 200);
        ofTranslate(skeletons.back().pts[skeletons.back().nameToIndex["SpineMid"]]);

        // Draw Bounding Cube
        ofRotate(0, orientation.x, orientation.y, orientation.z);

        //        ofBoxPrimitive p(100,100,100);
        //        p.drawWireframe();
        //
        //        ofDrawAxis(20);
        ofPopMatrix();

        centerNode.draw();
        if (boundingbox) {
            ofSetLineWidth(1);
            ofPushMatrix();
            ofMultMatrix(centerNode.getGlobalTransformMatrix());
            ofBoxPrimitive p(400, 400, 400);
            ofSetColor(255, 255, 255, 50);
            p.drawWireframe();
            ofPopMatrix();
            ofSetLineWidth(2);
            ofSetColor(255);
        }
    }
}

void kinectSkeletonAnalyzer::drawDebug()
{
    historyPlotsFBO.begin();

    ofClear(127, 127, 127, 50);

    ofSetColor(255);
    normFbo.draw(0, 0, 400, 400 * (480.0 / 640.0));
    ofTranslate(0, 400 * (480.0 / 640.0));

    float height = 75;
    int count = 0;

    if (skeletons.size() > 0) {
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
    }

    historyPlotsFBO.end();

    historyPlotsFBO.draw(0, 0);

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
