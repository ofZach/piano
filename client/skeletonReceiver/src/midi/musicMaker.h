
#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "kinectBody.h"
#include "skeletonMidiController.h"
#include "Graph.h"
#include "ofxMidi.h"
#include "midiTrigger.h"
#include "kinectButton.h"

// I analyze and do midi

class musicMaker {
    
    
public:
    
    void setup ();
    void addToDebugParamGroup ( ofParameterGroup & debugView);
    void drawInScene();
    void drawOverScene();
    void analyze (kinectBody & body);
    void exit();
    void setupAudio();
    void updateAudio(kinectBody &body);
    void clearBodies();
    
   
    
    vector < Graph > graphs;
    vector < Graph > graphsForSkeleton;
    vector < Graph > graphsHistory;
    
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
    
    skeletonMidiController skeletonMidi;
	
	shared_ptr<ofxMidiOut> midiOut;
	vector<int> midiNotes;
    map<string, bool> triggers;
	vector<triggerRef> midiTriggers;
    
    vector<kinectButton> buttons;
    
};