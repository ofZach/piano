//
//  ViewRenderer.h
//  sceneView
//
//  Created by dantheman on 3/14/15.
//
//
#pragma once
#include "ofMain.h"
#include "ofxMidi.h"
#include "SkeletonView.h"
#include "FloorView.h"
#include "MidiView.h"
#include "TVView.h"
#include "ofxGui.h"
#define N_SCENE 4
class ViewRenderer{
public:
    ViewRenderer();
    virtual ~ViewRenderer();
    void setup();
    void update();
    void draw();
    void setMainView(int i);
    void setupViewports();
    void exit();
    
    void triggerFloor();
    
    ofRectangle* getViews();
    
    ofRectangle viewMain;
    ofRectangle viewGrid[N_SCENE];
    int iMainView;
    
private:    
    vector<ofFbo*> views;
    
    SkeletonView kinectSkeleton;
    MidiView musicMaker;
    TVView tv;
    FloorView floor;
    
    shared_ptr<ofxMidiOut> midiOut;
    
    ofxPanel skeletonGUI;
    ofxPanel projectionGUI;
    ofxPanel tvGUI;
    ofxPanel midiGUI;
    
    int numPlayers;

    
};