#include "midiTrigger.h"

#pragma mark - Interface

void midiTrigger::setMidiOut(shared_ptr<ofxMidiOut> midiOut) {
	_midiOut = midiOut;
}

shared_ptr<ofxMidiOut> midiTrigger::getMidiOut() {
	return _midiOut;
}


#pragma mark - Grabbed Note

grabbedNote::grabbedNote() : _triggered(false), _currentNote(0) {
	_notes.push_back(58);
	_notes.push_back(60);
	_notes.push_back(65);
	_notes.push_back(67);
	_notes.push_back(72);
}

void grabbedNote::update(kinectBody &body) {
	float extendThreshHi = 0.7;
	float extendThreshLow = 0.6;
	
	float extendPerc = body.getLastSkeleton().armRightExtendedPct;
	float elevatePerc = body.getLastSkeleton().rightHandVHip;
	
	ofVec3f acc = body.accel[body.getLastSkeleton().rightEnumsToIndex[::hand]];
	
	float notePerc = ofMap(elevatePerc, 0.3, 0.7, 0, _notes.size() - 1, true);
	int note = _notes[notePerc];
	
	float sum = acc.x + acc.y + acc.z;
	bool shouldTrig = sum < -3 && extendPerc > extendThreshHi;
	bool shouldStop = extendPerc < extendThreshLow;
	
	if(shouldTrig && !_triggered) {
		getMidiOut()->sendNoteOn(4, note);
		_triggered = true;
	} else if(shouldStop && _triggered) {
		getMidiOut()->sendNoteOff(4, note);
		_triggered = false;
	} else if(_triggered && _currentNote != note) {
		getMidiOut()->sendNoteOn(4, note);
		getMidiOut()->sendNoteOff(4, _currentNote);
	}
	
	_currentNote = note;
}

#pragma mark - Grid Note

gridNote::gridNote() : _triggered(false), _currentNote(0) {
	_notes.push_back(58);
	_notes.push_back(60);
	_notes.push_back(64);
	_notes.push_back(65);
	_notes.push_back(67);
	
	_notes.push_back(72);
	_notes.push_back(76);
	_notes.push_back(77);
	_notes.push_back(79);
	_notes.push_back(84);
}

void gridNote::update(kinectBody &body) {
	kinectSkeleton& sk = body.getLastSkeleton();
	float extendThreshHi = 0.6;
	float extendPerc = sk.armLeftExtendedPct;
	
	bool shouldTrig = extendPerc > extendThreshHi;
	ofPoint hand = sk.getLeftPoint(::hand);
	ofPoint foot = sk.getLeftPoint(::foot);
	float dist = hand.distance(foot);
	int note = _notes[ofMap(dist, 0, sk.skeletonHeight, 0, _notes.size() - 1, true)];
	
	ofVec3f handVel = body.velocity[sk.leftEnumsToIndex[::hand]];
	int vel = ofMap(handVel.length(), 0, 50, 40, 120);
	
	if(shouldTrig && note != _currentNote) {
		getMidiOut()->sendNoteOn(5, note, vel);
		getMidiOut()->sendNoteOff(5, _currentNote);
		_triggered = true;
	} else if(!shouldTrig && _triggered) {
		getMidiOut()->sendNoteOff(5, _currentNote);
		_triggered = false;
	}
	
	_currentNote = note;
}
