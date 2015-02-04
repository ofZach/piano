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
    //ofLog(OF_LOG_NOTICE)<<"triggerNote "<<channel<<" "<<note<<" "<<velocity<<endl;
    midiOut->sendNoteOn(channel, note, velocity);
}

void skeletonMidiController::triggerPunch(int note, int velocity){
    //ofLog(OF_LOG_NOTICE)<<"triggerPunch "<<" "<<note<<" "<<velocity<<endl;
    midiOut->sendNoteOn(9, note, velocity);
}

void skeletonMidiController::triggerKick(int note, int velocity){
    //ofLog(OF_LOG_NOTICE)<<"triggerKick "<<" "<<note<<" "<<velocity<<endl;
    midiOut->sendNoteOn(9, note, velocity);
}