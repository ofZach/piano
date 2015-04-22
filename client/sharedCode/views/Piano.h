//
//  Piano.h
//  sceneView
//
//  Created by dantheman on 3/14/15.
//
//
#ifndef PIANO_H
#define PIANO_H
#pragma once
#include "ofMain.h"
#include "ofxMidi.h"
#include "SkeletonView.h"
#include "FloorView.h"
#include "MidiView.h"
#include "TVView.h"
#include "ofxGui.h"
#include "AppSettings.h"
#include "kinectButton.h"
#define N_SCENE 5


// change view to viewport


class Piano{
public:
    Piano();
    virtual ~Piano();
    void setup();
    void update();
    void draw();
    void setMainView(int i);
    void setupViewports();
    void exit();
    
    void triggerFloor();
    
    ofRectangle* getViews();
    
    ofRectangle projectorRect;
    ofRectangle tvRect;
    
    ofRectangle viewMain;
    ofRectangle viewGrid[N_SCENE];
    int iMainView;
    
    
    void addLineTracePlayerOne(int & i);
    void triggerTrianglePlayerOne(int & i);
    void addImpulsePlayerOne(int & i);
    
    void addLineTracePlayerTwo(int & i);
    void triggerTrianglePlayerTwo(int & i);
    void addImpulsePlayerTwo(int & i);
        
    vector<ofFbo*> views;
    
    AppSettings settings;
    
    SkeletonView skelView;
    MidiView midiView;
    TVView tvView;
    FloorView floorView;
    
    shared_ptr<ofxMidiOut> midiOut;
    
    ofxPanel hiddenSettings;
    ofxPanel skeletonGUI;
    ofxPanel stageGUI;
    ofxPanel appGUI;
    int numPlayers;
    
    bool bExpertMode;
};
#endif