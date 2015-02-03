#pragma once

#include "kinectBody.h"
#include "ofxMidiOut.h"


class midiTrigger {
	
public:
	virtual void update(kinectBody& body) { };
	virtual void setMidiOut(shared_ptr<ofxMidiOut> midiOut);
	
protected:
	shared_ptr<ofxMidiOut> getMidiOut();
	
private:
	shared_ptr<ofxMidiOut> _midiOut;
};


class grabbedNote : public midiTrigger {
	
public:
	grabbedNote();
	virtual void update(kinectBody& body);
	
private:
	bool _triggered;
	int _currentNote;
	vector<int> _notes;
};

class gridNote : public midiTrigger {
	
public:
	gridNote();
	virtual void update(kinectBody& body);
	
private:
	bool _triggered;
	int _currentNote;
	vector<int> _notes;
};

typedef shared_ptr<midiTrigger> triggerRef;
