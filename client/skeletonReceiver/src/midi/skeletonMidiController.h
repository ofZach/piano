//
//  skeletonMidiController.h
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxMidi.h"
class skeletonMidiController{
public:
    void setup(shared_ptr<ofxMidiOut> midi);
    
    void triggerNote(int channel, int note, int velocity);
    
private:
    shared_ptr<ofxMidiOut> midiOut;
    
    map<int, unsigned long long> lastSend;
};