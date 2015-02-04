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
#include "ofxMidi.h"
#include "midiTrigger.h"
#include "Graph.h"
#include "skeletonMidiController.h"
#include "Floor.h"
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
	
	void setupAudio();
	void updateAudio(kinectBody &body);
    
    //void newGesture(Gesture & newGest);
    ofxKinectV2OSC kinect;
    Skeleton* skeleton;
    vector<Skeleton>* skeletons;
	
	vector<kinectButton> buttons;
    
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
	
	ofParameterGroup buttonControl;
	ofParameter<bool> buttonDraw;
	ofParameter<float> buttonRadius;
	ofParameter<float> buttonTriggerScale;
	ofParameter<float> buttonApproachScale;
    
    ofParameterGroup graphsControl1;
    ofParameter<float> smoothUpSkeleton;
    ofParameter<float> smoothDownSkeleton;
    vector<ofParameter<float> > graphsSkeletonThresh;
    
    ofParameterGroup graphsControl2;
    vector<ofParameter<float> > graphsHistoryThresh;
    ofParameter<float> smoothUpHistory;
    ofParameter<float> smoothDownHistory;
    
    ofParameter<int>   bodyDropThreshold;
    ofParameter<bool> debugMode;
    ofParameter<int> outputMode;
    ofParameter<int> startNote;
    ofParameter<int> numNotes;
    ofParameter<bool> jazzDrums;
    ofParameter<bool> historyAndSkeleton;
    
    ofFbo fooFbo;

    
    unsigned long long bodyDropTimer;
    
    ofxControlPanel gui;
    
    ofCamera cam;
	
    ofPolyline leftHandHistory;
    ofPolyline rightHandHistory;
    ofMatrix4x4 mat;
    
    ofParameter <string> status;
    
    kinectSkeleton KS;
    kinectBody KB;
	kinectBodyAnalyser KBA;
    
    map<string, kinectBody> bodyMap;
    map<string, kinectSkeleton> ksMap;
	
    //kinectSkeletonAnalyzerInterpreter KSAI;
    kinectSkeletonAnalyser KSA;
    udpPacketReplayer UDPR;
    
    
    skeletonMidiController skeletonMidi;
	
	shared_ptr<ofxMidiOut> midiOut;
	vector<int> midiNotes;
    map<string, bool> triggers;
	
	vector<triggerRef> midiTriggers;
    //void newGesture(Gesture &newGest);
    
    
    vector < Graph > graphs;
    vector < Graph > graphsForSkeleton;
    vector < Graph > graphsHistory;
    
    Floor floorProjections;
    
};

