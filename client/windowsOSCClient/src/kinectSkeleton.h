#ifndef KINECTSKELETON_H
#define KINECTSKELETON_H
#pragma once

#include "ofMain.h"
#include "ofxKinectV2OSC.h"
#include "kinectSkeletonLayout.h"


class kinectSkeleton {
    
public:

    void setup();
    ofPoint getCenterPoint ( int name);  
    ofPoint getLeftPoint ( int name );  
    ofPoint getRightPoint ( int name);   
    ofPoint getPoint(int name, int side);
    void setFromSkeleton( Skeleton & sk, ofMatrix4x4 transform = ofMatrix4x4());
    void draw();
    void drawDebug(bool boundingbox);
    void drawBones();
    void drawBone(int indexA, int indexB);

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
};

#endif