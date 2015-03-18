//
//  SkeletonView.h
//  kinectDebug
//
//  Created by dantheman on 3/14/15.
//
//

#pragma once
#include "ofMain.h"
#include "Skeleton.h"
#include "ofxKinectV2OSC.h"
#include "kinectBody.h"
#include "kinectSkeleton.h"
#include "kinectBodyAnalyser.h"
#include "kinectSkeletonAnalyser.h"
#include "kinectButton.h"
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
    kinectBody * getBody(int i);
    
    int getPlayerOneMode();
    int getPlayerTwoMode();
    
    ofParameterGroup skeletonTransform;
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
    bool changeTriggered;
    
    ofParameter <float> scaleX;
    ofParameter <float> scaleY;
    ofParameter <float> scaleZ;
    ofParameter <float> rotationX;
    ofParameter <float> rotationY;
    ofParameter <float> rotationZ;
    ofParameter <float> offsetX;
    ofParameter <float> offsetY;
    ofParameter <float> offsetZ;
    ofParameter<float> stageSize;
    ofParameter<float> stageLeftX;
    ofParameter<float> stageLeftY;
    ofParameter<float> stageLeftZ;
    ofParameter<float> stageRightX;
    ofParameter<float> stageRightY;
    ofParameter<float> stageRightZ;
    

    ofRectangle viewPort;
    bool bMainView;
    
    ofPlanePrimitive plane;
    
    kinectButton playerOneSwitchMode;
    kinectButton playerTwoSwitchMode;
};
