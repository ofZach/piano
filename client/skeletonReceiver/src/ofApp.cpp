#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    UDPR.setup();
    
    string jointNames[26] =
    {   "ThumbRight",
        "SpineBase",
        "SpineMid",
        "Neck",
        "Head",
        "ShoulderLeft",
        "ElbowLeft",
        "WristLeft",
        "HandLeft",
        "ShoulderRight",
        "ElbowRight",
        "WristRight",
        "HandRight",
        "HipLeft",
        "KneeLeft",
        "AnkleLeft",
        "FootLeft",
        "HipRight",
        "KneeRight",
        "AnkleRight",
        "FootRight",
        "SpineShoulder",
        "HandTipLeft",
        "ThumbLeft",
        "HandTipRight"};
    
    
    for (int i = 0; i < 26; i++){
        cout << "pts[nameToIndex[\"" << jointNames[i] << "\"]].set(sk.get" << jointNames[i] << "().getPoint())" << endl;
    }
    
    
    
    
    ofTrueTypeFont smallFont, largeFont;
    
    smallFont.loadFont("selena.otf", 16); //http://openfontlibrary.org/en/font/selena
    largeFont.loadFont("selena.otf", 48);
    
    ofSetLineWidth(8);
    ofSetFrameRate(60);
    kinect.setup(12345, smallFont);
    kinect.setSmoothing(SIMPLE_MOVING_AVERAGE);
    skeletons = kinect.getSkeletons();
    renderer.setup(skeletons, largeFont);
    
    gui.setup("panel");
    
    gui.add(scale.set("scale", ofPoint(1,1,1), ofPoint(0.01,0.01,0.01), ofPoint(140,140,140)));
    gui.add(offset.set("offset", ofPoint(0,0,0), ofPoint(-1000,-1000,-1000), ofPoint(1000,1000,1000)));
    gui.add(rotationX.set("rotationX", 0,-180,180));
    gui.add(rotationY.set("rotationY", 0,-180,180));
    gui.add(rotationZ.set("rotationZ", 0,-180,180));
    gui.add(bUseUdpPlayer.set("use udp player", false));
    gui.add(bLoadNewUDP.set("load udp", false));
    
    
    
    
    gui.loadFromFile("settings.xml");
    
    
    cam.disableMouseMiddleButton();
    
    KS.setup();
    KSA.setup();
    
    
    armleft = new ofxHistoryPlot( NULL, "arm-left", 100, false);
    armright = new ofxHistoryPlot( NULL, "arm-right", 100, false);
    footLeft = new ofxHistoryPlot( NULL, "foot-left", 100, false);
    footRight = new ofxHistoryPlot( NULL, "foot-right", 100, false);
    
    leftHandHip = new ofxHistoryPlot( NULL, "hand-to-hip-left", 100, false);
    rightHandHip = new ofxHistoryPlot( NULL, "hand-to-hip-right", 100, false);
    
    
    armleft->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    armleft->setColor( ofColor(0,255,0) ); //color of the plot line
    armleft->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    armleft->setRespectBorders(true);	   //dont let the plot draw on top of text
    armleft->setLineWidth(1);				//plot line width
    armleft->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    armleft->setDrawGrid(true);
    armleft->setGridColor(ofColor(30)); //grid lines color
    armleft->setGridUnit(14);
    
    
    armright->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    armright->setColor( ofColor(0,255,0) ); //color of the plot line
    armright->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    armright->setRespectBorders(true);	   //dont let the plot draw on top of text
    armright->setLineWidth(1);				//plot line width
    armright->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    armright->setDrawGrid(true);
    armright->setGridColor(ofColor(30)); //grid lines color
    armright->setGridUnit(14);
    
    
    footLeft->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    footLeft->setColor( ofColor(0,255,0) ); //color of the plot line
    footLeft->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    footLeft->setRespectBorders(true);	   //dont let the plot draw on top of text
    footLeft->setLineWidth(1);				//plot line width
    footLeft->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    footLeft->setDrawGrid(true);
    footLeft->setGridColor(ofColor(30)); //grid lines color
    footLeft->setGridUnit(14);
    
    
    
    footRight->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    footRight->setColor( ofColor(0,255,0) ); //color of the plot line
    footRight->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    footRight->setRespectBorders(true);	   //dont let the plot draw on top of text
    footRight->setLineWidth(1);				//plot line width
    footRight->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    footRight->setDrawGrid(true);
    footRight->setGridColor(ofColor(30)); //grid lines color
    footRight->setGridUnit(14);
    
    
    leftHandHip->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    leftHandHip->setColor( ofColor(0,255,0) ); //color of the plot line
    leftHandHip->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    leftHandHip->setRespectBorders(true);	   //dont let the plot draw on top of text
    leftHandHip->setLineWidth(1);				//plot line width
    leftHandHip->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    leftHandHip->setDrawGrid(true);
    leftHandHip->setGridColor(ofColor(30)); //grid lines color
    leftHandHip->setGridUnit(14);
    
    rightHandHip->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    rightHandHip->setColor( ofColor(0,255,0) ); //color of the plot line
    rightHandHip->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    rightHandHip->setRespectBorders(true);	   //dont let the plot draw on top of text
    rightHandHip->setLineWidth(1);				//plot line width
    rightHandHip->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    rightHandHip->setDrawGrid(true);
    rightHandHip->setGridColor(ofColor(30)); //grid lines color
    rightHandHip->setGridUnit(14);
    
    
    center = new ofxHistoryPlot( NULL, "Center", 100, false);
    
    center->setRange(0, 1); //hard range, will not adapt to values off-scale
    //plot->addHorizontalGuide(ofGetHeight()/2, ofColor(255,0,0)); //add custom reference guides
    center->setColor( ofColor(0,255,0) ); //color of the plot line
    center->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    center->setRespectBorders(true);	   //dont let the plot draw on top of text
    center->setLineWidth(1);				//plot line width
    center->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    center->setDrawGrid(true);
    center->setGridColor(ofColor(30)); //grid lines color
    center->setGridUnit(14);
    
 
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (bLoadNewUDP == true){
        string folder = ofSystemLoadDialog("", true).getPath();
        if (folder != ""){
            UDPR.parseFolder(folder);
        }
        bLoadNewUDP = false;
    }
    
    if (bUseUdpPlayer){
        UDPR.update();
    }
    
    
    
    mat.makeIdentityMatrix();
    ofPoint offsetPt = offset;
    mat.glTranslate(offsetPt);
    mat.glRotate(rotationX, 1,0,0);
    mat.glRotate(rotationY, 0,1,0);
    mat.glRotate(rotationZ, 0,0,1);
    ofPoint scaleTemp = scale;
    mat.glScale(scaleTemp.x, scaleTemp.y, scaleTemp.z);
    
    
    
    kinect.update();
    
    if (skeletons->size() >= 1){
        KS.setFromSkeleton(skeletons->at(0), mat);
        KSA.analyze(KS);
        armright->update(KSA.armRightExtendedPct);
        armleft->update(KSA.armLeftExtendedPct);
        footLeft->update(KSA.footLeftExtendedPct);
        footRight->update(KSA.footRightExtendedPct);
        
        leftHandHip->update(KSA.leftHandVHip);
        rightHandHip->update(KSA.rightHandVHip);
        center->update(KSA.diffCenter);
        
    }
    
    if (ofGetFrameNum() < 10){
        cam.setPosition(698.314, 1647.53, -28.9273);
        cam.setOrientation(ofPoint(-112.963, -90.1957, 1.08879));
    }
    
    // there is no kinect is frame new...
    
    if (skeletons->size() >= 1){
        
        ofPoint rightHand = skeletons->at(0).getHandRight().getPoint() * mat;
        ofPoint leftHand = skeletons->at(0).getHandLeft().getPoint() * mat;
        
        leftHandHistory.addVertex(leftHand);
        rightHandHistory.addVertex(rightHand);
        
        if (leftHandHistory.size() > 100){
            leftHandHistory.getVertices().erase(leftHandHistory.getVertices().begin());
        }
        
        if (rightHandHistory.size() > 100){
            rightHandHistory.getVertices().erase(rightHandHistory.getVertices().begin());
        }
        
    } else {
        
        leftHandHistory.clear();
        rightHandHistory.clear();
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(ofColor::darkGray);
    
    
    cam.begin();
    
    ofSetColor(255,255,255,127);
    
    ofPushMatrix();
    ofRotate(90,0,0,1);
    ofDrawGridPlane(1000);
    ofPopMatrix();
    ofDrawAxis(50);
    
    ofLine( ofPoint(0,0), ofPoint(800,0));
    
    
    KS.draw();
    
    
    cam.end();
    
    gui.draw();
    
    armright->draw(0, 400);
    armleft->draw(0, 500);
    footLeft->draw(0, 600);
    footRight->draw(0, 700);
    
    leftHandHip->draw(200, 400);
    rightHandHip->draw(200, 500);
    center->draw(200, 600);
    
    
    UDPR.draw(ofRectangle(ofGetWidth()/2,0, 400, 100));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' '){
        cam.setPosition(698.314, 1647.53, -28.9273);
        cam.setOrientation(ofPoint(-112.963, -90.1957, 1.08879));
    }
    
    
    if (key == 's'){
        gui.saveToFile("settings.xml");
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
