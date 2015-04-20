//
//  MidiView.h
//  midiDebug
//
//  Created by dantheman on 3/14/15.
//
//
#ifndef MIDIVIEW_H
#define MIDIVIEW_H
#pragma once
#include "ofxMidi.h"
#include "musicMaker.h"
class MidiView{
public:
    MidiView();
    virtual ~MidiView();
    
    void setup(int numPlayers, shared_ptr<ofxMidiOut> midiOut, ofRectangle viewport = ofGetCurrentViewport());
    void update(kinectBody * playerOne, kinectBody * playerTwo);
    void draw(ofRectangle viewport);
    
    void exit();
    
    void setupGUI();
    bool isMain();
    void setMainView(bool mainView);
    void setPlayerTwoMode(int mode);
    void setPlayerOneMode(int mode);
    ofParameterGroup midiGroup;
    ofParameterGroup playerOne;
    ofParameterGroup playerTwo;
    

    int numPlayers;
    
    ofTrueTypeFont font;
    ofTrueTypeFont fontDebug;
    ofTrueTypeFont fontMain;
    
    musicMaker musicMakerP1;
    musicMaker musicMakerP2;
    shared_ptr<ofxMidiOut> midiOut;

    bool bMainView;
    
    ofRectangle viewPort;
    Trigger lastTriggerP1;
    Trigger lastTriggerP2;
    
    bool clearOne;
    bool clearTwo;
    
    float lastBodyPlayerOne;
    float lastBodyPlayerTwo;
};
#endif