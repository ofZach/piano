//
//  midiView.h
//  midiDebug
//
//  Created by dantheman on 3/14/15.
//
//

#pragma once
#include "ofxMidi.h"
#include "musicMaker.h"
#include "ofxControlPanel.h"

class midiView{
public:
    midiView();
    virtual ~midiView();
    
    void setup(int numPlayers, shared_ptr<ofxMidiOut> midiOut, ofRectangle viewport = ofGetCurrentViewport());
    void update(kinectBody * playerOne, kinectBody * playerTwo);
    void draw();
    
    void exit();
    
    void drawControlPanel();
    void setupGUI();
    bool isMain();
    void setMainView(bool mainView);
    
private:
    int numPlayers;
    
    ofTrueTypeFont font;
    
    musicMaker musicMakerP1;
    musicMaker musicMakerP2;    
    shared_ptr<ofxMidiOut> midiOut;
    
    ofParameterGroup playerOne;
    ofParameterGroup playerTwo;
    
    ofParameter<bool> sendMidiTest;
    
    ofxControlPanel midiControlPanel;
    
    bool bMainView;
    
    ofRectangle viewPort;
    Trigger lastTriggerP1;
    Trigger lastTriggerP2;
};
