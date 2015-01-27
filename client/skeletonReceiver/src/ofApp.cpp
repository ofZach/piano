#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    UDPR.setup();
    
    

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
    gui.add(offset.set("offset", ofPoint(0,0,0), ofPoint(-2000,-2000,-2000), ofPoint(2000,2000,2000)));
    gui.add(rotationX.set("rotationX", 0,-180,180));
    gui.add(rotationY.set("rotationY", 0,-180,180));
    gui.add(rotationZ.set("rotationZ", 0,-180,180));
    gui.add(bUseUdpPlayer.set("use udp player", false));
    gui.add(bLoadNewUDP.set("load udp", false));
    
    gui.add(cameraHeight.set("camera height", 800,0, 2000));
    gui.add(cameraRadius.set("camera radius", 800,0, 2000));
    gui.add(cameraAngle.set("camera angle", 0,-TWO_PI*2, TWO_PI*2));
    
    
    
    gui.loadFromFile("settings.xml");
    
    KS.setup();
    KSA.setup();
 
    fooFbo.allocate(1024, 728, GL_RGBA, 4);
    fooFbo.begin();
    ofClear(0, 0, 0, 0);
    fooFbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    

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

    
    ofPoint position (0 + cameraRadius * cos(cameraAngle), cameraHeight, 0 + cameraRadius * sin(cameraAngle));
    cam.setPosition(position);
    cam.lookAt( ofPoint(0,0,0));
 
    fooFbo.begin();
    ofClear(0, 0, 0);
    cam.begin(ofRectangle(ofVec2f(0, 0), fooFbo.getWidth(), fooFbo.getHeight()));
    cam.begin();
    ofSetColor(255,255,255,127);
    
    ofPushMatrix();
    ofRotate(90,0,0,1);
    ofDrawGridPlane(1000);
    ofPopMatrix();
    ofDrawAxis(50);
    
    ofLine( ofPoint(0,0), ofPoint(800,0));
    

    KS.draw();
    KSA.draw();
    
    cam.end();
    fooFbo.end();
    
    
    fooFbo.draw((ofGetWidth()-fooFbo.getWidth())/2.0, (ofGetHeight()-fooFbo.getHeight())/2.0);
    
    
    gui.draw();
 
    KSA.drawDebug();
    UDPR.draw(ofRectangle(2*ofGetWidth()/3,0, 400, 100));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    
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
