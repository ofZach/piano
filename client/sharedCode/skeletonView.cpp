//
//  skeletonView.cpp
//  kinectDebug
//
//  Created by dantheman on 3/14/15.
//
//

#include "skeletonView.h"

skeletonView::skeletonView(){
    
}
skeletonView::~skeletonView(){
    
}
void skeletonView::setup(int numPlayer, ofRectangle viewPort){
    kinect.setup(12345);
    kinect.setSmoothing(SIMPLE_MOVING_AVERAGE);
    skeletons = kinect.getSkeletons();
    KS.setup();
    setupGUI();
    cam.setPosition(0, 500, -1000);
    cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
    

    this->viewPort = viewPort;
    
    numPlayers = numPlayer;
    bMainView = false;

}

bool skeletonView::isMain(){
    return bMainView;
}

void skeletonView::update(){
    kinect.update();
    skeletonGui.update();
    mat.makeIdentityMatrix();
    ofPoint offsetPt = ofPoint(offsetX, offsetY, offsetZ);
    mat.glTranslate(offsetPt);
    mat.glRotate(rotationX, 1,0,0);
    mat.glRotate(rotationY, 0,1,0);
    mat.glRotate(rotationZ, 0,0,1);
    ofPoint scaleTemp = ofPoint(scaleX, scaleY, scaleZ);;
    mat.glScale(scaleTemp.x, scaleTemp.y, scaleTemp.z);
    
    if (skeletons->size() > 0){
        int index = 0;
        float dist = FLT_MAX;
        for(int i = 0; i < skeletons->size(); i++){
            KS.setFromSkeleton(skeletons->at(i), mat);
            kinectBody & body = bodyMap[skeletons->at(i).getBodyId()];
            KB = body;
            bool bNewFrame = body.addSkeleton(KS);
            if (bNewFrame){
                KSA.analyze(body.getLastSkeleton());
                KBA.analyze(body);
            }
        }
    }else {
        if(bodyMap.size() > 0){
            bodyMap.clear();
            
        }
    }
}

kinectBody * skeletonView::getBody(int i){
    if(skeletons->size() >= i+1){
        return &bodyMap[skeletons->at(i).getBodyId()];
    }else{
        return NULL;
    }
}

void skeletonView::setMainView(bool mainView){
    bMainView = mainView;
}
void skeletonView::draw(){
    ofBackground(0, 0, 0);
    cam.begin(viewPort);
    ofPushStyle();
    ofSetColor(255,255,255,200);
    ofSetLineWidth(3);
    ofPushMatrix();
    ofRotate(90,0,0,1);
    ofDrawGridPlane(1000);
    ofPopMatrix();
    
    
//    ofPushMatrix();
//    ofRotate(90, 1, 0, 0);
//    ofSetColor(0, 100, 100, 200);
//    ofTranslate(stageX-stageSize/2.0, stageY-stageSize/2.0, stageZ);
//    ofRect(0, 0, stageSize, stageSize);
//    ofPopMatrix();
//    ofSetColor(255,255,255,127);
//    ofDrawAxis(50);
    
    ofSetColor(255, 255, 255, 255);
    for( vector<Skeleton>::iterator iter = skeletons->begin(); iter != skeletons->end(); ++iter){
        ofPushStyle();
        bodyMap[iter->getBodyId()].draw();
        ofPopStyle();
    }
    
    ofPopStyle();
    cam.end();
    
    if(isMain()){
        drawControlPanel();
    }
}

void skeletonView::drawControlPanel(){
    if(isMain()){
        skeletonGui.draw();
    }
}

void skeletonView::exit(){
    skeletonGui.saveSettings("skeleton-transform.xml");
}

void skeletonView::setupGUI(){
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
    
    skeletonGui.setup("Skeleton Controls", 0, 0, 300, ofGetWindowHeight(), true, false);
    skeletonGui.setWhichPanel(0);
    skeletonGui.setWhichColumn(0);
    skeletonGui.addGroup(skeletonTransform);
    skeletonGui.loadSettings("skeleton-transform.xml");
}