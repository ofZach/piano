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
    
    //gui.setup("panel");
    
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
	
	buttonControl.setName("button control");
	buttonControl.add(buttonRadius.set("Radius", 75, 20, 150));
	buttonControl.add(buttonTriggerScale.set("Trigger Scale", 0.8, 0.3, 1.0));
    
    gui.setup("controls", ofGetWidth()-300-10, 10, 300, 700);
    gui.addPanel("main control", 4, false);
    gui.addPanel("analysis", 4, false);
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    
    gui.addGroup(skeletonTransform);
    gui.addGroup(dataPlayer);
    gui.addGroup(cameraControl);
    gui.addGroup(debugView);

    
    gui.setWhichPanel(1);
    gui.setWhichColumn(0);
    
    //gui.addGroup(KSA.anlaysisParams);
    //gui.addGroup(KSAI.interpreterParams);
    
	
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
	
	midiOut.openVirtualPort("OF Skeleton Tracker");
	
	buttons.resize(8 * 4);
	
	midiNotes.push_back(0);
	midiNotes.push_back(2);
	midiNotes.push_back(4);
	midiNotes.push_back(5);
	midiNotes.push_back(7);
	midiNotes.push_back(9);
	midiNotes.push_back(11);
	
	int midiRoot = 24; // C2
	
	for(int i = 0; i < buttons.size(); i++) {
		buttons[i].setTriggerBlock(^(bool on, float vel) {
			
			int octave = i / midiNotes.size();
			int interval = i % midiNotes.size();
			int note = midiNotes[interval] + (octave * 12) + midiRoot;
			int velocity = ofMap(vel, 0, 1, 40, 120);
			
			if(on) {
				midiOut.sendNoteOn(1, note, velocity);
			} else {
				midiOut.sendNoteOff(1, note, velocity);
			}
		});
	}
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
    
    if (skeletons->size() >= 1){
        KS.setFromSkeleton(skeletons->at(0), mat);
        bool bNewFrame = KB.addSkeleton(KS);
        
        if (bNewFrame){
            KSA.update(KS);
			
			float spacing = ofClamp(KS.shouldersWidth, 140, 175);
			vector<ofPoint> hands;
			hands.push_back(KS.getRightPoint(::hand));
			hands.push_back(KS.getLeftPoint(::hand));
			
			for(int i = 0; i < buttons.size(); i++) {
				
				int interval = i % 8;
				int octave = i / 8;
				
				float t = ofMap(interval, 0, 7, M_PI_2, M_PI + M_PI_2);
				
				float x = sin(t);
				float z = cos(t);
				float y = ofMap(octave, 0, 3, 0.2, 1);
				
				buttons[i].setParent(KS.centerPoint);
				buttons[i].setPosition(x * spacing, y * spacing, z * spacing);
				
				buttons[i].setRadius(buttonRadius);
				buttons[i].setTriggerScale(buttonTriggerScale);
				buttons[i].update(hands);
			}
        }
        //KSAI.analyze(KSA, KS);
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
    cam.begin();
    ofSetColor(255,255,255,127);
    ofSetLineWidth(3);
    ofPushMatrix();
    ofRotate(90,0,0,1);
    ofDrawGridPlane(1000);
    ofPopMatrix();
    ofDrawAxis(50);
    
    ofLine( ofPoint(0,0), ofPoint(800,0));
    
    if(drawSkeleton){
        KS.draw();
    }
    if(drawAnalyzer){
        KS.drawDebug(drawBoundingCube);
    }
	
	for(auto& button : buttons) {
		button.draw();
	}
	
    cam.end();
    
    
    
    ofClearAlpha();
    fooFbo.end();
    ofSetLineWidth(1);
    
    fooFbo.draw((ofGetWidth()-fooFbo.getWidth())/2.0, (ofGetHeight()-fooFbo.getHeight())/2.0);
    //KSA.drawDebug();
    
    gui.draw();
 
    
    
    

//    UDPR.draw(ofRectangle(2*ofGetWidth()/3,0, 400, 100));
}

//void ofApp::newGesture(Gesture &newGest){
//    ofLog()<<newGest.name<<endl;
//}

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
