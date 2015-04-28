#include "midiTrigger.h"
#pragma mark - Interface

midiTrigger::midiTrigger() : _settings(), _triggered(false){
    
}

void midiTrigger::setMidiOut(shared_ptr<ofxMidiOut> midiOut) {
    _midiOut = midiOut;
}

shared_ptr<ofxMidiOut> midiTrigger::getMidiOut() {
    return _midiOut;
}

void midiTrigger::reset() {
    for(const auto& note : getSettings().notes) {
        getMidiOut()->sendNoteOff(getSettings().channel, note);
    }
}

#pragma mark - Util

float NormalizedHeight(kinectSkeleton& sk, int name, int side) {
    if(sk.skeletonHeight == 0) {
        return 0;
    } else {
        ofPoint p = sk.getPoint(name, side);
        ofPoint f = sk.getPoint(::foot, side);
        return ofMap(p.distance(f), 0, sk.skeletonHeight * 1.25, 0, 1);
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

void AllNotesOff(ofxMidiOut& midiOut) {
    for(int channel = 1; channel <= 16; channel++) {
        for(int note = 0; note <= 127; note++) {
            midiOut.sendNoteOff(channel, note);
        }
    }
}

int OtherSide(int side) {
    return (side == ::left ? ::right : ::left);
}

bool GreatestLength(ofPoint a, ofPoint b) {
    return a.lengthSquared() > b.lengthSquared();
}

#pragma mark - Grabbed Note

grabbedNote::grabbedNote() :  _currentNote(0) {
    
}

void grabbedNote::update(kinectBody &body) {
    kinectSkeleton& sk = body.getLastSkeleton();
    
    float extendThreshHi = 0.8;
    float extendThreshLow = 0.7;
    
    float extendPerc = sk.armRightExtendedPct;
    
    float height = ofMap(NormalizedHeight(sk, ::hand, ::right), 0.3, 1, 0, 1);
    int note = ExtractNote(getSettings(), height);
    
    ofVec3f acc = body.accel[SKELETOR::Instance()->rightEnumsToIndex[::hand]];
    
    float sum = acc.x + acc.y + acc.z;
    bool shouldTrig = sum < -3 && extendPerc > extendThreshHi;
    bool shouldStop = extendPerc < extendThreshLow;
    
    if(shouldTrig && !_triggered) {
        getMidiOut()->sendNoteOn(getSettings().channel, note);
        if(getSettings().channel-8 < 0){
           ofNotifyEvent(triggerLinePlayerOne,getSettings().channel, this);
        }else{
            ofNotifyEvent(triggerLinePlayerTwo,getSettings().channel, this);
        }
        _time = ofGetElapsedTimef();
        _triggered = true;
        _currentNote = note;
    } else if(shouldStop && _triggered) {
        getMidiOut()->sendNoteOff(getSettings().channel, note);
        _triggered = false;
        _currentNote = 0;
    } else if(_triggered && _currentNote != note) {
        getMidiOut()->sendNoteOn(getSettings().channel, note);
        getMidiOut()->sendNoteOff(getSettings().channel, _currentNote);
        _currentNote = note;
        if(getSettings().channel-8 < 0){
            ofNotifyEvent(triggerImplusePlayerOne,getSettings().channel, this);
        }else{
            ofNotifyEvent(triggerImplusePlayerTwo,getSettings().channel, this);
        }
        _time = ofGetElapsedTimef();
    }
}

#pragma mark - Grid Note

gridNote::gridNote() : _triggered(false), _currentNote(0), _lastTrigger(0) {
    
}

void gridNote::update(kinectBody &body) {
    kinectSkeleton& sk = body.getLastSkeleton();
    
    float extendPerc;
    ofVec3f handVel;
    
    if(getSettings().side == ::left) {
        extendPerc = sk.armLeftExtendedPct;
        handVel = body.velocity[SKELETOR::Instance()->leftEnumsToIndex[::hand]];
    } else {
        extendPerc = sk.armRightExtendedPct;
        handVel = body.velocity[SKELETOR::Instance()->rightEnumsToIndex[::hand]];
    }
    
    float extendThresh = 0.65;
    bool shouldTrig = extendPerc > extendThresh;
    int note = ExtractNote(getSettings(), NormalizedHeight(sk, ::hand, getSettings().side));
    int vel = ofMap(handVel.length(), 0, 40, 40, 120);
    
    auto now = ofGetElapsedTimeMillis();
    unsigned long long timeThresh = 100;
    bool timeOk = (now - _lastTrigger) > timeThresh;
    
    if(shouldTrig && note != _currentNote && timeOk) {
        getMidiOut()->sendNoteOn(getSettings().channel, note, vel);
        getMidiOut()->sendNoteOff(getSettings().channel, _currentNote);
        if(getSettings().channel-8 < 0){
            ofNotifyEvent(triggerImplusePlayerOne,getSettings().channel, this);
        }else{
            ofNotifyEvent(triggerImplusePlayerTwo,getSettings().channel, this);
        }
        _time = ofGetElapsedTimef();
        _triggered = true;
        _currentNote = note;
        _lastTrigger = now;
    } else if(!shouldTrig && _triggered) {
        getMidiOut()->sendNoteOff(getSettings().channel, _currentNote);
        _triggered = false;
        _currentNote = 0;
        _lastTrigger = now;
    }
}

#pragma mark - Accordian Note

accordianNote::accordianNote() : _currentNote(0), _lastDist(0) {
    getSettings().notes.resize(2);
}

void accordianNote::update(kinectBody &body) {
    kinectSkeleton& sk = body.getLastSkeleton();
    
    float dist = ofMap(sk.getLeftPoint(::hand).distance(sk.getRightPoint(::hand)), 0, sk.skeletonHeight, 0, 1);
    
    float onThresh = 0.5;
    float retrigThresh = 0.0004;
    int note = dist > _lastDist ? getSettings().notes[0] : getSettings().notes[1];
    
    if(dist > onThresh) {
        if(!_triggered) {
            getMidiOut()->sendNoteOn(getSettings().channel, note);
            if(getSettings().channel-8 < 0){
               ofNotifyEvent(triggerLinePlayerOne,getSettings().channel, this);
            }else{
				ofNotifyEvent(triggerLinePlayerTwo,getSettings().channel, this);
            }
            _time = ofGetElapsedTimef();
        } else if(note != _currentNote && abs(_lastDist - dist) > retrigThresh) {
            getMidiOut()->sendNoteOn(getSettings().channel, note);
            getMidiOut()->sendNoteOff(getSettings().channel, _currentNote);
            if(getSettings().channel-8 < 0){
               ofNotifyEvent(triggerLinePlayerOne,getSettings().channel, this);
            }else{
                ofNotifyEvent(triggerLinePlayerTwo,getSettings().channel, this);
            }
            _time = ofGetElapsedTimef();
        }
        _currentNote = note;
        _triggered = true;
        _lastDist = dist;
        
    } else if(_triggered) {
        getMidiOut()->sendNoteOff(getSettings().channel, _currentNote);
        _currentNote = 0;
        _lastDist = 0;
        _triggered = false;
    }
}

#pragma mark - Leg CC

legCC::legCC() : _accumulator(0) {
    
}

void legCC::update(kinectBody &body) {
    kinectSkeleton& sk = body.getLastSkeleton();
    
    vector<ofPoint> acc;
    acc.push_back(body.accel[SKELETOR::Instance()->leftEnumsToIndex[::foot]]);
    acc.push_back(body.accel[SKELETOR::Instance()->leftEnumsToIndex[::knee]]);
    acc.push_back(body.accel[SKELETOR::Instance()->leftEnumsToIndex[::hip]]);
    acc.push_back(body.accel[SKELETOR::Instance()->rightEnumsToIndex[::foot]]);
    acc.push_back(body.accel[SKELETOR::Instance()->rightEnumsToIndex[::knee]]);
    acc.push_back(body.accel[SKELETOR::Instance()->rightEnumsToIndex[::hip]]);
    ofSort(acc, GreatestLength);
    
    float greatest = acc.front().length();
    float amt = ofMap(greatest, 0, sk.skeletonHeight / 32., 0, 1, true);
    _accumulator = ofLerp(_accumulator, amt, 0.07);
    getMidiOut()->sendControlChange(1, 1, _accumulator * 127);
    _time = ofGetElapsedTimef();
    if(getSettings().channel-8 < 0){
       ofNotifyEvent(triggerLinePlayerOne,getSettings().channel, this);
    }else{
        ofNotifyEvent(triggerLinePlayerTwo,getSettings().channel, this);
    }
}

#pragma mark - Stomp

stompNote::stompNote() {
    reset();
}

void stompNote::reset() {
    _primed = false;
    _ignoredFrameCount = 0;
}

void stompNote::update(kinectBody &body) {
    kinectSkeleton& sk = body.getLastSkeleton();
    
    float primeThresh = 0.7;
    float triggerThresh = 0.5;
    int framesToWait = 40; // number of frames to wait for a negative acceleration
    
    size_t idx = SKELETOR::Instance()->getPointIndex(::foot, getSettings().side);
    size_t opp = SKELETOR::Instance()->getPointIndex(::foot, OtherSide(getSettings().side));
    
    float footDiff = ofMap(sk.pts[idx].y - sk.pts[opp].y, 0, sk.skeletonHeight / 10., 0, 1, true);
    
    if(!_primed && footDiff > primeThresh) {
        _primed = true;
        if(getSettings().notes.size() > 1) {
            getMidiOut()->sendNoteOn(getSettings().channel, getSettings().notes[1]);
            _time = ofGetElapsedTimef();
            if(getSettings().channel-8 < 0){
               ofNotifyEvent(triggerLinePlayerOne,getSettings().channel, this);
            }else{
                ofNotifyEvent(triggerLinePlayerTwo,getSettings().channel, this);
            }
        }
    } else if(_primed) {
        
        float acc = body.accel[idx].y;
        
        if(footDiff < triggerThresh) {
            int velocity = ofMap(acc, -0.1, -4, 80, 100);
            getMidiOut()->sendNoteOn(getSettings().channel, getSettings().notes.front(), velocity);
            reset();
            _time = ofGetElapsedTimef();
            if(getSettings().channel-8 < 0){
               ofNotifyEvent(triggerLinePlayerOne,getSettings().channel, this);
            }else{
                ofNotifyEvent(triggerLinePlayerTwo,getSettings().channel, this);
            }
            
        }
    }
}

#pragma mark - Ass Note

dropDatNote::dropDatNote() : _primed(false), _ignoredFrameCount(0) {
    
}

void dropDatNote::reset() {
    _primed = false;
    _ignoredFrameCount = 0;
}

void dropDatNote::update(kinectBody &body) {
    kinectSkeleton& sk = body.getLastSkeleton();
    
    float primeThresh = 0.95;
    float triggerThresh = 0.91;
    
    float left = sk.legLeftExtendedPct;
    float right = sk.legRightExtendedPct;
    bool abovePrime = (left > primeThresh) && (right > primeThresh);
    bool belowTrigger = (left < triggerThresh) && (right < triggerThresh);
    
    if(!_triggered && abovePrime) {
        _triggered = true;
        if(getSettings().notes.size() > 1) {
            getMidiOut()->sendNoteOn(getSettings().channel, getSettings().notes[1]);
            _time = ofGetElapsedTimef();
            if(getSettings().channel-8 < 0){
               ofNotifyEvent(triggerLinePlayerOne,getSettings().channel, this);
            }else{
                ofNotifyEvent(triggerLinePlayerTwo,getSettings().channel, this);
            }
            
        }
    } else if(_triggered && belowTrigger) {
        getMidiOut()->sendNoteOn(getSettings().channel, getSettings().notes.front());
        _time = ofGetElapsedTimef();
        if(getSettings().channel-8 < 0){
           ofNotifyEvent(triggerLinePlayerOne,getSettings().channel, this);
        }else{
            ofNotifyEvent(triggerLinePlayerTwo,getSettings().channel, this);
        }
        _triggered = false;
    }
}
