#ifndef MIDITRIGGER_H
#define MIDITRIGGER_H
#pragma once

#include "kinectBody.h"
#include "ofxMidiOut.h"

extern void AllNotesOff(ofxMidiOut& midiOut);

class midiTrigger {
	
public:
	
	midiTrigger();
	
	struct Settings {
		Settings(int channel = 1, int side = ::left) : notes(0), channel(channel), side(side) { }
		vector<int> notes;
		int channel;
		int side;
	};
	
	virtual void update(kinectBody& body) { };
	virtual void reset();
	virtual void setMidiOut(shared_ptr<ofxMidiOut> midiOut);
	void setSettings(Settings settings) { _settings = settings; }
    bool _triggered;
    float _time;
protected:
	shared_ptr<ofxMidiOut> getMidiOut();
	Settings& getSettings() { return _settings; }
	
private:

	shared_ptr<ofxMidiOut> _midiOut;
	Settings _settings;
};

class grabbedNote : public midiTrigger {
	
public:
	grabbedNote();
	virtual void update(kinectBody& body);
	
private:
//	bool _triggered;
	int _currentNote;
};

class gridNote : public midiTrigger {
	
public:
	gridNote();
	virtual void update(kinectBody& body);
	
private:
	bool _triggered;
	int _currentNote;
	unsigned long long _lastTrigger;
};

class accordianNote : public midiTrigger {
	
public:
	accordianNote();
	virtual void update(kinectBody& body);
	
private:
//	bool _triggered;
	int _currentNote;
	float _lastDist;
};

class legCC : public midiTrigger {
	
public:
	legCC();
	virtual void update(kinectBody& body);
	
private:
	double _accumulator;
};

class stompNote : public midiTrigger {
	
public:
	stompNote();
	virtual void update(kinectBody& body);
	virtual void reset();
	
private:
	bool _primed;
	int _ignoredFrameCount;
};

class dropDatNote : public midiTrigger {
	
public:
	dropDatNote();
	virtual void update(kinectBody &body);
	virtual void reset();
	
private:
	bool _primed;
	int _ignoredFrameCount;
};

#endif