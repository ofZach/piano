#pragma once

#include "kinectBody.h"
#include "ofxMidiOut.h"


class midiTrigger {
	
public:
	
	struct Settings {
		Settings() : notes(0), channel(1) { }
		vector<int> notes;
		int channel;
	};
	
	virtual void update(kinectBody& body) { };
	virtual void setMidiOut(shared_ptr<ofxMidiOut> midiOut);
	void setSettings(Settings settings) { _settings = settings; }
	
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
	bool _triggered;
	int _currentNote;
};

class gridNote : public midiTrigger {
	
public:
	gridNote();
	virtual void update(kinectBody& body);
	
private:
	bool _triggered;
	int _currentNote;
};

class handSpreadNote : public midiTrigger {
	
public:
	handSpreadNote();
	virtual void update(kinectBody& body);
	
private:
	bool _triggered;
};

typedef shared_ptr<midiTrigger> triggerRef;
