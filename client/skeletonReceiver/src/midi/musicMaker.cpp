//
//  musicMaker.cpp
//  skeletonReceiver
//
//  Created by zach on 2/5/15.
//
//

#include "musicMaker.h"
#include "ofApp.h"

void musicMaker::setup (){
	setupGraphs();
	setupMidiTriggers();
    
    buttonControl.setName("button control");
    buttonControl.add(buttonDraw.set("Draw Buttons", false));
    buttonControl.add(buttonRadius.set("Radius", 75, 20, 150));
    buttonControl.add(buttonTriggerScale.set("Trigger Scale", 0.8, 0.3, 1.0));
    buttonControl.add(buttonApproachScale.set("Approach Scale", 1.2, 0.5, 2.0));
}

void musicMaker::setupGraphs() {
	for (int i = 0; i < 4; i++){
		graphs.push_back(Graph());
		graphs.back().setup(ofToString(i));
		graphs.back().setSize(100, 50);
		graphs.back().setMinMaxRange(0, 1);
		graphs.back().setBidirectional(false);
		graphs.back().setThreshold(0);
	}
	
	graphs[0].setName("kick left");
	graphs[1].setName("kick right");
	graphs[2].setName("punch left");
	graphs[3].setName("punch right");
	
	
	for (int i = 0; i < 25; i++){
		graphsForSkeleton.push_back(Graph());
		graphsForSkeleton.back().setup(ofToString(i));
		graphsForSkeleton.back().setBidirectional(false);
		graphsForSkeleton.back().setSize(100, 20);
		graphsForSkeleton.back().setThreshold(0);
		ofParameter<float> fooParam;
		fooParam.set("Threshold-"+ofToString(i), 0, 0, 100);
		graphsSkeletonThresh.push_back(fooParam);
		graphsControl1.add(graphsSkeletonThresh.back());
		
	}
	
	for (int i = 0; i < 25; i++){
		graphsHistory.push_back(Graph());
		graphsHistory.back().setup(ofToString(i));
		graphsHistory.back().setSize(100, 20);
		graphsHistory.back().setThreshold(0);
		graphsHistory.back().setBidirectional(false);
		graphsHistory.back().setMinMaxRange(0, 1);
		ofParameter<float> fooParam;
		fooParam.set("History Threshold-"+ofToString(i), 0, 0, 100);
		graphsHistoryThresh.push_back(fooParam);
		graphsControl2.add(graphsHistoryThresh.back());
	}
}

// TODO: less verbose
void musicMaker::setupMidiTriggers() {
	
	int stringNotes[] = {53, 59, 60, 64, 67, 72};
	int pianoNotes[] = {41, 43, 48, 53, 55, 59, 60, 62, 67, 72, 77, 79, 86};
	int accordNotes[] = {36, 43};
 
#define END(a) (a + (sizeof(a) / sizeof(a[0])))
	
	triggerRef strings = triggerRef(new gridNote);
	midiTrigger::Settings stringSettings;
	stringSettings.notes.assign(stringNotes, END(stringNotes));
	stringSettings.channel = 5;
	stringSettings.side = ::right;
	strings->setSettings(stringSettings);
	
	triggerRef piano = triggerRef(new gridNote);
	midiTrigger::Settings pianoSettings;
	pianoSettings.notes.assign(pianoNotes, END(pianoNotes));
	pianoSettings.channel = 4;
	pianoSettings.side = ::left;
	piano->setSettings(pianoSettings);
	
	triggerRef accord = triggerRef(new accordianNote);
	midiTrigger::Settings accordSettings;
	accordSettings.channel = 8;
	accordSettings.notes.assign(accordNotes, END(accordNotes));
	accord->setSettings(accordSettings);
	
	triggerRef legs = triggerRef(new legCC);
	
	triggerRef stompLeft = triggerRef(new stompNote);
	midiTrigger::Settings stompLeftSettings;
	stompLeftSettings.side = ::left;
	stompLeftSettings.notes.push_back(50);
	stompLeftSettings.channel = 10;
	stompLeft->setSettings(stompLeftSettings);
	
	triggerRef stompRight = triggerRef(new stompNote);
	midiTrigger::Settings stompRightSettings;
	stompRightSettings.side = ::right;
	stompRightSettings.notes.push_back(56);
	stompRightSettings.channel = 10;
	stompRight->setSettings(stompRightSettings);
	
	triggerRef drop = triggerRef(new dropDatNote);
	midiTrigger::Settings dropSettings;
	dropSettings.notes.push_back(66);
	dropSettings.channel = 9;
	drop->setSettings(dropSettings);
	
	triggerRef drumStompLeft = triggerRef(new stompNote);
	midiTrigger::Settings drumpStompLeftSettings;
	drumpStompLeftSettings.channel = 9;
	drumpStompLeftSettings.side = ::left;
	drumpStompLeftSettings.notes.push_back(66);
	drumStompLeft->setSettings(drumpStompLeftSettings);
	
	triggerRef drumStompRight = triggerRef(new stompNote);
	midiTrigger::Settings drumStompRightSettings;
	drumStompRightSettings.channel = 9;
	drumStompRightSettings.side = ::right;
	drumStompRightSettings.notes.push_back(67);
	drumStompRight->setSettings(drumStompRightSettings);
	
#undef END
	
	midiTriggers.push_back(strings);
	midiTriggers.push_back(piano);
	midiTriggers.push_back(accord);
	midiTriggers.push_back(legs);
	midiTriggers.push_back(stompLeft);
	midiTriggers.push_back(stompRight);
	
	drumMidiTriggers.push_back(drumStompLeft);
	drumMidiTriggers.push_back(drumStompRight);
	drumMidiTriggers.push_back(drop);
	
	midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
	midiOut->openVirtualPort("OF Kinect");
	
	for(auto& t : midiTriggers) {
		t->setMidiOut(midiOut);
	}
	for(auto& t : drumMidiTriggers) {
		t->setMidiOut(midiOut);
	}

	skeletonMidi.setup(midiOut);
}

void musicMaker::addToDebugParamGroup ( ofParameterGroup & debugView){
    debugView.add(startNote.set("Start Note", 36, 0, 127));
    debugView.add(numNotes.set("Num Note", 16, 0, 127));
    debugView.add(smoothUpHistory.set("Smooth Up History", 0, 0, 1));
    debugView.add(smoothDownHistory.set("Smooth Down History", 0, 0, 1));
    debugView.add(smoothDownSkeleton.set("Smooth Down Skeleton", 0, 0, 1));
    debugView.add(smoothUpSkeleton.set("Smooth Up Skeleton", 0, 0, 1));
    debugView.add(jazzDrums.set("JazzDrums", false));
    debugView.add(historyAndSkeleton.set("Send All Triggers", false));
    debugView.add(debugMode.set("Draw Graphs", false));
    debugView.add(bodyDropThreshold.set("Body Drop Time", 500, 100, 2000));
    debugView.add(outputMode.set("Output", 0, 0, 2));
	outputMode.addListener(this, &musicMaker::outputmodeChanged);
}

void musicMaker::drawInScene(){
    if(buttonDraw) {
        for(auto& button : buttons) {
            button.draw();
        }
    }
}

void musicMaker::drawOverScene(){
    if(debugMode){
        
        for (int i = 0; i < 4; i++){
            graphs[i].draw(350, i * 50);
        }
        
        for (int i = 0; i < graphsForSkeleton.size(); i++){
            graphsForSkeleton[i].draw(150, i*25);
        }
        
        for (int i = 0; i < graphsHistory.size(); i++){
            graphsHistory[i].draw(0, i*25);
        }
    }
}

void musicMaker::analyze (kinectBody & body) {
	
    if(outputMode == 0){
        updateMidiTriggers(body);
    }else{
        if(outputMode == 1){
            jazzDrums.set(false);
        }else{
            jazzDrums.set(true);
        }
		updateGraphs(body);
    }
}


void musicMaker::exit() {
    AllNotesOff(*midiOut);
    midiOut->closePort();
}

void musicMaker::updateMidiTriggers(kinectBody &body) {
    for(auto& t : midiTriggers) {
        t->update(body);
    }
}

void musicMaker::updateGraphs(kinectBody &body) {

	for (int i = 0; i < body.historyPlots.size(); i++){
		if(body.historyPlots[i]->getValues().size()> 0){
			graphsHistory[i].addSample(body.historyPlots[i]->getValues().back());
		}
		
		if (graphsHistory[i].getTriggered() && body.historyPlots[i]->getValues().size()> 0 ){
			
			if((!jazzDrums || historyAndSkeleton)){
				if(!triggers[body.historyPlots[i]->varName]){
					
					if(ofIsStringInString(body.historyPlots[i]->varName, "arm")){
						skeletonMidi.triggerNote(62+i%2, ofMap(body.historyPlots[i]->getValues().back(),
															   body.historyPlots[i]->getLowerRange(),
															   body.historyPlots[i]->getHigerRange(),
															   0, 127, true));
						triggers[body.historyPlots[i]->varName] = true;
					}
					if(ofIsStringInString(body.historyPlots[i]->varName, "foot-head")){
						skeletonMidi.triggerNote(i%2==0?77:78, ofMap(body.historyPlots[i]->getValues().back(),
																	 body.historyPlots[i]->getLowerRange(),
																	 body.historyPlots[i]->getHigerRange(),
																	 0, 127, true));
						triggers[body.historyPlots[i]->varName] = true;
					}
					if(ofIsStringInString(body.historyPlots[i]->varName, "elbow-angle")){
						skeletonMidi.triggerNote(i%2==0?72:73, ofMap(body.historyPlots[i]->getValues().back(),
																	 body.historyPlots[i]->getLowerRange(),
																	 body.historyPlots[i]->getHigerRange(),
																	 0, 127, true));
						triggers[body.historyPlots[i]->varName] = true;
					}
					if(ofIsStringInString(body.historyPlots[i]->varName, "knee-angle")){
						skeletonMidi.triggerNote(i%2==0?64:65, ofMap(body.historyPlots[i]->getValues().back(),
																	 body.historyPlots[i]->getLowerRange(),
																	 body.historyPlots[i]->getHigerRange(),
																	 0, 127, true));
						triggers[body.historyPlots[i]->varName] = true;
					}
					if(ofIsStringInString(body.historyPlots[i]->varName, "hand-hip")){
						skeletonMidi.triggerNote(i%2==0?79:80, ofMap(body.historyPlots[i]->getValues().back(),
																	 body.historyPlots[i]->getLowerRange(),
																	 body.historyPlots[i]->getHigerRange(),
																	 0, 127, true));
						triggers[body.historyPlots[i]->varName] = true;
					}
					
					if(triggers[body.historyPlots[i]->varName]){
						((ofApp*)ofGetAppPtr())->floorProjections.addLineTrace();
					}
					
				}
			}
		}else if(triggers[body.historyPlots[i]->varName]){
			triggers[body.historyPlots[i]->varName] = false;
		}
		graphsHistory[i].setSmoothing(smoothDownHistory, smoothUpHistory);
		graphsHistory[i].setMinMaxRange(body.historyPlots[i]->getLowerRange(), body.historyPlots[i]->getHigerRange());
		graphsHistoryThresh[i].set(graphsHistory[i].threshold);
	}
	
	for (int i = 0; i < graphsForSkeleton.size(); i++){
		if(find(SKELETOR::Instance()->skipList.begin(), SKELETOR::Instance()->skipList.end(),SKELETOR::Instance()->indexToName[i]) == SKELETOR::Instance()->skipList.end()){
            
            string nameOfPt = SKELETOR::Instance()->indexToName[i];
            
			graphsForSkeleton[i].addSample(body.velLen[i]);
			if (graphsForSkeleton[i].getTriggered()){
				if((jazzDrums || historyAndSkeleton)){
					if(!ofIsStringInString(nameOfPt, "Spine")){
						if(!triggers[nameOfPt]){
							
							if(ofIsStringInString(nameOfPt, "Hand")){
								skeletonMidi.triggerNote(i%2==0?70:71, graphsForSkeleton[i].getNormalized()*127);
								triggers[nameOfPt] = true;
							}
							if(ofIsStringInString(nameOfPt, "Hip")){
								skeletonMidi.triggerNote(i%2?68:69, graphsForSkeleton[i].getNormalized()*127);
								triggers[nameOfPt] = true;
							}
							if(ofIsStringInString(nameOfPt, "Foot")){
								skeletonMidi.triggerNote(i%2?64:65, graphsForSkeleton[i].getNormalized()*127);
								triggers[nameOfPt] = true;
							}
							if(ofIsStringInString(nameOfPt, "Knee")){
								skeletonMidi.triggerNote(i%2?66:67, graphsForSkeleton[i].getNormalized()*127);
								triggers[nameOfPt] = true;
							}
							if(ofIsStringInString(nameOfPt, "Neck")){
								skeletonMidi.triggerNote(77, graphsForSkeleton[i].getNormalized()*127);
								triggers[nameOfPt] = true;
							}
							
							if(triggers[nameOfPt]){
								ofLog(OF_LOG_NOTICE)<<"Trigger "<<nameOfPt<<endl;
								((ofApp*)ofGetAppPtr())->floorProjections.triggerTriangles();
							}
						}
					}
					
				}
				
			}else if(triggers[nameOfPt]){
				triggers[nameOfPt] = false;
			}
			graphsForSkeleton[i].setSmoothing(smoothDownHistory, smoothUpHistory);
			graphsSkeletonThresh[i].set(graphsForSkeleton[i].threshold);
			
		}
	}
	
	if(body.gestureHistory.size() > 0){
		graphs[0].addSample(body.gestureHistory.back()["kick_Left"].value);
		graphs[1].addSample(body.gestureHistory.back()["kick_Right"].value);
		graphs[2].addSample(body.gestureHistory.back()["punch_Left"].value);
		graphs[3].addSample(body.gestureHistory.back()["punch_Right"].value);
		
		
		if(graphs[0].getTriggered()  && !triggers["kick_Left"]){
			skeletonMidi.triggerKick(60, graphs[0].getNormalized()*127);
			((ofApp*)ofGetAppPtr())->floorProjections.triggerTriangles();
			((ofApp*)ofGetAppPtr())->floorProjections.addLineTrace();
			triggers["kick_Left"] = true;
		}else if(!graphs[0].getTriggered()  && triggers["kick_Left"]){
			triggers["kick_Left"] = false;
		}
		if(graphs[1].getTriggered()  && !triggers["kick_Right"]){
			skeletonMidi.triggerKick(61, graphs[1].getNormalized()*127);
			((ofApp*)ofGetAppPtr())->floorProjections.triggerTriangles();
			((ofApp*)ofGetAppPtr())->floorProjections.addLineTrace();
		}else if(!graphs[1].getTriggered()  && triggers["kick_Right"]){
			triggers["kick_Right"] = false;
		}
		if(graphs[2].getTriggered()&& !triggers["punch_Left"]){
			skeletonMidi.triggerPunch(62, graphs[2].getNormalized()*127);
			((ofApp*)ofGetAppPtr())->floorProjections.triggerTriangles();
			((ofApp*)ofGetAppPtr())->floorProjections.addLineTrace();
		}else if(!graphs[2].getTriggered()  && triggers["punch_Left"]){
			triggers["punch_Left"] = false;
		}
		if(graphs[3].getTriggered() && !triggers["punch_Right"]){
			skeletonMidi.triggerPunch(63, graphs[3].getNormalized()*127);
			((ofApp*)ofGetAppPtr())->floorProjections.triggerTriangles();
			((ofApp*)ofGetAppPtr())->floorProjections.addLineTrace();
		}else if(!graphs[3].getTriggered() && triggers["punch_Right"]){
			triggers["punch_Right"] = false;
		}
		
		graphs[0].setSmoothing(smoothDownHistory, smoothUpHistory);
		graphs[1].setSmoothing(smoothDownHistory, smoothUpHistory);
		graphs[2].setSmoothing(smoothDownHistory, smoothUpHistory);
		graphs[3].setSmoothing(smoothDownHistory, smoothUpHistory);
	}
	
	for(auto& trig : drumMidiTriggers) {
		trig->update(body);
	}
}

void musicMaker::clearBodies(){
    for(int i = 0; i < graphsForSkeleton.size(); i++){
        graphsForSkeleton[i].clear();
		
    }
	
    for(int i = 0; i < graphsHistory.size(); i++){
        graphsHistory[i].clear();
		
    }
	
    for(int i = 0; i < graphs.size(); i++){
        graphs[i].clear();
		
    }
	
	for(auto& t : midiTriggers) {
		t->reset();
	}
	
	for(auto& t : drumMidiTriggers) {
		t->reset();
	}
}

void musicMaker::outputmodeChanged(int &mode) {
	for(auto& trig : midiTriggers) {
		trig->reset();
	}
}
