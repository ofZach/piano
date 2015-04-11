//
//  skeletonMidiController.h
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//
#ifndef SKELETONMIDICONTROLLER_H
#define SKELETONMIDICONTROLLER_H
#pragma once
#include "ofMain.h"
#include "ofxMidi.h"


class skeletonMidiController{
public:
    void setup(shared_ptr<ofxMidiOut> midi, int offset);
    
    void triggerNote(int note, int velocity);
    void triggerPunch(int note, int velocity);
    void triggerKick(int note, int velocity);
private:
    shared_ptr<ofxMidiOut> midiOut;
    map<int, unsigned long long> lastSend;
    int channelOffset;

};
#endif