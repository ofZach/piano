//
//  ViewRenderer.h
//  sceneView
//
//  Created by dantheman on 3/14/15.
//
//
#pragma once
#include "ofMain.h"
#include "skeletonView.h"
#include "midiView.h"
#include "ofxMidi.h"
#define N_SCENE 4
class ViewRenderer{
public:
    ViewRenderer();
    virtual ~ViewRenderer();
    void setup(int numPlayers);
    void update();
    void draw();
    void setMainView(int i);
    void setupViewports();
    void exit();
    
    ofRectangle* getViews();
    
    ofRectangle viewMain;
    ofRectangle viewGrid[N_SCENE];
    int iMainView;
    
private:
    ofFbo skeletonView;
    ofFbo floorView;
    ofFbo midiView;
    ofFbo tvView;
    
    vector<ofFbo*> views;
    
    class skeletonView kinectSkeleton;
    class midiView musicMaker;

    shared_ptr<ofxMidiOut> midiOut;
    
    int numPlayers;
};