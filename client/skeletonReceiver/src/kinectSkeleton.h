#pragma once

#include "ofMain.h"
#include "ofxKinectV2OSC.h"


enum {
    thumb,
    shoulder,
    elbow,
    wrist,
    hand,
    hip,
    knee,
    foot,
    ankle,
    handTip
} symetricalBodyPts;

enum {
    spineBase,
    spineMid,
    spineShoulder,
    neck,
    head
} nonsymeticalPts;

enum side {
    left,
    right,
    center
};

typedef struct {
    vector < int > pointIds;
} bone;


class kinectSkeleton {
    
public:
    
    
    map < int, int > rightEnumsToIndex;
    map < int, int > leftEnumsToIndex;
    map < int, int > centerEnumsToIndex;
    
    map < string, bone > bones;
    
    
    vector < ofPoint > pts;
    vector < int > trackingStates;
    vector < string > skipList;
    
    map < string, int > nameToIndex;
    map < int, string> indexToName;
    
    
    //----------------------------------- statistics to be calculated:
    float shouldersWidth;
    float armLeftExtendedPct;
    float armRightExtendedPct;
    float legLeftExtendedPct;
    float legRightExtendedPct;
    
    
    
    

    void setup(){
        
        string jointNames[25] = { "ThumbRight", "SpineBase", "SpineMid", "Neck", "Head", "ShoulderLeft", "ElbowLeft", "WristLeft", "HandLeft", "ShoulderRight", "ElbowRight", "WristRight", "HandRight", "HipLeft", "KneeLeft", "AnkleLeft", "FootLeft", "HipRight", "KneeRight", "AnkleRight", "FootRight", "SpineShoulder", "HandTipLeft", "ThumbLeft", "HandTipRight"};
        
        for (int i = 0; i < 25; i++){
            nameToIndex[jointNames[i]] = i;
            indexToName[i] = jointNames[i];
            pts.push_back(ofPoint(0,0,0));
            trackingStates.push_back(0);
        }

        
        //--------------------------------------------------------------------- enums
        rightEnumsToIndex[ thumb ]      = nameToIndex["ThumbRight"];
        rightEnumsToIndex[ shoulder ]   = nameToIndex["ShoulderRight"];
        rightEnumsToIndex[ elbow ]      = nameToIndex["ElbowRight"];
        rightEnumsToIndex[ wrist ]      = nameToIndex["WristRight"];
        rightEnumsToIndex[ hand ]       = nameToIndex["HandRight"];
        rightEnumsToIndex[ hip ]        = nameToIndex["HipRight"];
        rightEnumsToIndex[ knee ]       = nameToIndex["KneeRight"];
        rightEnumsToIndex[ foot ]       = nameToIndex["FootRight"];
        rightEnumsToIndex[ ankle ]      = nameToIndex["AnkleRight"];
        rightEnumsToIndex[ handTip ]    = nameToIndex["HandTipRight"];
        
        leftEnumsToIndex[ thumb ]      = nameToIndex["ThumbLeft"];
        leftEnumsToIndex[ shoulder ]   = nameToIndex["ShoulderLeft"];
        leftEnumsToIndex[ elbow ]      = nameToIndex["ElbowLeft"];
        leftEnumsToIndex[ wrist ]      = nameToIndex["WristLeft"];
        leftEnumsToIndex[ hand ]       = nameToIndex["HandLeft"];
        leftEnumsToIndex[ hip ]        = nameToIndex["HipLeft"];
        leftEnumsToIndex[ knee ]       = nameToIndex["KneeLeft"];
        leftEnumsToIndex[ foot ]       = nameToIndex["FootLeft"];
        leftEnumsToIndex[ ankle ]      = nameToIndex["AnkleLeft"];
        leftEnumsToIndex[ handTip ]    = nameToIndex["HandTipLeft"];
        
        centerEnumsToIndex[ spineBase ] = nameToIndex["SpineBase"];
        centerEnumsToIndex[ spineShoulder ] = nameToIndex["SpineShoulder"];
        centerEnumsToIndex[ neck ] = nameToIndex["neck"];
        centerEnumsToIndex[ head ] = nameToIndex["head"];
        centerEnumsToIndex[ spineMid ] = nameToIndex["SpineMid"];
        //---------------------------------------------------------------------

        skipList.push_back("WristRight");
        skipList.push_back("WristLeft");
        skipList.push_back("HandTipRight");
        skipList.push_back("HandTipLeft");
        skipList.push_back("ThumbLeft");
        skipList.push_back("ThumbRight");
        skipList.push_back("FootLeft");
        skipList.push_back("FootRight");
        
        
        bones["leftLeg"].pointIds.push_back(getPointIndex( hip, ::left ));
        bones["leftLeg"].pointIds.push_back(getPointIndex( knee, ::left ));
        bones["leftLeg"].pointIds.push_back(getPointIndex( ankle, ::left ));
        
        bones["rightLeg"].pointIds.push_back(getPointIndex( hip, ::right ));
        bones["rightLeg"].pointIds.push_back(getPointIndex( knee, ::right ));
        bones["rightLeg"].pointIds.push_back(getPointIndex( ankle, ::right ));
        
        bones["leftArm"].pointIds.push_back(getPointIndex( shoulder, ::left ));
        bones["leftArm"].pointIds.push_back(getPointIndex( elbow, ::left ));
        bones["leftArm"].pointIds.push_back(getPointIndex( hand, ::left ));
        
        bones["rightArm"].pointIds.push_back(getPointIndex( shoulder, ::right ));
        bones["rightArm"].pointIds.push_back(getPointIndex( elbow, ::right ));
        bones["rightArm"].pointIds.push_back(getPointIndex( hand, ::right ));
        
        bones["torso"].pointIds.push_back(getPointIndex( head, ::center ));
        bones["torso"].pointIds.push_back(getPointIndex( neck, ::center ));
        bones["torso"].pointIds.push_back(getPointIndex( spineShoulder, ::center ));
        bones["torso"].pointIds.push_back(getPointIndex( spineMid, ::center ));
        bones["torso"].pointIds.push_back(getPointIndex( spineBase, ::center ));
        
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
            if (centerEnumsToIndex.find(name) != centerEnumsToIndex.end()){
                return pts[centerEnumsToIndex[name]];
            }
        } else if (side == ::left){
            cout << "left"<< nameToIndex["HandLeft"] <<  " " << leftEnumsToIndex[name] << endl;
            
            if (leftEnumsToIndex.find(name) != leftEnumsToIndex.end()){
                cout << "found"<< endl;
                
                return pts[leftEnumsToIndex[name]];
            }
        } else if (side == ::right){
            if (rightEnumsToIndex.find(name) != rightEnumsToIndex.end()){
                return pts[rightEnumsToIndex[name]];
            }
        }
        return ofPoint(0,0,0);
    }
    
    int getPointIndex(int name, int side){
        if (side == center){
            if (centerEnumsToIndex.find(name) != centerEnumsToIndex.end()){
                return centerEnumsToIndex[name];
            }
        } else if (side == ::left){
            if (leftEnumsToIndex.find(name) != leftEnumsToIndex.end()){
                return leftEnumsToIndex[name];
            }
        } else if (side == ::right){
            if (rightEnumsToIndex.find(name) != rightEnumsToIndex.end()){
                return rightEnumsToIndex[name];
            }
        }
        return -1;
    }

    
    
    void setFromSkeleton( Skeleton & sk, ofMatrix4x4 transform = ofMatrix4x4()){
        pts[nameToIndex["ThumbRight"]].set(sk.getThumbRight().getPoint() * transform);
        pts[nameToIndex["SpineBase"]].set(sk.getSpineBase().getPoint() * transform);
        pts[nameToIndex["SpineMid"]].set(sk.getSpineMid().getPoint() * transform);
        pts[nameToIndex["Neck"]].set(sk.getNeck().getPoint() * transform);
        pts[nameToIndex["Head"]].set(sk.getHead().getPoint() * transform);
        pts[nameToIndex["ShoulderLeft"]].set(sk.getShoulderLeft().getPoint() * transform);
        pts[nameToIndex["ElbowLeft"]].set(sk.getElbowLeft().getPoint() * transform);
        pts[nameToIndex["WristLeft"]].set(sk.getWristLeft().getPoint() * transform);
        pts[nameToIndex["HandLeft"]].set(sk.getHandLeft().getPoint() * transform);
        pts[nameToIndex["ShoulderRight"]].set(sk.getShoulderRight().getPoint() * transform);
        pts[nameToIndex["ElbowRight"]].set(sk.getElbowRight().getPoint() * transform);
        pts[nameToIndex["WristRight"]].set(sk.getWristRight().getPoint() * transform);
        pts[nameToIndex["HandRight"]].set(sk.getHandRight().getPoint() * transform);
        pts[nameToIndex["HipLeft"]].set(sk.getHipLeft().getPoint() * transform);
        pts[nameToIndex["KneeLeft"]].set(sk.getKneeLeft().getPoint() * transform);
        pts[nameToIndex["AnkleLeft"]].set(sk.getAnkleLeft().getPoint() * transform);
        pts[nameToIndex["FootLeft"]].set(sk.getFootLeft().getPoint() * transform);
        pts[nameToIndex["HipRight"]].set(sk.getHipRight().getPoint() * transform);
        pts[nameToIndex["KneeRight"]].set(sk.getKneeRight().getPoint() * transform);
        pts[nameToIndex["AnkleRight"]].set(sk.getAnkleRight().getPoint() * transform);
        pts[nameToIndex["FootRight"]].set(sk.getFootRight().getPoint() * transform);
        pts[nameToIndex["SpineShoulder"]].set(sk.getSpineShoulder().getPoint() * transform);
        pts[nameToIndex["HandTipLeft"]].set(sk.getHandTipLeft().getPoint() * transform);
        pts[nameToIndex["ThumbLeft"]].set(sk.getThumbLeft().getPoint() * transform);
        pts[nameToIndex["HandTipRight"]].set(sk.getHandTipRight().getPoint() * transform);
        
        
        trackingStates[nameToIndex["ThumbRight"]] = sk.getThumbRight().getTrackingState();
        trackingStates[nameToIndex["SpineBase"]] = sk.getSpineBase().getTrackingState();
        trackingStates[nameToIndex["SpineMid"]] = sk.getSpineMid().getTrackingState();
        trackingStates[nameToIndex["Neck"]] = sk.getNeck().getTrackingState();
        trackingStates[nameToIndex["Head"]] = sk.getHead().getTrackingState();
        trackingStates[nameToIndex["ShoulderLeft"]] = sk.getShoulderLeft().getTrackingState();
        trackingStates[nameToIndex["ElbowLeft"]] = sk.getElbowLeft().getTrackingState();
        trackingStates[nameToIndex["WristLeft"]] = sk.getWristLeft().getTrackingState();
        trackingStates[nameToIndex["HandLeft"]] = sk.getHandLeft().getTrackingState();
        trackingStates[nameToIndex["ShoulderRight"]] = sk.getShoulderRight().getTrackingState();
        trackingStates[nameToIndex["ElbowRight"]] = sk.getElbowRight().getTrackingState();
        trackingStates[nameToIndex["WristRight"]] = sk.getWristRight().getTrackingState();
        trackingStates[nameToIndex["HandRight"]] = sk.getHandRight().getTrackingState();
        trackingStates[nameToIndex["HipLeft"]] = sk.getHipLeft().getTrackingState();
        trackingStates[nameToIndex["KneeLeft"]] = sk.getKneeLeft().getTrackingState();
        trackingStates[nameToIndex["AnkleLeft"]] = sk.getAnkleLeft().getTrackingState();
        trackingStates[nameToIndex["FootLeft"]] = sk.getFootLeft().getTrackingState();
        trackingStates[nameToIndex["HipRight"]] = sk.getHipRight().getTrackingState();
        trackingStates[nameToIndex["KneeRight"]] = sk.getKneeRight().getTrackingState();
        trackingStates[nameToIndex["AnkleRight"]] = sk.getAnkleRight().getTrackingState();
        trackingStates[nameToIndex["FootRight"]] = sk.getFootRight().getTrackingState();
        trackingStates[nameToIndex["SpineShoulder"]] = sk.getSpineShoulder().getTrackingState();
        trackingStates[nameToIndex["HandTipLeft"]] = sk.getHandTipLeft().getTrackingState();
        trackingStates[nameToIndex["ThumbLeft"]] = sk.getThumbLeft().getTrackingState();
        trackingStates[nameToIndex["HandTipRight"]] = sk.getHandTipRight().getTrackingState();

    }
    
    void draw(){
        ofSpherePrimitive primitve(10,10);
        ofVboMesh mesh = primitve.getMesh();
        for (int i = 0; i < pts.size(); i++){
            
            if (std::find(skipList.begin(), skipList.end(), indexToName[i] )!=skipList.end()){
                continue;
            }
            if (trackingStates[i] == TRACKED) ofSetColor(255,255,255, 100);
            else if (trackingStates[i] == INFERRED) ofSetColor(127,127,127, 100);
            else ofSetColor(255,0,0, 100);
            ofPushMatrix();
            
            ofTranslate(pts[i]);
            mesh.draw();
            ofPopMatrix();
        }
        
        drawBones();
    }
    
    
    void drawBones() {
        
        for(auto boneTemp : bones) {
            vector < int > & ptIds = boneTemp.second.pointIds;
            for (int i = 0; i+1 < ptIds.size(); i++){
                drawBone(ptIds[i], ptIds[i+1]);
            }
        }
    }
    
    
    
    void drawBone(int indexA, int indexB){
        
        
        if (std::find(skipList.begin(), skipList.end(), indexToName[indexA] )!=skipList.end() ||
            std::find(skipList.begin(), skipList.end(), indexToName[indexB] )!=skipList.end() ){
            
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