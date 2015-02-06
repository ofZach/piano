#pragma once

#include "ofMain.h"
#include "ofxKinectV2OSC.h"
#include "kinectSkeleton.h"
#include "kinectSkeletonAnalyser.h"
#include "kinectButton.h"
#include "kinectBody.h"
#include "kinectBodyAnalyser.h"
#include "udpPacketReplayer.h"
#include "ofxControlPanel.h"


#include "tvScreen.h"
#include "Floor.h"
#include "musicMaker.h"



class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
	void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	 
    //void newGesture(Gesture & newGest);
    ofxKinectV2OSC kinect;
    Skeleton* skeleton;
    vector<Skeleton>* skeletons;
	
	 
    BodyRenderer renderer;
    
    ofParameterGroup skeletonTransform;
    ofParameter <float> scaleX;
    ofParameter <float> scaleY;
    ofParameter <float> scaleZ;
    ofParameter <float> rotationX;
    ofParameter <float> rotationY;
    ofParameter <float> rotationZ;
    ofParameter <float> offsetX;
    ofParameter <float> offsetY;
    ofParameter <float> offsetZ;
    ofParameterGroup debugView;
    ofParameter <bool> drawSkeleton;
    ofParameter <bool> drawAnalyzer;
    ofParameter <bool> drawBoundingCube;
    
    ofParameterGroup dataPlayer;
    ofParameter <bool> bUseUdpPlayer;
    ofParameter <bool> bLoadNewUDP;
    ofParameter<float> udpDuration;
    
    ofParameterGroup cameraControl;
    ofParameter <float> cameraHeight;
    ofParameter <float> cameraRadius;
    ofParameter <float> cameraAngle;
	
    
    kinectButton switchMode;
    ofParameter<float> buttonX;
    ofParameter<float> buttonY;
    ofParameter<float> buttonZ;
    ofParameter<float> buttonApproach;
    ofParameter<float> buttonTriggerScale;
    ofParameter<float> buttonRadius;
    bool changeTriggered;
    ofFbo fooFbo;

    
    musicMaker SA;
    
    unsigned long long bodyDropTimer;

    ofxControlPanel gui;
    
    ofCamera cam;
	
    ofMatrix4x4 mat;
    
    ofParameter <string> status;
    
    kinectSkeleton KS;
    kinectBody KB;
	kinectBodyAnalyser KBA;
    
    
    kinectSkeletonAnalyser KSA;
    udpPacketReplayer UDPR;
    
    
    map<string, kinectSkeleton> ksMap;
    map<string, kinectBody> bodyMap;
    
    
    tvScreen TV;
    Floor floorProjections;
    
};

