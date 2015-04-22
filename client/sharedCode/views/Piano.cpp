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
    
    
    globalSettings.setName("Microsoft Music Box");
    globalSettings.add(numPlayers.set("Number of Players", 2, 1, 2));
    renderSettings.setName("Render Settings");
    renderSettings.add(bDrawProjections.set("Draw Projections", true));
    renderSettings.add(projectorViewX.set("Projector X Pos", ofGetScreenWidth(), ofGetScreenWidth(), ofGetScreenWidth()*2));
    renderSettings.add(projectorViewY.set("Projector Y Pos", 0, 0, ofGetScreenHeight()));
    renderSettings.add(projectorViewWidth.set("Projector Screen Width", 1280, 0, 2560));
    renderSettings.add(projectorViewHeight.set("Projector Screen Height", 768, 0, 1440));
    
    renderSettings.add(tvViewX.set("TV X Pos", projectorViewX+projectorViewWidth, ofGetScreenWidth(), ofGetScreenWidth()*3));
    renderSettings.add(tvViewY.set("TV Y Pos", 0, 0, ofGetScreenHeight()));
    renderSettings.add(tvViewWidth.set("TV Screen Width", 1920, 0, 3840));
    renderSettings.add(tvViewHeight.set("TV Screen Height", 1080, 0, 2160));
    
    globalSettings.add(renderSettings);

    

    
    
    projectorRect = ofRectangle(projectorViewX, projectorViewY, projectorViewWidth, projectorViewHeight);
    tvRect = ofRectangle(tvViewX, tvViewY, tvViewWidth, tvViewHeight);
    
    setupViewports();
    iMainView = 4;
    
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->listPorts();
	midiOut->openPort("PIANO");
    
    
    skelView.setup(numPlayers, viewMain);
    midiView.setup(numPlayers, midiOut, viewMain);
    floorView.setup(numPlayers, ofRectangle(ofGetScreenWidth(), 0, 1280, 768), viewMain);
    tvView.setup(numPlayers, ofRectangle(ofGetScreenWidth()+1280, 0, 1920, 1080), viewMain);
    
    
    skeletonGUI.setup(skelView.skeletonTransform);
    stageGUI.setup(skelView.stageParams);
	floorGUI.setup(floorView.projectionParameters);

    globalSettings.add(skelView.hiddenSettings);
    globalSettings.add(tvView.tvParameters);
    globalSettings.add(midiView.midiGroup);
	globalSettings.add(floorView.floorControls);


    skeletonGUI.setSize(250, 400);
    skeletonGUI.setWidthElements(250);
    skeletonGUI.setPosition(ofGetScreenWidth()-250, 0);
    
    stageGUI.setSize(250, 400);
    stageGUI.setWidthElements(250);
    stageGUI.setPosition(ofGetScreenWidth()-500, 0);
    

    appGUI.setup(globalSettings);
    appGUI.setSize(350, appGUI.getHeight());
    appGUI.setWidthElements(350);
    appGUI.minimizeAll();

	floorGUI.loadFromFile("projections.xml");
	stageGUI.loadFromFile("stage.xml");
    skeletonGUI.loadFromFile("skeleton.xml");
    appGUI.loadFromFile("app-settings.xml");

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

    
    bExpertMode = false;
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
       

		tvView.setPlayerOneMode(midiView.playerOne.getInt("Output Mode"));
		tvView.setPlayerTwoMode(midiView.playerTwo.getInt("Output Mode"));

        
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
	stageGUI.saveToFile("stage.xml");
    skeletonGUI.saveToFile("skeleton.xml");
    appGUI.saveToFile("app-settings.xml");
	floorGUI.saveToFile("projections.xml");
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
    }else if(floorView.isMain()){
        floorView.drawDebug();
		floorGUI.draw();
    }else if(tvView.isMain()){
        tvView.draw(viewMain);
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
    
    if(bExpertMode){
        appGUI.draw();
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