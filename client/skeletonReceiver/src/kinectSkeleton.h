#pragma once

#include "ofMain.h"
#include "ofMain.h"
#include "ofxKinectV2OSC.h"
#include "ofxGui.h"



class kinectSkeleton {
    
public:
    
    
    vector < ofPoint > pts;
    vector < int > trackingStates;
    vector < string > skipList;
    
    
    typedef struct {
        int a;
        int b;
    } connection;
    
    vector < connection > connections;
    map < string, int > nameToIndex;
    map < int, string> indexToName;
    
    void setup(){
        
        string jointNames[25] = { "ThumbRight", "SpineBase", "SpineMid", "Neck", "Head", "ShoulderLeft", "ElbowLeft", "WristLeft", "HandLeft", "ShoulderRight", "ElbowRight", "WristRight", "HandRight", "HipLeft", "KneeLeft", "AnkleLeft", "FootLeft", "HipRight", "KneeRight", "AnkleRight", "FootRight", "SpineShoulder", "HandTipLeft", "ThumbLeft", "HandTipRight"};
        
        for (int i = 0; i < 25; i++){
            nameToIndex[jointNames[i]] = i;
            indexToName[i] = jointNames[i];
            pts.push_back(ofPoint(0,0,0));
            trackingStates.push_back(0);
        }
        
        skipList.push_back("WristRight");
        skipList.push_back("WristLeft");
        skipList.push_back("HandTipRight");
        skipList.push_back("HandTipLeft");
        skipList.push_back("ThumbLeft");
        skipList.push_back("ThumbRight");
        skipList.push_back("FootLeft");
        skipList.push_back("FootRight");
        
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
            if (trackingStates[i] == TRACKED) ofSetColor(255,255,255);
            else if (trackingStates[i] == INFERRED) ofSetColor(127,127,127);
            else ofSetColor(255,0,0);
            ofPushMatrix();
            ofTranslate(pts[i]);
            mesh.draw();
            ofPopMatrix();
        }
        
        drawBones();
    }
    
    
    void drawBones() {
        drawTorso();
        drawRightArm();
        drawLeftArm();
        drawRightLeg();
        drawLeftLeg();
    }
    
    void drawTorso(){
        drawBone(nameToIndex["Head"], nameToIndex["Neck"]);
        drawBone(nameToIndex["Neck"], nameToIndex["SpineShoulder"]);
        drawBone(nameToIndex["SpineShoulder"], nameToIndex["SpineMid"]);
        drawBone(nameToIndex["SpineMid"], nameToIndex["SpineBase"]);
        drawBone(nameToIndex["SpineShoulder"], nameToIndex["ShoulderRight"]);
        drawBone(nameToIndex["SpineShoulder"], nameToIndex["ShoulderLeft"]);
        drawBone(nameToIndex["SpineBase"], nameToIndex["HipRight"]);
        drawBone(nameToIndex["SpineBase"], nameToIndex["HipLeft"]);
    }
    
    void drawRightArm(){
        drawBone(nameToIndex["ShoulderRight"], nameToIndex["ElbowRight"]);
        drawBone(nameToIndex["ElbowRight"], nameToIndex["HandRight"]);
       
    }
    
    void drawLeftArm(){
        drawBone(nameToIndex["ShoulderLeft"], nameToIndex["ElbowLeft"]);
        drawBone(nameToIndex["ElbowLeft"], nameToIndex["HandLeft"]);
  //      drawBone(nameToIndex["WristLeft"], nameToIndex["HandLeft"]);
   //     drawBone(nameToIndex["HandLeft"], nameToIndex["HandTipLeft"]);
   //     drawBone(nameToIndex["WristLeft"], nameToIndex["ThumbLeft"]);
    }
    
    void drawRightLeg(){
        drawBone(nameToIndex["HipRight"], nameToIndex["KneeRight"]);
        drawBone(nameToIndex["KneeRight"], nameToIndex["AnkleRight"]);
        //drawBone(nameToIndex["AnkleRight"], nameToIndex["FootRight"]);
    }
    
    void drawLeftLeg(){
        drawBone(nameToIndex["HipLeft"], nameToIndex["KneeLeft"]);
        drawBone(nameToIndex["KneeLeft"], nameToIndex["AnkleLeft"]);
        //drawBone(nameToIndex["AnkleLeft"], nameToIndex["FootLeft"]);
    }
    
    void drawBone(int indexA, int indexB){
        
        
//        if (std::find(skipList.begin(), skipList.end(), indexToName[indexA] )!=skipList.end() ||
//            std::find(skipList.begin(), skipList.end(), indexToName[indexB] )!=skipList.end() ){
//            
//            return;
//            
//        }
        
        
        
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
            ofSetColor(ofColor::white);
            ofLine( pts[indexA], pts[indexB] );
        } else if(trackingState == INFERRED) {
            ofSetLineWidth(2);
            ofSetColor(ofColor::white);
            ofLine( pts[indexA], pts[indexB] );
        }
    }
    
    
    
    
};