#include "midiController.h"

midiController::midiController(){

}

midiController::~midiController(){
    
}

void midiController::setup(){
    ofAddListener(ofEvents().update, this, &midiController::update);
    sequencer.setup(6, 32);
    sequencer.setBpm(120, 4);
    sequencer.setDiscrete(false);
    sequencer.setRange(0.0, 127.0);
    sequencer.setPosition(ofGetWidth()-300, ofGetHeight()-200, 300, 200);
    sequencer.setMouseActive(false);
    ofAddListener(sequencer.sequencerEvent, this, &midiController::sequencerStep);
    sequencer.start();
}

void midiController::updateSequencerStep(int row, float value){
    int col = sequencer.getColumn();
    sequencer.setValue(row, col, value);
}

void midiController::clear(){
    for(int i = 0; i < sequencer.getNumberColumns(); i++){
        for(int j = 0; j < sequencer.getNumberRows(); j++){
            sequencer.setValue(j, i, 0);
        }
    }
}

void midiController::sequencerStep(vector<float> &column) {
    int count = 12;
    for(int i = 0; i < column.size(); i++){
        triggerMidiNote(i, count++, column[i]);
        column[i] = column[i]*0.5;
    }
}

void midiController::update(ofEventArgs & args){

}

void midiController::setBMP(int bpm){
    sequencer.stop();
    sequencer.setBpm(bpm);
    sequencer.start();
}

void midiController::draw(){
    sequencer.draw();
}

void midiController::triggerMidiNote(int channel, int note, int velocity){
    midiOut.sendNoteOn(channel, note, velocity);
}
