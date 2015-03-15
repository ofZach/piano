
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

struct Trigger{
    float time;
    int channel;
};

class musicMaker {
    
    
public:
    
    void setup (shared_ptr<ofxMidiOut> midi, int offset = 0);
    void addToDebugParamGroup ( ofParameterGroup & debugView);
    void drawInScene();
    void drawOverScene();
    void analyze (kinectBody & body);
    void exit();
	void setupGraphs();
	void setupMidiTriggers();
	void updateMidiTriggers(kinectBody &body);
	void updateGraphs(kinectBody &body);
    void clearBodies();
    Trigger lastTriggeredNote();
    
	void outputmodeChanged(int& mode);
	
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
    ofParameter<float> percentile;
    ofParameter<float> thresholdSmoothing;
    ofParameter<float> velocityCutOff;
    skeletonMidiController skeletonMidi;
	
	shared_ptr<ofxMidiOut> midiOut;
	vector<int> midiNotes;
    map<string, bool> triggers;
	vector<triggerRef> midiTriggers;
	vector<triggerRef> drumMidiTriggers;
	
    vector<kinectButton> buttons;
    
    int channelOffset;
    
    
    Trigger lastTrigger;
    
};