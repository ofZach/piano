//
//  kinectSkeletonAnalyser.h
//  skeletonReceiver
//
//  Created by dantheman on 2/2/15.
//
//

#pragma once
#include "ofMain.h"
#include "kinectSkeleton.h"
class kinectSkeletonAnalyser{
public:
    
    ofCamera normCam;
    
    void analyze(kinectSkeleton & skeleton){
        
        
        //---------------------------------------------------------------------------------------
        //
        // update arm values
        //
        //---------------------------------------------------------------------------------------

        ofPoint elbows[2];
        ofPoint hands[2];
        ofPoint shoulders[2];
        ofPoint hips[2];
        ofPoint spineshoulders = skeleton.getCenterPoint(spineShoulder);
        
        for (int i = 0; i < 2; i++) {
            elbows[i] = i==0?skeleton.getLeftPoint(elbow):skeleton.getRightPoint(elbow);
            shoulders[i] =i==0?skeleton.getLeftPoint(shoulder):skeleton.getRightPoint(shoulder);
            hands[i] = i==0?skeleton.getLeftPoint(hand):skeleton.getRightPoint(hand);
            hips[i] = i==0?skeleton.getLeftPoint(hip):skeleton.getRightPoint(hip);
        }
        
        float handDist[2];
        float totalDist[2];
        
        for (int i = 0; i < 2; i++) {
            handDist[i] = (spineshoulders - hands[i]).length();
            totalDist[i] = (spineshoulders - shoulders[i]).length() + (shoulders[i] - elbows[i]).length() + (elbows[i] - hands[i]).length();
        }
        
        skeleton.armLeftExtendedPct = handDist[0] / totalDist[0];
        skeleton.armRightExtendedPct = handDist[1] / totalDist[1];
        
        ofPoint spinebase = skeleton.getCenterPoint(spineBase);
        float handtohandDist[2];
        float totalDistHands[2];
        
        for (int i = 0; i < 2; i++) {
            handtohandDist[i] = (hands[(i + 1) % 2] - hands[i]).length();
            totalDistHands[i] = (hands[i] - elbows[i]).length() + (elbows[i] - shoulders[i]).length() + (shoulders[i] - spineshoulders).length() + (spineshoulders - shoulders[(i + i) % 2]).length() + (elbows[(i + 1) % 2] - shoulders[(i + 1) % 2]).length() + (hands[(i + 1) % 2] - hands[(i)]).length();
        }
        
        skeleton.leftHandSpan = handtohandDist[0] / totalDistHands[0];
        
        skeleton.rightHandSpan = handtohandDist[1] / totalDistHands[1];
        
        float handSpineDist[2];
        float totalHipDist[2];
        
        for (int i = 0; i < 2; i++) {
            handSpineDist[i] = (hips[i] - hands[i]).length();
            totalHipDist[i] = (hips[i] - spinebase).length() + (spineshoulders - spinebase).length() + (spineshoulders - shoulders[i]).length() + (shoulders[i] - elbows[i]).length() + (elbows[i] - hands[i]).length();
        }
        
         skeleton.leftHandVHip = handSpineDist[0] / totalHipDist[0];
         skeleton.rightHandVHip = handSpineDist[1] / totalHipDist[1];
         skeleton.angleLeftElbow = (hands[0] - elbows[0])
        .normalized()
        .angle((elbows[0] - shoulders[0]).normalized());
         skeleton.angleRightElbow = (hands[1] - elbows[1])
        .normalized()
        .angle((elbows[1] - shoulders[1]).normalized());
        
        skeleton.angleHandToHand = (hands[0] - hands[1]).angle( skeleton.orientation - ofVec3f(0, 1, 0));

        
        //---------------------------------------------------------------------------------------
        //
        // update leg values
        //
        //---------------------------------------------------------------------------------------

        
        ofPoint knees[2];
        ofPoint feet[2];
        ofPoint headPt = skeleton.getCenterPoint(head);

        
        for (int i = 0; i < 2; i++) {
            hips[i] = i==0?skeleton.getLeftPoint(hip):skeleton.getRightPoint(hip);
            knees[i] = i==0?skeleton.getLeftPoint(knee):skeleton.getRightPoint(knee);
            feet[i] = i==0?skeleton.getLeftPoint(ankle):skeleton.getRightPoint(ankle);
        }
        
        float feetDist[2];
        float totalFeetDist[2];
        
        for (int i = 0; i < 2; i++) {
            feetDist[i] = (hips[i] - feet[i]).length();
            totalFeetDist[i] = (hips[i] - knees[i]).length() + (knees[i] - feet[i]).length();
        }
        
        skeleton.legLeftExtendedPct = feetDist[0] / totalFeetDist[0];
        skeleton.legRightExtendedPct = feetDist[1] / totalFeetDist[1];
        
        skeleton.angleLeftKnee = (feet[0] - knees[0]).angle(knees[0] - hips[0]);
        skeleton.angleRightKnee = (feet[1] - knees[1]).angle(knees[1] - hips[1]);
        float footToFootDist[2];
        
        for (int i = 0; i < 2; i++) {
            footToFootDist[i] = (feet[0] - feet[1]).length();
            totalFeetDist[i] = (spinebase - hips[i]).length() + (hips[i] - knees[i]).length() + (knees[i] - feet[i]).length();
        }
        
        skeleton.leftFootSpan = footToFootDist[0] / totalFeetDist[0];
        skeleton.rightFootSpan = footToFootDist[1] / totalFeetDist[1];
        
        for (int i = 0; i < 2; i++) {
            footToFootDist[i] = (feet[i] - headPt).length();
            totalFeetDist[i] = (headPt - spinebase).length() + (spinebase - hips[i]).length() + (hips[i] - knees[i]).length() + (knees[i] - feet[i]).length();
        }
        
        skeleton.distFootLeft = footToFootDist[0] / totalFeetDist[0];
        skeleton.distFootRight = footToFootDist[1] / totalFeetDist[1];

        skeleton.shouldersWidth = (shoulders[0] - shoulders[1]).length();
        
    
        //---------------------------------------------------------------------------------------
        //
        // update orientation
        //
        //---------------------------------------------------------------------------------------
        
        
        ofPoint spineDiff = skeleton.getCenterPoint(spineBase) - skeleton.getCenterPoint(spineShoulder);
        ofPoint shoulderDiff = (shoulders[0] - shoulders[1]);
        spineDiff.normalize();
        shoulderDiff.normalize();
        skeleton.orientation = shoulderDiff.cross(spineDiff);
        
        skeleton.centerPoint.setPosition(skeleton.getCenterPoint(spineMid));
        ofQuaternion q;
        q.makeRotate(ofPoint(0, 0, 1), skeleton.orientation);
        skeleton.centerPoint.setOrientation(q);
        
        
        // make 2d points:
        normCam.setPosition(skeleton.centerPoint.getPosition() + ofPoint(0, 0, -1000));
        normCam.lookAt(skeleton.centerPoint, skeleton.centerPoint.getUpDir());
        for (int i = 0; i < skeleton.pts.size(); i++) {
            skeleton.pts2d[i] = normCam.worldToScreen(skeleton.pts[i], ofRectangle(0, 0, 640, 480));
        }
  
    }
};