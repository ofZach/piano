#pragma once
#include "ofMain.h"
#include "ofxMidi.h"
//#include "ofxSequencer.h"
class midiController{
public:
    midiController();
    ~midiController();
    void setup();
    void update(ofEventArgs & args);
    void draw();
    void triggerMidiNote(int channel, int note, int velocity);
    void sequencerStep(vector<float> &column);
    void updateSequencerStep(int row, float value);
    void clear();
    void setBMP(int _bpm);
    ofxMidiOut midiOut;
//    ofxSequencer sequencer;
};