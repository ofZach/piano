#include "ofApp.h"


void ofApp::exit(){
    MM.exit();
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_SILENT);
    
    //UDPR.setup();
    setupGUI();
    
    ofTrueTypeFont smallFont, largeFont;
    
    smallFont.loadFont("selena.otf", 16); //http://openfontlibrary.org/en/font/selena
    largeFont.loadFont("selena.otf", 48);
    
    
    kinect.setup(12345, smallFont);
    kinect.setSmoothing(SIMPLE_MOVING_AVERAGE);
    skeletons = kinect.getSkeletons();
    renderer.setup(skeletons, largeFont);
    
    
    
    ///Moving midiOut to the OF App because there can be only one midiOut!!
    midiOut = shared_ptr<ofxMidiOut>(new ofxMidiOut);
    midiOut->openVirtualPort("OF Kinect");
    
    KS.setup();
    TV.setup();
    MM.setup(midiOut);
    MM2.setup(midiOut, 8);
    MM.TV = &TV;
    MM2.TV = &TV;
    floorProjections.setup();
    
    allocateFbos();
    

    bodyDropTimer = ofGetElapsedTimeMillis();
    
        TV.setStagePos(stageX, stageY, stageZ, stageSize);
    
    
    debugCam.setPosition(0, 500, -1000);
    debugCam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
    
    
}



void ofApp::setupGUI(){
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
    
    skeletonTransform.add(centerX.set("centerX", 0, -2000,2000));
    skeletonTransform.add(centerY.set("centerY", 0, -2000,2000));
    skeletonTransform.add(centerZ.set("centerZ", 0, -2000,2000));
    
    skeletonTransform.add(buttonX.set("buttonX", 0, -2000,2000));
    skeletonTransform.add(buttonY.set("buttonY", 0, -2000,2000));
    skeletonTransform.add(buttonZ.set("buttonZ", 0, -2000,2000));
    skeletonTransform.add(buttonApproach.set("buttonApproach", 0, 1.0,2.0));
    skeletonTransform.add(buttonTriggerScale.set("buttonTriggerScale", 0, 0.1,2.0));
    skeletonTransform.add(buttonRadius.set("buttonRadius", 10, 1,50));
    
    
    
    dataPlayer.setName("dataPlayer");
    dataPlayer.add(bUseUdpPlayer.set("use udp player", false));
    dataPlayer.add(bLoadNewUDP.set("load udp", false));
    dataPlayer.add(udpDuration.set("Playback Pos", 0, 0, 1));
    
    cameraControl.setName("camera control");
    cameraControl.add(cameraHeight.set("camera height", 800,0, 2000));
    cameraControl.add(cameraRadius.set("camera radius", 800,0, 2000));
    cameraControl.add(cameraAngle.set("camera angle", 0,-TWO_PI*2, TWO_PI*2));
    cameraControl.add(swayCamera.set("sway camera", true));
    cameraControl.add(swayAmount.set("sway amount", 0.5, 0, 1));
    cameraControl.add(swayRate.set("sway rate", 0.5, 0, 1));
    
    debugView.setName("DebugView");
    debugView.add(drawSkeleton.set("Draw Skeleton", true));
    debugView.add(drawAnalyzer.set("Draw Analyzer", true));
    debugView.add(drawBoundingCube.set("Draw Bounding Cube", true));
    debugView.add(stageX.set("Stage X", 0, -1000, 1000));
    debugView.add(stageY.set("Stage Y", 0, -1000, 1000));
    debugView.add(stageZ.set("Stage Z", 0, -1000, 1000));
    debugView.add(stageSize.set("Stage Size", 0, 0, 1000));
    
    playerOne.setName("playerOne");
    playerTwo.setName("playerTwo");
    MM.addToDebugParamGroup(playerOne);
    MM2.addToDebugParamGroup(playerTwo);
    
    
    gui.setup("main control", ofGetWidth()-300-10, 10, 300, ofGetHeight());
    gui.addPanel("Debug Controls", 4, false);
    gui.addPanel("Player 1 Controls", 4, false);
    gui.addPanel("Thresholds Skeletons Player 1", 4, false);
    gui.addPanel("Thresholds History Player 1", 4, false);
    gui.addPanel("Player 2 Controls", 4, false);
    gui.addPanel("Thresholds Skeletons Player 2", 4, false);
    gui.addPanel("Thresholds History Player 2", 4, false);
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    
    gui.addGroup(skeletonTransform);
    gui.addGroup(dataPlayer);
    gui.addGroup(cameraControl);
    
    gui.setWhichPanel(1);
    gui.setWhichColumn(0);
    gui.addGroup(debugView);
    
    gui.setWhichPanel(2);
    gui.setWhichColumn(0);
    gui.addGroup(playerOne);
    
    
    gui.setWhichPanel(3);
    gui.setWhichColumn(0);
    gui.addGroup(MM.graphsControl1);
    
    gui.setWhichPanel(4);
    gui.setWhichColumn(0);
    gui.addGroup(MM.graphsControl2);
    
    gui.setWhichPanel(5);
    gui.setWhichColumn(0);
    gui.addGroup(playerTwo);
    
    gui.setWhichPanel(6);
    gui.setWhichColumn(0);
    gui.addGroup(MM2.graphsControl2);
    
    gui.setWhichPanel(7);
    gui.setWhichColumn(0);
    gui.addGroup(MM2.graphsControl2);
    
    
    
    
    
    status = "first frame";
    gui.setStatusMessage(status);
    
    gui.loadSettings("settings.xml");
    gui.setupEvents();
    gui.enableEvents();
    
    gui.loadSettings("settings.xml");
    
}

void ofApp::allocateFbos(){
    debugFbo.allocate(ofGetWindowWidth()/2, ofGetWindowHeight()/2, GL_RGBA);
    debugFbo.begin();
    ofClear(0, 0, 0, 0);
    debugFbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    oscWorkhorse();
    updateGUI();
    
    
    
    //update the camera
    ofPoint position (0 + cameraRadius * cos(cameraAngle), cameraHeight, 0 + cameraRadius * sin(cameraAngle));
    
    if(swayCamera) {
        float t = ofGetElapsedTimef() * swayRate;
        ofVec3f swayDir(ofNoise(t * 0.9), ofNoise(t * 0.89), ofNoise(t * 0.88));
        position += swayDir * (swayAmount * 100.);
    }
    
    cam.setPosition(position);
    cam.lookAt( ofPoint(0,0,0));
    


    
    if (skeletons->size() >= 1){
        int index = 0;
        float dist = FLT_MAX;
        
        //        for(int i = 0; i < skeletons->size(); i++){
        //            if(((centerPoint-skeletons->at(i).getSpineBase().getPoint()).length()) < dist){
        //                index = i;
        //                dist = ((centerPoint-skeletons->at(i).getSpineBase().getPoint()).length());
        //            }
        //        }
        
        for(int i = 0; i < skeletons->size(); i++){
            KS.setFromSkeleton(skeletons->at(i), mat);
            kinectBody & body = bodyMap[skeletons->at(i).getBodyId()];
            bool bNewFrame = body.addSkeleton(KS);
            
//            cout<<i<<endl;
            if (bNewFrame){
                KSA.analyze(body.getLastSkeleton());
                KBA.analyze(body);
                if(i == 0){
//                    cout<<"update player one"<<endl;
                    MM.analyze(body);
                }else{
//                    cout<<"update player two"<<endl;
                    MM2.analyze(body);
                }
                TV.update(&body);
                
                //switch mode
                //                int whichLeft = SKELETOR::Instance()->getPointIndex(ankle, ::left);
                //                int whichRight = SKELETOR::Instance()->getPointIndex(ankle, ::right);
                //                vector<std::pair<ofPoint, float> > feet;
                //                feet.push_back(make_pair(body.getLastSkeleton().pts[whichLeft], 0.f));
                //                feet.push_back(make_pair(body.getLastSkeleton().pts[whichRight], 0.f));
                //                switchMode.update(feet);
            }
        }
    }else {
        
        TV.update(NULL);
        
        
        if(ofGetElapsedTimeMillis() - bodyDropTimer > MM.bodyDropThreshold){
            
            
            if(bodyMap.size() > 0){
                bodyMap.clear();
                MM.clearBodies();
                MM2.clearBodies();
            }
            
            bodyDropTimer = ofGetElapsedTimeMillis();
        }
    }
    
    
    TV.drawIntoFbo(cam);

}

void ofApp::updateGUI(){
    gui.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    TV.setStagePos(stageX, stageY, stageZ, stageSize);


    centerPoint.set(centerX, centerY, centerZ);
    centerButton.setPosition(centerX, centerY, centerZ);
    
    switchMode.setRadius(buttonRadius);
    switchMode.setApproachScale(buttonApproach);
    switchMode.setTriggerScale(buttonTriggerScale);
    switchMode.setPosition(buttonX, buttonY, buttonZ);
    
    
    mat.makeIdentityMatrix();
    ofPoint offsetPt = ofPoint(offsetX, offsetY, offsetZ);
    mat.glTranslate(offsetPt);
    mat.glRotate(rotationX, 1,0,0);
    mat.glRotate(rotationY, 0,1,0);
    mat.glRotate(rotationZ, 0,0,1);
    ofPoint scaleTemp = ofPoint(scaleX, scaleY, scaleZ);;
    mat.glScale(scaleTemp.x, scaleTemp.y, scaleTemp.z);
}

void ofApp::oscWorkhorse(){
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
    kinect.update();

}



//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0, 0, 0);
    
    ofEnableAlphaBlending();
    debugFbo.begin();
    ofClear(0, 0, 0);
    ofBackground(0, 0, 0);
    debugCam.begin(ofRectangle(ofVec2f(0, 0), debugFbo.getWidth(), debugFbo.getHeight()));
    ofPushStyle();
    ofSetColor(255,255,255,127);
    ofSetLineWidth(3);
    ofPushMatrix();
    ofRotate(90,0,0,1);
    ofDrawGridPlane(1000);
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofRotate(90, 1, 0, 0);
    ofSetColor(0, 100, 100, 200);
    ofTranslate(stageX-stageSize/2.0, stageY-stageSize/2.0, stageZ);
    ofRect(0, 0, stageSize, stageSize);
    ofPopMatrix();
    ofSetColor(255,255,255,127);
    ofDrawAxis(50);

    
    for( vector<Skeleton>::iterator iter = skeletons->begin(); iter != skeletons->end(); ++iter){
        ofPushStyle();
        bodyMap[iter->getBodyId()].draw();
        ofPopStyle();
    }

    ofPopStyle();
    debugCam.end();
    
    
    
    ofClearAlpha();
    debugFbo.end();
    ofSetLineWidth(1);
    
    ofSetColor(255,255,255);
    debugFbo.draw(0, 0, debugFbo.getWidth(), debugFbo.getHeight());


    TV.draw(ofRectangle(ofGetWindowWidth()/2,0,TV.tvSkeletonView.getWidth()/2, TV.tvSkeletonView.getHeight()/2));
    floorProjections.floor.draw(0, ofGetWindowHeight()/2, floorProjections.floor.getWidth()/2, floorProjections.floor.getHeight()/2);
    
    
    for( vector<Skeleton>::iterator iter = skeletons->begin(); iter != skeletons->end(); ++iter){
        bodyMap[iter->getBodyId()].drawHistory();
    }

    MM.drawOverScene();
    MM2.drawOverScene();
    
    gui.draw();
    

    ofDisableAlphaBlending();
    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's'){
        gui.saveSettings("settings.xml");
    }
    if(key == 'k'){
        gui.toggleView();
    }
    
    if (key == ' '){
        TV.addImpluse();
    }
    
    if(key == '1'){
        MM.outputMode.set(0);
    }
    if(key == '2'){
        MM.outputMode.set(1);
    }
    if(key == '3'){
        MM2.outputMode.set(0);
    }
    if(key == '4'){
        MM2.outputMode.set(1);
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
    allocateFbos();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
