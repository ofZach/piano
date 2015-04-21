//
//  Piano.cpp
//  sceneView
//
//  Created by dantheman on 3/14/15.
//
//

#include "Piano.h"


Piano::Piano(){
    
}
Piano::~Piano(){
    
}

void Piano::setupViewports(){
    int xOffset = viewMain.width / 2;
    int yOffset = viewMain.height / 2;
    int x = 0;
    int y = 0;
    
    for(int i = 0; i < N_SCENE; i++){
        viewGrid[i].x = x;
        viewGrid[i].y = y;
        viewGrid[i].width = xOffset;
        viewGrid[i].height = yOffset;
        
        if(i == 0){
            x = xOffset;
            y = 0;
        }
        if(i == 1){
            x = 0;
            y = yOffset;
        }
        if(i == 2){
            x = xOffset;
            y = yOffset;
        }
    }
}
void Piano::setup(){
    viewMain = ofRectangle(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
    
    
    settings.globalSettings.setName("Microsoft Music Box");
    settings.globalSettings.add(settings.numPlayers.set("Number of Players", 2, 1, 2));
    
    settings.globalSettings.add(settings.projectorViewX.set("Projector X Pos", ofGetScreenWidth(), ofGetScreenWidth(), ofGetScreenWidth()*2));
    settings.globalSettings.add(settings.projectorViewY.set("Projector Y Pos", 0, 0, ofGetScreenHeight()));
    settings.globalSettings.add(settings.projectorViewWidth.set("Projector Screen Width In Pixels", 1280, 640, 1920));
    settings.globalSettings.add(settings.projectorViewHeight.set("Projector Screen Height In Pixel", 768, 480, 1080));
    
    settings.globalSettings.add(settings.tvViewX.set("TV X Pos", settings.projectorViewX+settings.projectorViewWidth, ofGetScreenWidth(), ofGetScreenWidth()*3));
    settings.globalSettings.add(settings.tvViewY.set("TV Y Pos", 0, 0, ofGetScreenHeight()));
    settings.globalSettings.add(settings.tvViewWidth.set("TV Screen Width In Pixels", 1920, 1280, 1920));
    settings.globalSettings.add(settings.tvViewHeight.set("TV Screen Height In Pixel", 1080, 720, 1200));
    
    
    
    appGUI.setup(settings.globalSettings);
    appGUI.setSize(350, 250);
    appGUI.setWidthElements(350);
    appGUI.loadFromFile("app-settings.xml");
    
    projectorRect = ofRectangle(settings.projectorViewX, settings.projectorViewY, settings.projectorViewWidth, settings.projectorViewHeight);
    tvRect = ofRectangle(settings.tvViewX, settings.tvViewY, settings.tvViewWidth, settings.tvViewHeight);
    
    setupViewports();
    iMainView = 4;
    
    numPlayers = settings.numPlayers;
    
    
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->listPorts();
	midiOut->openPort("PIANO");
    
    
    skelView.setup(numPlayers, viewMain);
    midiView.setup(numPlayers, midiOut, viewMain);
    floorView.setup(numPlayers, ofRectangle(ofGetScreenWidth(), 0, 1280, 768), viewMain);
    tvView.setup(numPlayers, ofRectangle(ofGetScreenWidth()+1280, 0, 1920, 1080), viewMain);
    
    
    skeletonGUI.setup(skelView.skeletonTransform);
    stageGUI.setup(skelView.stageParams);
    hiddenSettings.setup(skelView.hiddenSettings);
//    projectionGUI.setup(floorView.squareOptions);
    tvGUI.setup(tvView.tvParameters);
    midiGUI.setup(midiView.midiGroup);
    
    midiGUI.setSize(250, 400);
	midiGUI.setWidthElements(250);

    skeletonGUI.setSize(250, 400);
    skeletonGUI.setWidthElements(250);
    skeletonGUI.setPosition(ofGetScreenWidth()-250, 0);
    
    stageGUI.setSize(250, 400);
    stageGUI.setWidthElements(250);
    stageGUI.setPosition(ofGetScreenWidth()-500, 0);
    
    
    stageGUI.loadFromFile("stage.xml");
    skeletonGUI.loadFromFile("skeleton.xml");
    hiddenSettings.loadFromFile("hiddensettings.xml");
 
    tvGUI.loadFromFile("tvView.xml");
    midiGUI.loadFromFile("midi.xml");
    
    

	for(int i = 0; i < midiView.musicMakerP1.midiTriggers.size(); i++){
		ofAddListener( midiView.musicMakerP1.midiTriggers[i]->triggerImplusePlayerOne, this, &Piano::addImpulsePlayerOne);
		ofAddListener( midiView.musicMakerP1.midiTriggers[i]->triggerLinePlayerOne, this, &Piano::addLineTracePlayerOne);
	}

	for(int i = 0; i < midiView.musicMakerP2.midiTriggers.size(); i++){
		ofAddListener( midiView.musicMakerP2.midiTriggers[i]->triggerImplusePlayerTwo, this, &Piano::addImpulsePlayerTwo);
		ofAddListener( midiView.musicMakerP2.midiTriggers[i]->triggerLinePlayerTwo, this, &Piano::addLineTracePlayerTwo);
	}
    

	ofAddListener(midiView.musicMakerP1.triggerImplusePlayerOne, this, &Piano::addImpulsePlayerOne);
	ofAddListener(midiView.musicMakerP1.triggerLinesPlayerOne, this, &Piano::addLineTracePlayerOne);
	ofAddListener(midiView.musicMakerP2.triggerImplusePlayerTwo, this, &Piano::addImpulsePlayerTwo);
	ofAddListener(midiView.musicMakerP2.triggerLinesPlayerTwo, this, &Piano::addLineTracePlayerTwo);

}
void Piano::update(){
    
    skelView.update();
    floorView.update();
    
    if(numPlayers <= 1){
        kinectBody * kb1 = skelView.getBody(0);
        midiView.update(kb1, NULL);
        tvView.update(kb1, NULL);
        if(kb1 != NULL){
            floorView.setPlayerOne(true);
        }else{
            floorView.setPlayerOne(false);
        }
        if (midiView.playerOne.getBool("ableToChangeModeManually") == false){
            midiView.setPlayerOneMode(skelView.getPlayerOneMode());
        }
        
        tvView.setPlayerOneMode(midiView.playerOne.getInt("Output Mode"));
        skelView.setButtonPos(floorView.getPlayerOneButtonPos(), floorView.getPlayerOneButtonPos());
        
    }else{
        kinectBody * kb1 = skelView.getBody(0);
        kinectBody * kb2 = skelView.getBody(1);
        midiView.update(kb1, kb2);
        tvView.update(kb1, kb2);
        
        
        skelView.setButtonPos(floorView.getPlayerOneButtonPos(), floorView.getPlayerTwoButtonPos());
        
        
        if (midiView.playerOne.getBool("ableToChangeModeManually") == false){
            midiView.setPlayerOneMode(skelView.getPlayerOneMode());
        }
        
        if (midiView.playerTwo.getBool("ableToChangeModeManually") == false){
            midiView.setPlayerTwoMode(skelView.getPlayerTwoMode());
        }
       

		tvView.setPlayerOneMode(skelView.getPlayerOneMode());
		tvView.setPlayerTwoMode(skelView.getPlayerTwoMode());

        
        if(kb1 != NULL){
            floorView.setPlayerOne(true);
        }else{
            floorView.setPlayerOne(false);
        }
        
        if(kb2 != NULL){
            floorView.setPlayerTwo(true);
        }else{
            floorView.setPlayerTwo(false);
        }
        
        
        
    }
    
    tvView.setStageParameters(skelView.stageParams);
    
    
}

void Piano::exit(){
    skeletonGUI.saveToFile("skeleton.xml");
    stageGUI.saveToFile("stage.xml");
//    projectionGUI.saveToFile("projection.xml");
    tvGUI.saveToFile("tvView.xml");
    midiGUI.saveToFile("midi.xml");
    appGUI.saveToFile("app-settings.xml");
    hiddenSettings.saveToFile("hiddensettings.xml");
	
	midiView.exit();
}


void Piano::draw(){
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    
    if(iMainView == N_SCENE){
        ofSetColor(255, 255, 255, 255);
        skelView.draw(viewGrid[0]);
        midiView.draw(viewGrid[1]);
        floorView.draw(viewGrid[2]);
        tvView.draw(viewGrid[3]);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetScreenWidth()-50, ofGetScreenHeight()-50);
    }
    
    if(skelView.isMain()){
        skelView.draw(viewMain);
        skeletonGUI.draw();
        stageGUI.draw();
    }else if(midiView.isMain()){
        midiView.draw(viewMain);
        midiGUI.draw();
    }else if(floorView.isMain()){
        floorView.drawDebug();
    }else if(tvView.isMain()){
        tvView.draw(viewMain);
        tvGUI.draw();
        skeletonGUI.draw();
        stageGUI.draw();
    }
    
    floorView.drawProjections();
    tvView.drawTV();
    ofDisableAlphaBlending();
    
    if(iMainView == 4){
        ofSetColor(255, 0, 0);
        ofRect(viewMain);
        ofSetColor(0, 255, 0);
        ofRect(projectorRect);
        ofSetColor(0, 0, 255);
        ofRect(tvRect);
        ofSetColor(255, 255, 255);
        ofLine(viewMain.x, viewMain.y, viewMain.x+viewMain.width, viewMain.y+viewMain.height);
        ofLine(viewMain.x+viewMain.width, viewMain.y, viewMain.x, viewMain.y+viewMain.height);
        ofLine(projectorRect.x, projectorRect.y, projectorRect.x+projectorRect.width, projectorRect.y+projectorRect.height);
        ofLine(projectorRect.x+projectorRect.width, projectorRect.y, projectorRect.x, projectorRect.y+projectorRect.height);
		ofLine(tvRect.x, tvRect.y, tvRect.x+tvRect.width, tvRect.y+tvRect.height);
        ofLine(tvRect.x+tvRect.width, tvRect.y, tvRect.x, tvRect.y+tvRect.height);
    }
    
}

void Piano::addLineTracePlayerOne(int & i){
    floorView.addLineTrace(0);
}
void Piano::triggerTrianglePlayerOne(int & i){
//    floorView.floors[0].triggerTriangles();
}
void Piano::addImpulsePlayerOne(int & i){
    tvView.addPulsePlayerOne();
}

void Piano::addLineTracePlayerTwo(int & i){
    floorView.addLineTrace(1);
}
void Piano::triggerTrianglePlayerTwo(int & i){
//    floorView.p2Floor.triggerTriangles();
    
}
void Piano::addImpulsePlayerTwo(int & i){
    tvView.addPlusePlayerTwo();
}


void Piano::setMainView(int i){
    iMainView = i;
    if(iMainView == 0){
        skelView.setMainView(true);
    }else{
        skelView.setMainView(false);
    }
    if(iMainView == 1){
        midiView.setMainView(true);
    }else{
        midiView.setMainView(false);
    }
    if(iMainView == 2){
        floorView.setMainView(true);
    }else{
        floorView.setMainView(false);
    }
    if(iMainView == 3){
        tvView.setMainView(true);
    }else{
        tvView.setMainView(false);
    }
}
ofRectangle* Piano::getViews(){
    return &viewGrid[0];
}