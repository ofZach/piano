#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    UDPR.setup();
    
    
    
    ofTrueTypeFont smallFont, largeFont;
    
    smallFont.loadFont("selena.otf", 16); //http://openfontlibrary.org/en/font/selena
    largeFont.loadFont("selena.otf", 48);
    
    
    ofSetFrameRate(60);
    kinect.setup(12345, smallFont);
    kinect.setSmoothing(SIMPLE_MOVING_AVERAGE);
    skeletons = kinect.getSkeletons();
    renderer.setup(skeletons, largeFont);
    
    gui.setup("panel");

    KS.setup();
    
    //KSAI.setup();
    
    
    skeletonTransform.setName("skeleton transform");
    skeletonTransform.add(scaleX.set("scaleX", 1.0,0.01, 20));
    skeletonTransform.add(scaleY.set("scaleY", 1.0,0.01, 20));
    skeletonTransform.add(scaleZ.set("scaleZ", 1.0,0.01, 140));
    skeletonTransform.add(offsetX.set("offsetX", 0, -2000,2000));
    skeletonTransform.add(offsetY.set("offsetY", 0, -2000,2000));
    skeletonTransform.add(offsetZ.set("offsetZ", 0, -2000,2000));
    skeletonTransform.add(rotationX.set("rotationX", 0,-180,180));
    skeletonTransform.add(rotationY.set("rotationY", 0,-180,180));
    skeletonTransform.add(rotationZ.set("rotationZ", 0,-180,180));
    
    dataPlayer.setName("dataPlayer");
    dataPlayer.add(bUseUdpPlayer.set("use udp player", false));
    dataPlayer.add(bLoadNewUDP.set("load udp", false));
    dataPlayer.add(udpDuration.set("Playback Pos", 0, 0, 1));
    
    cameraControl.setName("camera control");
    cameraControl.add(cameraHeight.set("camera height", 800,0, 2000));
    cameraControl.add(cameraRadius.set("camera radius", 800,0, 2000));
    cameraControl.add(cameraAngle.set("camera angle", 0,-TWO_PI*2, TWO_PI*2));
    
    
    debugView.setName("DebugView");
    debugView.add(drawSkeleton.set("Draw Skeleton", true));
    debugView.add(drawAnalyzer.set("Draw Analyzer", true));
    debugView.add(drawBoundingCube.set("Draw Bounding Cube", true));
    debugView.add(startNote.set("Start Note", 36, 0, 127));
    debugView.add(numNotes.set("Num Note", 16, 0, 127));
    debugView.add(smoothUpHistory.set("Smooth Up History", 0, 0, 1));
    debugView.add(smoothDownHistory.set("Smooth Down History", 0, 0, 1));
    debugView.add(smoothDownSkeleton.set("Smooth Down Skeleton", 0, 0, 1));
    debugView.add(smoothUpSkeleton.set("Smooth Up Skeleton", 0, 0, 1));
    debugView.add(jazzDrums.set("JazzDrums", false));
    debugView.add(historyAndSkeleton.set("Send All Triggers", false));
    
    
    
    buttonControl.setName("button control");
    buttonControl.add(buttonDraw.set("Draw Buttons", false));
    buttonControl.add(buttonRadius.set("Radius", 75, 20, 150));
    buttonControl.add(buttonTriggerScale.set("Trigger Scale", 0.8, 0.3, 1.0));
    buttonControl.add(buttonApproachScale.set("Approach Scale", 1.2, 0.5, 2.0));
    
    
    gui.setup("controls", ofGetWidth()-300-10, 10, 300, 700);
    gui.addPanel("main control", 4, false);
    gui.addPanel("analysis", 4, false);
    gui.addPanel("Debug Controls", 4, false);
    gui.addPanel("Thresholds Skeletons", 4, false);
    gui.addPanel("Thresholds History", 4, false);
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    
    gui.addGroup(skeletonTransform);
    gui.addGroup(dataPlayer);
    gui.addGroup(cameraControl);

    
    
    gui.setWhichPanel(3);
    gui.setWhichColumn(0);
    
    //gui.addGroup(KSA.anlaysisParams);
    //gui.addGroup(KSAI.interpreterParams);
    
    gui.addGroup(debugView);
    
    
    gui.setWhichPanel(2);
    gui.setWhichColumn(0);
    gui.addGroup(buttonControl);
    
    gui.setWhichPanel(0);
    
    status = "first frame";
    gui.setStatusMessage(status);
    
    gui.loadSettings("settings.xml");
    gui.setupEvents();
    gui.enableEvents();
    
    
    //ofAddListener(kinect.newGesture, this, &ofApp::newGesture);
    
    
    
    fooFbo.allocate(1024, 728, GL_RGBA, 4);
    fooFbo.begin();
    ofClear(0, 0, 0, 0);
    fooFbo.end();
    
    setupAudio();
    
    
    for (int i = 0; i < 4; i++){
        graphs.push_back(Graph());
        graphs.back().setup(ofToString(i));
        graphs.back().setSize(100, 50);
        graphs.back().setMinMaxRange(0, 1);
        graphs.back().setBidirectional(true);
        graphs.back().setThreshold(0);
    }
    
    
    
    
    graphs[0].setName("kick left");
    graphs[1].setName("kick right");
    graphs[2].setName("punch left");
    graphs[3].setName("punch right");
    
    
    for (int i = 0; i < 25; i++){
        graphsForSkeleton.push_back(Graph());
        graphsForSkeleton.back().setup(ofToString(i));
        graphsForSkeleton.back().setBidirectional(true);
        graphsForSkeleton.back().setSize(100, 20);
        graphsForSkeleton.back().setThreshold(0);
        ofParameter<float> fooParam;
        fooParam.set("Threshold-"+ofToString(i), 0, 0, 100);
        graphsSkeletonThresh.push_back(fooParam);
        graphsControl1.add(graphsSkeletonThresh.back());
        
    }
    gui.setWhichPanel(4);
    gui.setWhichColumn(0);
    gui.addGroup(graphsControl1);
    
    for (int i = 0; i < 25; i++){
        graphsHistory.push_back(Graph());
        graphsHistory.back().setup(ofToString(i));
        graphsHistory.back().setSize(100, 20);
        graphsHistory.back().setThreshold(0);
        graphsHistory.back().setBidirectional(true);
        graphsHistory.back().setMinMaxRange(0, 1);
        ofParameter<float> fooParam;
        fooParam.set("History Threshold-"+ofToString(i), 0, 0, 100);
        graphsHistoryThresh.push_back(fooParam);
        graphsControl2.add(graphsHistoryThresh.back());
    }
    
    gui.setWhichPanel(5);
    gui.setWhichColumn(0);
    gui.addGroup(graphsControl2);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    gui.update();
    
    
    
    
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    if (bLoadNewUDP == true){
        string folder = ofSystemLoadDialog("", true).getPath();
        if (folder != ""){
            UDPR.parseFolder(folder);
        }
        bLoadNewUDP = false;
    }
    
    if (bUseUdpPlayer){
        UDPR.update();
        udpDuration.set(UDPR.pct);
    }
    
    mat.makeIdentityMatrix();
    ofPoint offsetPt = ofPoint(offsetX, offsetY, offsetZ);
    mat.glTranslate(offsetPt);
    mat.glRotate(rotationX, 1,0,0);
    mat.glRotate(rotationY, 0,1,0);
    mat.glRotate(rotationZ, 0,0,1);
    ofPoint scaleTemp = ofPoint(scaleX, scaleY, scaleZ);;
    mat.glScale(scaleTemp.x, scaleTemp.y, scaleTemp.z);
    
    
    kinect.update();
    
    
    //ofLog(OF_LOG_NOTICE) << skeletons->size() << endl;
    //
    
    
    if (skeletons->size() >= 1){
        for(int i = 0; i < skeletons->size(); i++){
            KS.setFromSkeleton(skeletons->at(i), mat);
            kinectBody & body = bodyMap[skeletons->at(i).getBodyId()];
            bool bNewFrame = body.addSkeleton(KS);
            
            
            if (bNewFrame){
                KSA.analyze(body.getLastSkeleton());
                KBA.analyze(body);
                
                //                for (int i = 0; i < body.velocity.size(); i++){
                //                    graphsForSkeleton[i].addSample(body.velLen[i]);
                //                }
                
                for (int i = 0; i < body.historyPlots.size(); i++){
                    if(body.historyPlots[i]->getValues().size()> 0){
                        graphsHistory[i].addSample(body.historyPlots[i]->getValues().back());
                    }
                    
                    if (graphsHistory[i].getTriggered() && body.historyPlots[i]->getValues().size()> 0 && (!jazzDrums || historyAndSkeleton)){
                        midi.updateSequencerStep(startNote + i%numNotes, ofMap(body.historyPlots[i]->getValues().back(), body.historyPlots[i]->getLowerRange(), body.historyPlots[i]->getHigerRange(), 0, 127, true));
                    }
                    graphsHistory[i].setSmoothing(smoothDownHistory, smoothUpHistory);
                    graphsHistory[i].setMinMaxRange(body.historyPlots[i]->getLowerRange(), body.historyPlots[i]->getHigerRange());
                    graphsHistoryThresh[i].set(graphsHistory[i].threshold);
                    
                }
                
                for (int i = 0; i < graphsForSkeleton.size(); i++){
                    
                    graphsForSkeleton[i].addSample(body.velLen[i]);
                    if (graphsForSkeleton[i].getTriggered() && (jazzDrums || historyAndSkeleton)){
                        midi.updateSequencerStep(startNote + (i%numNotes), ofMap(graphsForSkeleton[i].getNormalized(), 0, 1, 0, 127, true));
                    };
                    graphsForSkeleton[i].setSmoothing(smoothDownHistory, smoothUpHistory);
                    graphsSkeletonThresh[i].set(graphsForSkeleton[i].threshold);
                }
                
                
                
                
                
                updateAudio(body);
                
                if(body.gestureHistory.size() > 0){
                    int count = 12;
                    int i = 0;
                    
                    int counter = 0;
                    graphs[0].addSample(body.gestureHistory.back()["kick_Left"].value);
                    graphs[1].addSample(body.gestureHistory.back()["kick_Right"].value);
                    graphs[2].addSample(body.gestureHistory.back()["punch_Left"].value);
                    graphs[3].addSample(body.gestureHistory.back()["punch_Right"].value);
                    
                    
                    //                    if((body.gestureHistory.back()["kick_Left"].triggered || body.gestureHistory.back()["kick_Left"].value > 0.75) && !triggers["kick_Left"] ){
                    //                        triggers["kick_Left"] = true;
                    //                        //midi.updateSequencerStep(12, 127);
                    //                    }else if(!body.gestureHistory.back()["kick_Left"].triggered && triggers["punch_Left"]){
                    //
                    //                        triggers["kick_Left"] = false;
                    //                    }
                    //
                    //                    if((body.gestureHistory.back()["kick_Right"].triggered || body.gestureHistory.back()["kick_Right"].value > 0.75) && !triggers["kick_Right"] ){
                    //                        triggers["kick_Right"] = true;
                    //                        //midi.updateSequencerStep(12, 127);
                    //                    }else if(!body.gestureHistory.back()["kick_Right"].triggered && triggers["punch_Left"]){
                    //
                    //                        triggers["kick_Right"] = false;
                    //                    }
                    //
                    //                    if((body.gestureHistory.back()["punch_Left"].triggered || body.gestureHistory.back()["punch_Left"].value > 0.75) && !triggers["punch_Left"] ){
                    //                        triggers["punch_Left"] = true;
                    //                        //midi.updateSequencerStep(15, 127);
                    //                    }else if(!body.gestureHistory.back()["punch_Left"].triggered && triggers["punch_Left"]){
                    //                        triggers["punch_Left"] = false;
                    //                    }
                    //
                    //                    if((body.gestureHistory.back()["punch_Right"].triggered || body.gestureHistory.back()["punch_Right"].value > 0.75) && !triggers["punch_Right"] ){
                    //                        triggers["punch_Right"] = true;
                    //                        //midi.updateSequencerStep(15, 127);
                    //                    }else if(!body.gestureHistory.back()["punch_Right"].triggered && triggers["punch_Left"]){
                    //                        triggers["punch_Right"] = false;
                    //                    }
                    
                    
                    for (int i = 0; i < graphs.size(); i++){
                        if (graphs[i].getTriggered()){
                            // midi.updateSequencerStep(12 + i, 127);
                        }
                    }
                    
                    
                    
                    
                    
                    
                }
            }
        }
    }else{
        bodyMap.clear();
        midi.clear();
        for(int i = 0; i < graphsForSkeleton.size(); i++){
            graphsForSkeleton[i].clear();
            graphsForSkeleton[i].setup(ofToString(i));
            
        }
        
    }
    
    //KSAI.drawEvents( KSA.normFbo);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(ofColor::darkGray);
    
    
    ofPoint position (0 + cameraRadius * cos(cameraAngle), cameraHeight, 0 + cameraRadius * sin(cameraAngle));
    cam.setPosition(position);
    cam.lookAt( ofPoint(0,0,0));
    
    fooFbo.begin();
    ofBackground(ofColor::darkGray);
    //ofClear(0, 0, 0);
    cam.begin(ofRectangle(ofVec2f(0, 0), fooFbo.getWidth(), fooFbo.getHeight()));
    ofSetColor(255,255,255,127);
    ofSetLineWidth(3);
    ofPushMatrix();
    ofRotate(90,0,0,1);
    ofDrawGridPlane(1000);
    ofPopMatrix();
    ofDrawAxis(50);
    
    ofLine( ofPoint(0,0), ofPoint(800,0));
    
    
    for( vector<Skeleton>::iterator iter = skeletons->begin(); iter != skeletons->end(); ++iter){
        if(drawSkeleton){
            bodyMap[iter->getBodyId()].draw();
        }
        if(drawAnalyzer){
            bodyMap[iter->getBodyId()].drawDebug(drawBoundingCube);
        }
    }
    
    
    
    if(buttonDraw) {
        for(auto& button : buttons) {
            button.draw();
        }
    }
    
    cam.end();
    
    
    
    ofClearAlpha();
    fooFbo.end();
    ofSetLineWidth(1);
    
    ofSetColor(255,255,255);
    fooFbo.draw((ofGetWidth()-fooFbo.getWidth())/2.0, (ofGetHeight()-fooFbo.getHeight())/2.0);
    for( vector<Skeleton>::iterator iter = skeletons->begin(); iter != skeletons->end(); ++iter){
        bodyMap[iter->getBodyId()].drawHistory();
    }
    
    gui.draw();
    midi.draw();
    
    for (int i = 0; i < 4; i++){
        graphs[i].draw(350, i * 50);
    }
    
    for (int i = 0; i < graphsForSkeleton.size(); i++){
        graphsForSkeleton[i].draw(150, i*25);
    }
    
    for (int i = 0; i < graphsHistory.size(); i++){
        graphsHistory[i].draw(0, i*25);
    }
    //    UDPR.draw(ofRectangle(2*ofGetWidth()/3,0, 400, 100));
}

void ofApp::exit() {
    // turn off all MIDI notes, "All notes off" message is unreliable
    for(int channel = 1; channel <= 16; channel++) {
        for(int note = 0; note <= 127; note++) {
            midiOut->sendNoteOff(channel, note);
        }
    }
}

#define END(a) (a + (sizeof(a) / sizeof(a[0])))

void ofApp::setupAudio() {
	midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
	midiOut->openVirtualPort("OF Skeleton Tracker");
	
	int stringNotes[] = {53, 59, 60, 65, 67, 72};
	int pianoNotes[] = {48, 53, 55, 59, 60, 64, 65, 67, 72, 77, 79, 84};
	
	
	triggerRef strings = triggerRef(new gridNote);
	midiTrigger::Settings stringSettings;
	stringSettings.notes.assign(stringNotes, END(stringNotes));
	stringSettings.channel = 4;
	stringSettings.side = ::right;
	strings->setSettings(stringSettings);
	
	triggerRef piano = triggerRef(new gridNote);
	midiTrigger::Settings pianoSettings;
	pianoSettings.notes.assign(pianoNotes, END(pianoNotes));
	pianoSettings.channel = 5;
	pianoSettings.side = ::left;
	piano->setSettings(pianoSettings);
	
	triggerRef accord = triggerRef(new accordianNote);
	midiTrigger::Settings accordSettings;
	accordSettings.channel = 8;
	accord->setSettings(accordSettings);
	
	midiTriggers.push_back(strings);
	midiTriggers.push_back(piano);
	midiTriggers.push_back(accord);
	
	for(auto& t : midiTriggers) {
		t->setMidiOut(midiOut);
	}
	
	midi.setup();
}

#undef END

void ofApp::updateAudio(kinectBody &body) {
    for(auto& t : midiTriggers) {
        t->update(body);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's'){
        gui.saveSettings("settings.xml");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
