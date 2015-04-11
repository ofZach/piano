//
//  MidiView.cpp
//  midiDebug
//
//  Created by dantheman on 3/14/15.
//
//

#include "MidiView.h"

MidiView::MidiView(){
    
}
MidiView::~MidiView(){
    
}

void MidiView::setup(int numPlayers, shared_ptr<ofxMidiOut> midiOut, ofRectangle viewport){
    this->numPlayers = numPlayers;
    this->midiOut = midiOut;
    viewPort = viewport;
    if(this->numPlayers == 1){
        musicMakerP1.setup(midiOut);
        musicMakerP1.playerID = 0;
    }else{
        musicMakerP1.setup(midiOut);
        musicMakerP1.playerID = 0;
        musicMakerP2.setup(midiOut, 8);
        musicMakerP2.playerID = 1;
    }
    setupGUI();
    fontDebug.loadFont(OF_TTF_SANS, 25);
    fontMain.loadFont(OF_TTF_SANS, 50);
    bMainView = false;
}

void MidiView::setPlayerOneMode(int mode){
    if(mode != musicMakerP1.outputMode){
        musicMakerP1.outputMode.set(mode);
    }
}

void MidiView::setPlayerTwoMode(int mode){
    if(mode != musicMakerP2.outputMode){
        musicMakerP2.outputMode.set(mode);
    }
}

void MidiView::update(kinectBody * p1, kinectBody * p2){
    
    if(p1 != NULL){
        lastBodyPlayerOne = ofGetElapsedTimef();
        musicMakerP1.analyze(*p1);
        clearOne = true;
          if(playerOne.getInt("Output Mode") == 1){
        lastTriggerP1 = musicMakerP1.lastDrumTriggeredNote();
    }else{
        lastTriggerP1 = musicMakerP1.lastPianoTriggeredNote();
    }
    }else if(ofGetElapsedTimef() - lastBodyPlayerOne > playerOne.getFloat("Body Drop Time") && clearOne){
        musicMakerP1.clearBodies();
        clearOne = false;
    }
    
    
    if(p2 != NULL){
        lastBodyPlayerTwo = ofGetElapsedTimef();
        musicMakerP2.analyze(*p2);
        clearTwo = true;
		    if(numPlayers == 2 && playerTwo.getInt("Output Mode") == 1){
        lastTriggerP2 = musicMakerP2.lastDrumTriggeredNote();
    }else{
        lastTriggerP2 = musicMakerP2.lastPianoTriggeredNote();
    }
    }else if(ofGetElapsedTimef() - lastBodyPlayerTwo > playerOne.getFloat("Body Drop Time") && clearTwo){
        musicMakerP2.clearBodies();
        clearTwo = false;
        
    }
  
    
  

}
void MidiView::draw(ofRectangle viewport){
    if(isMain()){
        font = fontMain;
    }else{
        font = fontDebug;
    }
    if(musicMakerP1.debugMode){
        ofPushMatrix();
        ofTranslate(325, 0);
        musicMakerP1.drawOverScene();
        ofPopMatrix();
    }
    if(numPlayers > 1){
        if(musicMakerP2.debugMode){
            ofPushMatrix();
            ofTranslate(700, 0);
            musicMakerP2.drawOverScene();
            ofPopMatrix();
        }
    }
    
    
    ofSetColor(ofColor::slateBlue, 200);
    ofRect(viewport.x, viewport.y, viewport.width/2, viewport.height);
    
    ofSetColor(ofColor::white, 255);
    ofRectangle bounding = font.getStringBoundingBox("Player One", 0, 0);
    font.drawString("Player One", viewport.x+(viewport.width/4-bounding.width/2), 100);
    bounding = font.getStringBoundingBox("Note Triggered", 0, 0);
    
    ofSetColor(ofColor::white, 255);
    ofSetColor(ofColor::white, ofMap(ofGetElapsedTimef() - lastTriggerP1.time, 0, 1, 255, 0, true));
    font.drawString("Note Triggered", viewport.x+(viewport.width/4-bounding.width/2), viewport.y+200);
    
    bounding = font.getStringBoundingBox("Output Mode: "+ofToString(musicMakerP1.outputMode), 0, 0);
    ofSetColor(255, 255, 255, 255);
    font.drawString("Output Mode: "+ofToString(musicMakerP1.outputMode), viewport.x+(viewport.width/4-bounding.width/2), viewport.y+300);
    
    ofSetColor(ofColor::slateGray, 200);
    ofRect(viewport.x+viewport.width/2, viewport.y, viewport.width/2, viewport.height);
    bounding = font.getStringBoundingBox("Player One", 0, 0);
    
    if(numPlayers > 1){
        ofSetColor(ofColor::white, 255);
        font.drawString("Player Two", viewport.x+viewport.width/2+(viewport.width/4-bounding.width/2), viewport.y+100);
        bounding = font.getStringBoundingBox("Note Triggered", 0, 0);
        
        ofSetColor(ofColor::white, ofMap(ofGetElapsedTimef() - lastTriggerP2.time, 0, 1, 255, 0, true));
        font.drawString("Note Triggered", viewport.x+viewport.width/2+(viewport.width/4-bounding.width/2), viewport.y+200);
        
        bounding = font.getStringBoundingBox("Output Mode: "+ofToString(musicMakerP2.outputMode), 0, 0);
        
        ofSetColor(255, 255, 255, 255);
        font.drawString("Output Mode: "+ofToString(musicMakerP2.outputMode), viewport.x+viewport.width/2+(viewport.width/4-bounding.width/2), viewport.y+300);
    }
}

bool MidiView::isMain(){
    return bMainView;
}

void MidiView::setMainView(bool mainView){
    bMainView = mainView;
}

void MidiView::setupGUI(){
    playerOne.setName("playerOne");
    
    
    midiGroup.setName("MidiSettings");
    musicMakerP1.addToDebugParamGroup(playerOne);
    playerOne.add(musicMakerP1.buttonControl);
    midiGroup.add(playerOne);
    if(numPlayers == 2){
        playerTwo.setName("playerTwo");
        musicMakerP2.addToDebugParamGroup(playerTwo);
        playerTwo.add(musicMakerP2.buttonControl);
        midiGroup.add(playerTwo);
    }
    
    
}

void MidiView::exit(){
    
}
