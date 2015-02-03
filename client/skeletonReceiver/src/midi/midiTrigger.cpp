#include "midiTrigger.h"

#pragma mark - Interface

void midiTrigger::setMidiOut(shared_ptr<ofxMidiOut> midiOut) {
	_midiOut = midiOut;
}

shared_ptr<ofxMidiOut> midiTrigger::getMidiOut() {
	return _midiOut;
}

#pragma mark - Util

float NormalizedHeight(kinectSkeleton& sk, int name, int side, bool clamp = true) {
	if(sk.skeletonHeight == 0) {
		return 0;
	} else {
		ofPoint p = sk.getPoint(name, side);
		ofPoint f = sk.getPoint(::foot, side);
		return ofMap(p.distance(f), 0, sk.skeletonHeight, 0, 1, clamp);
	}
}

template <typename T>
T ExtractNote(const vector<T>& notes, float perc) {
	float mx = notes.size() - 1;
	return notes[ofClamp(perc * mx, 0, mx)];
}

int ExtractNote(const midiTrigger::Settings &settings, float perc) {
	return ExtractNote(settings.notes, perc);
}

#pragma mark - Grabbed Note

grabbedNote::grabbedNote() : _triggered(false), _currentNote(0) {

}

void grabbedNote::update(kinectBody &body) {
	kinectSkeleton& sk = body.getLastSkeleton();
	
	float extendThreshHi = 0.8;
	float extendThreshLow = 0.7;
	
	float extendPerc = sk.armRightExtendedPct;
	
	float height = ofMap(NormalizedHeight(sk, ::hand, ::right), 0, 1, 0.3, 1);
	int note = ExtractNote(getSettings(), height);
	
	ofVec3f acc = body.accel[sk.rightEnumsToIndex[::hand]];
	
	float sum = acc.x + acc.y + acc.z;
	bool shouldTrig = sum < -3 && extendPerc > extendThreshHi;
	bool shouldStop = extendPerc < extendThreshLow;
	
	if(shouldTrig && !_triggered) {
		getMidiOut()->sendNoteOn(getSettings().channel, note);
		_triggered = true;
	} else if(shouldStop && _triggered) {
		getMidiOut()->sendNoteOff(getSettings().channel, note);
		_triggered = false;
	} else if(_triggered && _currentNote != note) {
		getMidiOut()->sendNoteOn(getSettings().channel, note);
		getMidiOut()->sendNoteOff(getSettings().channel, _currentNote);
	}
	
	_currentNote = note;
}

#pragma mark - Grid Note

gridNote::gridNote() : _triggered(false), _currentNote(0) {

}

void gridNote::update(kinectBody &body) {
	kinectSkeleton& sk = body.getLastSkeleton();
	float extendThreshHi = 0.6;
	float extendPerc = sk.armLeftExtendedPct;
	
	bool shouldTrig = extendPerc > extendThreshHi;
	int note = ExtractNote(getSettings(), NormalizedHeight(sk, ::hand, ::left, true));
	
	ofVec3f handVel = body.velocity[sk.leftEnumsToIndex[::hand]];
	int vel = ofMap(handVel.length(), 0, 40, 40, 120);
	
	if(shouldTrig && note != _currentNote) {
		getMidiOut()->sendNoteOn(getSettings().channel, note, vel);
		getMidiOut()->sendNoteOff(getSettings().channel, _currentNote);
		_triggered = true;
	} else if(!shouldTrig && _triggered) {
		getMidiOut()->sendNoteOff(getSettings().channel, _currentNote);
		_triggered = false;
	}
	
	_currentNote = note;
}

#pragma mark - Hand Spread Note

handSpreadNote::handSpreadNote() : _triggered(false) {
	
}

void handSpreadNote::update(kinectBody &body) {
//	kinectSkeleton& sk = body.getLastSkeleton();
//	float height = NormalizedHeight(sk, ::hand, ::right);
//	
//	float onThresh = 0.1;
//	
//	if(!_triggered && mapped > onThresh) {
//		getMidiOut()->sendNoteOn(6, 67);
//		_triggered = true;
//	} else if(_triggered && mapped < onThresh) {
//		getMidiOut()->sendNoteOff(6, 67);
//		_triggered = false;
//	}
//	
//	getMidiOut()->sendAftertouch(6, mapped * 127);
}
