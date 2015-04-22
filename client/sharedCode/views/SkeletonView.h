//
//  SkeletonView.h
//  kinectDebug
//
//  Created by dantheman on 3/14/15.
//
//
#ifndef SKELETONVIEW_H
#define SKELETONVIEW_H
#pragma once
#include "ofMain.h"
#include "Skeleton.h"
#include "ofxKinectV2OSC.h"
#include "kinectBody.h"
#include "kinectSkeleton.h"
#include "kinectBodyAnalyser.h"
#include "kinectSkeletonAnalyser.h"
#include "kinectButton.h"
#define D_CAMERAS 4

class SkeletonView{
public:
    SkeletonView();
    virtual ~SkeletonView();
    
    void setup(int numPlayer, ofRectangle viewPort = ofGetCurrentViewport());
    void update();
    void draw(ofRectangle viewport);
    void exit();
    void setupGUI();
    bool isMain();
    void setMainView(bool mainView);
    void setButtonPos(ofVec3f p1, ofVec3f p2);
    void setPlayerOneModeParam(ofParameterGroup & params);
    void setPlayerTwoModeParam(ofParameterGroup & params);
    void drawScene(ofRectangle viewport, ofCamera camera);
    kinectBody * getBody(int i);
    
    int getPlayerOneMode();
    int getPlayerTwoMode();
    
    ofParameterGroup skeletonTransform;
    ofParameterGroup hiddenSettings;
    ofParameterGroup stageParams;
private:
    //Kinect
    ofxKinectV2OSC kinect;

    int playerOneMode;
    int playerTwoMode;
    
    //Matrix Transform
    ofMatrix4x4 mat;
    
    //Data Holders
    Skeleton* skeleton;
    kinectSkeleton KS;
    kinectBody KB;
    
    //Kinect Analysers
    kinectBodyAnalyser KBA;
    kinectSkeletonAnalyser KSA;
    
    ofPoint p1SwitchMode, p2SwitchMode;
    
    //Collections
    vector<Skeleton>* skeletons;
    map<string, kinectSkeleton> ksMap;
    map<string, kinectBody> bodyMap;

    int numPlayers;
    //3D Drawing
    ofCamera cam;
    
    ofCamera leftCam;
    ofCamera topCam;
    ofCamera frontCam;
    
    

    
    ofRectangle rect[D_CAMERAS];
    
    bool changeTriggeredOne, changeTriggeredTwo;
    

    ofParameter<float> stageSize;
    
    ofParameter<ofVec3f> playerOneStagePosition, playerTwoStagePosition, playerOneTriggerPosition, playerTwoTriggerPosition, skeletonRotation, skeletonOffset, skeletonScale;

    
    ofParameter<bool> bCalibrate;
    
    
    ofParameter<bool> overRideLeftTrigger;
    ofParameter<float> stageLeftTriggerScale;

    
    ofParameter<bool> overRideRightTrigger;
    ofParameter<float> stageRightTriggerScale;

    
    ofParameter<float> presencesZoneSizeP1;
    ofParameter<float> presencesZoneSizeP2;
    
    
    ofRectangle viewPort;
    bool bMainView;
    
    ofPlanePrimitive plane;
    
    kinectButton playerOneSwitchMode;
    kinectButton playerTwoSwitchMode;
    
    kinectButton stageOne;
    kinectButton stageTwo;
    
};
#endif