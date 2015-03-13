#pragma once

#include "ofMain.h"
#include "ofxKinectV2OSC.h"
#include "kinectSkeletonLayout.h"


class kinectSkeleton {
    
public:
    
    
    
    ofNode centerPoint;
    
    
    vector < ofPoint > pts;
    vector < ofPoint > pts2d;           // normalized 2d points for this body
    
    vector < int > trackingStates;
    

    map<string, Gesture> gestures;
    
    float shouldersWidth;
    float armLeftExtendedPct;
    float armRightExtendedPct;
    float legLeftExtendedPct;
    float legRightExtendedPct;
    float leftFootSpan, rightFootSpan;
    float leftHandSpan, rightHandSpan;
    float angleLeftKnee, angleRightKnee;
    float angleLeftElbow, angleRightElbow;
    float angleHandToHand;
    float leftHandVHip;
    float rightHandVHip;
    float distFootLeft, distFootRight;
    float skeletonHeight;
    
    ofVec3f orientation;
    

    void setup(){
        
        
        SKELETOR::Instance()->setup();
        
        for (int i = 0; i < 25; i++){
            pts.push_back(ofPoint(0,0,0));
            trackingStates.push_back(0);
            pts2d.push_back(ofPoint(0,0,0));
        }
        
    }
    
    ofPoint getCenterPoint ( int name){
        return getPoint(name, ::center);
    }
    
    ofPoint getLeftPoint ( int name ){
        return getPoint(name, ::left);
    }
    
    ofPoint getRightPoint ( int name){
        return getPoint(name, ::right);
    }
    
    ofPoint getPoint(int name, int side){
        if (side == center){
            if (SKELETOR::Instance()->centerEnumsToIndex.find(name) != SKELETOR::Instance()->centerEnumsToIndex.end()){
                return pts[SKELETOR::Instance()->centerEnumsToIndex[name]];
            }
        } else if (side == ::left){
            if (SKELETOR::Instance()->leftEnumsToIndex.find(name) != SKELETOR::Instance()->leftEnumsToIndex.end()){
                return pts[SKELETOR::Instance()->leftEnumsToIndex[name]];
            }
        } else if (side == ::right){
            if (SKELETOR::Instance()->rightEnumsToIndex.find(name) != SKELETOR::Instance()->rightEnumsToIndex.end()){
                return pts[SKELETOR::Instance()->rightEnumsToIndex[name]];
            }
        }
        return ofPoint(0,0,0);
    }
    
    
    
    
    
    void setFromSkeleton( Skeleton & sk, ofMatrix4x4 transform = ofMatrix4x4()){
        pts[SKELETOR::Instance()->nameToIndex["ThumbRight"]].set(sk.getThumbRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["SpineBase"]].set(sk.getSpineBase().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["SpineMid"]].set(sk.getSpineMid().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["Neck"]].set(sk.getNeck().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["Head"]].set(sk.getHead().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["ShoulderLeft"]].set(sk.getShoulderLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["ElbowLeft"]].set(sk.getElbowLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["WristLeft"]].set(sk.getWristLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["HandLeft"]].set(sk.getHandLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["ShoulderRight"]].set(sk.getShoulderRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["ElbowRight"]].set(sk.getElbowRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["WristRight"]].set(sk.getWristRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["HandRight"]].set(sk.getHandRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["HipLeft"]].set(sk.getHipLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["KneeLeft"]].set(sk.getKneeLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["AnkleLeft"]].set(sk.getAnkleLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["FootLeft"]].set(sk.getFootLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["HipRight"]].set(sk.getHipRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["KneeRight"]].set(sk.getKneeRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["AnkleRight"]].set(sk.getAnkleRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["FootRight"]].set(sk.getFootRight().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["SpineShoulder"]].set(sk.getSpineShoulder().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["HandTipLeft"]].set(sk.getHandTipLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["ThumbLeft"]].set(sk.getThumbLeft().getPoint() * transform);
        pts[SKELETOR::Instance()->nameToIndex["HandTipRight"]].set(sk.getHandTipRight().getPoint() * transform);
        
        
        trackingStates[SKELETOR::Instance()->nameToIndex["ThumbRight"]] = sk.getThumbRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["SpineBase"]] = sk.getSpineBase().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["SpineMid"]] = sk.getSpineMid().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["Neck"]] = sk.getNeck().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["Head"]] = sk.getHead().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["ShoulderLeft"]] = sk.getShoulderLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["ElbowLeft"]] = sk.getElbowLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["WristLeft"]] = sk.getWristLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["HandLeft"]] = sk.getHandLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["ShoulderRight"]] = sk.getShoulderRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["ElbowRight"]] = sk.getElbowRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["WristRight"]] = sk.getWristRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["HandRight"]] = sk.getHandRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["HipLeft"]] = sk.getHipLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["KneeLeft"]] = sk.getKneeLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["AnkleLeft"]] = sk.getAnkleLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["FootLeft"]] = sk.getFootLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["HipRight"]] = sk.getHipRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["KneeRight"]] = sk.getKneeRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["AnkleRight"]] = sk.getAnkleRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["FootRight"]] = sk.getFootRight().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["SpineShoulder"]] = sk.getSpineShoulder().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["HandTipLeft"]] = sk.getHandTipLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["ThumbLeft"]] = sk.getThumbLeft().getTrackingState();
        trackingStates[SKELETOR::Instance()->nameToIndex["HandTipRight"]] = sk.getHandTipRight().getTrackingState();
        
        
        gestures = sk.gestures;
    }
    
    void draw(){
        ofPushStyle();
        ofSpherePrimitive primitve(10,10);
        ofVboMesh mesh = primitve.getMesh();
        for (int i = 0; i < pts.size(); i++){
            
            if (std::find(SKELETOR::Instance()->skipList.begin(), SKELETOR::Instance()->skipList.end(), SKELETOR::Instance()->indexToName[i] )!= SKELETOR::Instance()->skipList.end()){
                continue;
            }

            if (trackingStates[i] == TRACKED) ofSetColor(255,255,255, 255);
            else if (trackingStates[i] == INFERRED) ofSetColor(127,127,127, 255);

            else ofSetColor(255,0,0, 100);
            ofPushMatrix();
            
            ofTranslate(pts[i]);
            mesh.draw();
            ofPopMatrix();
        }
        
        ofSetColor(255,255,255,255);
        drawBones();
        ofPopStyle();
    }
    
    void drawDebug(bool boundingbox){
        
        ofPushStyle();
        ofEnableAlphaBlending();
        float alpha = 1.0;
        
        // draw HandToHandVector
        ofSetColor(
                   ofFloatColor(ofColor::orange, ((leftHandSpan + rightHandSpan) / 2.0) * alpha));
        ofLine(getLeftPoint(hand),
               getRightPoint(hand));
        
        // Draw Left Arm Extended
        ofSetColor(ofFloatColor(ofColor::red, (armLeftExtendedPct*alpha)));
        ofLine(getLeftPoint(hand),
               getLeftPoint(shoulder));
        
        // Draw Right Arm Extended
        ofSetColor(ofFloatColor(ofColor::red, (armRightExtendedPct*alpha)));
        ofLine(getRightPoint(hand),
               getRightPoint(shoulder));
        
        // Draw Left Arm To Hip
        ofSetColor(ofFloatColor(ofColor::darkorange, leftHandVHip*alpha));
        ofLine(getLeftPoint(hand),
               getLeftPoint(hip));
        
        // Draw Left Arm To Hip
        ofSetColor(ofFloatColor(ofColor::darkorange, rightHandVHip*alpha));
        ofLine(getRightPoint(hand),
               getRightPoint(hip));
        
        // Draw Head to Feet
        ofSetColor(ofFloatColor(ofColor::darkRed, (1.0 - distFootLeft)*alpha));
        ofLine(getCenterPoint(head),
               getLeftPoint(ankle));
        
        // Draw Head to Feet
        ofSetColor(ofFloatColor(ofColor::darkRed, (1.0 - distFootRight)*alpha));
        ofLine(getCenterPoint(head),
               getRightPoint(ankle));
        
        // Draw Foot to Foot
        ofSetColor(
                   ofFloatColor(ofColor::violet, ((leftFootSpan + rightFootSpan) / 2.0)*alpha));
        ofLine(getLeftPoint(ankle),
               getRightPoint(ankle));
        
        // Draw Left Hip to Foot
        ofSetColor(ofFloatColor(ofColor::blueViolet, legLeftExtendedPct*alpha));
        ofLine(getLeftPoint(hip),
               getLeftPoint(ankle));
        
        // Draw Right Hip to Foot
        ofSetColor(ofFloatColor(ofColor::blueViolet, legRightExtendedPct*alpha));
        ofLine(getRightPoint(hip),
               getRightPoint(ankle));
        
        // Draw Angle Right Knee
        ofSpherePrimitive primitve(5, 5);
        ofVboMesh mesh = primitve.getMesh();
        ofPushMatrix();
        ofSetColor(
                   ofFloatColor(ofColor::orangeRed, ofMap(angleRightKnee, 0, 180, 0, 1, true)*alpha));
        ofTranslate(
                    getRightPoint(knee));
        mesh.draw();
        ofPopMatrix();
        
        ofLine(getRightPoint(hip),
               getRightPoint(knee));
        ofLine(getRightPoint(ankle),
               getRightPoint(knee));
        
        // Draw Angle Left Knee
        ofPushMatrix();
        ofSetColor(
                   ofFloatColor(ofColor::orangeRed, ofMap(angleLeftKnee, 0, 180, 0, 1, true)*alpha));
        ofTranslate(
                    getLeftPoint(knee));
        mesh.draw();
        ofPopMatrix();
        
        ofLine(getLeftPoint(hip),
               getLeftPoint(knee));
        ofLine(getLeftPoint(ankle),
               getLeftPoint(knee));
        
        // Draw Elbow Left Elbow
        ofPushMatrix();
        ofSetColor(ofFloatColor(ofColor::orangeRed,
                                ofMap(angleRightElbow, 0, 180, 0, 1, true)*alpha));
        ofTranslate(
                    getLeftPoint(elbow));
        mesh.draw();
        ofPopMatrix();
        
        ofLine(getLeftPoint(hand),
               getLeftPoint(elbow));
        ofLine(getLeftPoint(elbow),
               getLeftPoint(shoulder));
        
        // Draw Elbow Right Elbow
        ofPushMatrix();
        ofSetColor(ofFloatColor(ofColor::magenta,
                                ofMap(angleRightElbow, 0, 180, 0, 1, true)*alpha));
        ofTranslate(
                    getRightPoint(elbow));
        mesh.draw();
        ofPopMatrix();
        
        ofLine(getRightPoint(hand),
               getRightPoint(elbow));
        ofLine(getRightPoint(elbow),
               getRightPoint(shoulder));
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPushMatrix();
        ofSetColor(255, 255, 255, 200);
        ofTranslate(getCenterPoint(spineMid));
        
//        // Draw Bounding Cube
//        ofRotate(0, orientation.x, orientation.y, orientation.z);
//        
//        //        ofBoxPrimitive p(100,100,100);
//        //        p.drawWireframe();
//        //
//        //        ofDrawAxis(20);
        ofPopMatrix();
        
        centerPoint.draw();
        if (boundingbox) {
            ofSetLineWidth(1);
            ofPushMatrix();
            ofMultMatrix(centerPoint.getGlobalTransformMatrix());
            ofBoxPrimitive p(400, 400, 400);
            ofSetColor(255, 255, 255, 255*alpha);
            p.drawWireframe();
            ofPopMatrix();
            ofSetLineWidth(2);
            ofSetColor(255);
        }
        ofDisableAlphaBlending();
        ofPopStyle();
    }
    
    
    
    
    void drawBones() {
        
        for(auto boneTemp : SKELETOR::Instance()->bones) {
            vector < int > & ptIds = boneTemp.second.pointIds;
            for (int i = 0; i+1 < ptIds.size(); i++){
                
                drawBone(ptIds[i], ptIds[i+1]);
            }
        }
    }
    
    
    
    void drawBone(int indexA, int indexB){
        
        
        if (std::find(SKELETOR::Instance()->skipList.begin(), SKELETOR::Instance()->skipList.end(), SKELETOR::Instance()->indexToName[indexA] )!=SKELETOR::Instance()->skipList.end() ||
            std::find(SKELETOR::Instance()->skipList.begin(), SKELETOR::Instance()->skipList.end(), SKELETOR::Instance()->indexToName[indexB] )!=SKELETOR::Instance()->skipList.end() ){
            
            return;
            
        }
        
        int trackingState;
        
        trackingState = TRACKED;
        
        if (trackingStates[indexA] == INFERRED ||
            trackingStates[indexB] == INFERRED){
            trackingState = INFERRED;
        }
        
        if (trackingStates[indexA] == NOT_TRACKED ||
            trackingStates[indexB] == NOT_TRACKED){
            trackingState = NOT_TRACKED;
        }
        
        if(trackingState == TRACKED) {
            ofSetLineWidth(2);
            ofSetColor(ofColor::white, 100);
            ofLine( pts[indexA], pts[indexB] );
        } else if(trackingState == INFERRED) {
            ofSetLineWidth(2);
            ofSetColor(ofColor::white, 100);
            ofLine( pts[indexA], pts[indexB] );
        }
    }
    
    
    
    
};