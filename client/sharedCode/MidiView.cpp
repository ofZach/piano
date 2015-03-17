//
//  midiView.cpp
//  midiDebug
//
//  Created by dantheman on 3/14/15.
//
//

#include "midiView.h"

midiView::midiView(){
    
}
midiView::~midiView(){
    
}

void midiView::setup(int numPlayers, shared_ptr<ofxMidiOut> midiOut, ofRectangle viewport){
    this->numPlayers = numPlayers;
    this->midiOut = midiOut;
    viewPort = viewport;
    if(this->numPlayers == 1){
        musicMakerP1.setup(midiOut);
    }else{
        musicMakerP1.setup(midiOut);
        musicMakerP2.setup(midiOut);
    }
    setupGUI();
    fontDebug.loadFont(OF_TTF_SANS, 25);
    fontMain.loadFont(OF_TTF_SANS, 50);
    bMainView = false;
}
void midiView::update(kinectBody * playerOne, kinectBody * playerTwo){
    midiControlPanel.update();
    if(sendMidiTest){
        musicMakerP1.skeletonMidi.triggerKick(62, 255);
        sendMidiTest = false;
    }
    if(numPlayers == 1){
        musicMakerP1.analyze(*playerOne);
    }else{
        if(playerOne != NULL)
            musicMakerP1.analyze(*playerOne);
        if(playerTwo != NULL)
            musicMakerP2.analyze(*playerTwo);
    }
    
    lastTriggerP1 = musicMakerP1.lastTriggeredNote();
    lastTriggerP2 = musicMakerP2.lastTriggeredNote();
}
void midiView::draw(ofRectangle viewport){
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
    
    
    ofSetColor(ofColor::slateGray, 200);
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
    
    ofSetColor(ofColor::slateBlue, 200);
    ofRect(viewport.x+viewport.width/2, viewport.y, viewport.width/2, viewport.height);
    bounding = font.getStringBoundingBox("Player One", 0, 0);
    
    ofSetColor(ofColor::white, 255);
    font.drawString("Player Two", viewport.x+viewport.width/2+(viewport.width/4-bounding.width/2), viewport.y+100);
    bounding = font.getStringBoundingBox("Note Triggered", 0, 0);
    
    ofSetColor(ofColor::white, ofMap(ofGetElapsedTimef() - lastTriggerP2.time, 0, 1, 255, 0, true));
    font.drawString("Note Triggered", viewport.x+viewport.width/2+(viewport.width/4-bounding.width/2), viewport.y+200);
    
    bounding = font.getStringBoundingBox("Output Mode: "+ofToString(musicMakerP2.outputMode), 0, 0);
    
    ofSetColor(255, 255, 255, 255);
    font.drawString("Output Mode: "+ofToString(musicMakerP2.outputMode), viewport.x+viewport.width/2+(viewport.width/4-bounding.width/2), viewport.y+300);
}
bool midiView::isMain(){
    return bMainView;
}
void midiView::setMainView(bool mainView){
    bMainView = mainView;
}

void midiView::drawControlPanel(){
    if(isMain()){
        midiControlPanel.draw();
    }
}
void midiView::setupGUI(){
    playerOne.setName("playerOne");
    
    musicMakerP1.addToDebugParamGroup(playerOne);
    playerOne.add(sendMidiTest.set("Send Test", false));
    midiControlPanel.setup("Player One", 0, ofGetHeight()-500, 300, 500, true, false);
    midiControlPanel.addPanel("Player Two");
    midiControlPanel.setWhichPanel(0);
    midiControlPanel.setWhichColumn(0);
    midiControlPanel.addGroup(playerOne);
    if(numPlayers == 2){
        playerTwo.setName("playerTwo");
        musicMakerP2.addToDebugParamGroup(playerTwo);
        midiControlPanel.setWhichPanel(1);
        midiControlPanel.setWhichColumn(0);
        midiControlPanel.addGroup(playerTwo);
    }
    midiControlPanel.loadSettings("midi-settings.xml");
}

void midiView::exit(){
    midiControlPanel.saveSettings("midi-settings.xml");
}
