//
//  skeletonMidiController.cpp
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#include "skeletonMidiController.h"


void skeletonMidiController::setup(shared_ptr<ofxMidiOut> midi){
    midiOut = midi;
}

void skeletonMidiController::triggerNote(int channel, int note, int velocity){
    if(ofGetElapsedTimeMillis() - lastSend[note] > 30){
        midiOut->sendNoteOn(channel, note, velocity);
        lastSend[note] = ofGetElapsedTimeMillis();
    }
}