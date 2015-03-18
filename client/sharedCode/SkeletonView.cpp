//
//  SkeletonView.cpp
//  kinectDebug
//
//  Created by dantheman on 3/14/15.
//
//

#include "SkeletonView.h"

SkeletonView::SkeletonView(){
    
}
SkeletonView::~SkeletonView(){
    
}
void SkeletonView::setup(int numPlayer, ofRectangle viewPort){
    setupGUI();
    kinect.setup(12345);
    kinect.setSmoothing(SIMPLE_MOVING_AVERAGE);
    skeletons = kinect.getSkeletons();
    KS.setup();

    cam.setPosition(0, 500, -1000);
    cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
    
    
    this->viewPort = viewPort;
    
    numPlayers = numPlayer;
    bMainView = false;
    
    plane.set(1000, 1000, 20, 20);
}

bool SkeletonView::isMain(){
    return bMainView;
}

void SkeletonView::update(){
    kinect.update();
    mat.makeIdentityMatrix();
    ofPoint offsetPt = ofPoint(offsetX, offsetY, offsetZ);
    mat.glTranslate(offsetPt);
    mat.glRotate(rotationX, 1,0,0);
    mat.glRotate(rotationZ, 0,0,1);
    mat.glRotate(rotationY, 0,1,0);
    ofPoint scaleTemp = ofPoint(scaleX, scaleY, scaleZ);;
    mat.glScale(scaleTemp.x, scaleTemp.y, scaleTemp.z);
    
    ofQuaternion foo = kinect.getQuaternion();
    ofMatrix4x4 fooMat;
    foo.get(fooMat);
    
    mat*=fooMat.getInverse();
    
    
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

kinectBody * SkeletonView::getBody(int i){
    if(skeletons->size() >= i+1){
        return &bodyMap[skeletons->at(i).getBodyId()];
    }else{
        return NULL;
    }
}

void SkeletonView::setMainView(bool mainView){
    bMainView = mainView;
}
void SkeletonView::draw(ofRectangle viewport){
    ofBackground(0, 0, 0);
    cam.begin(viewport);
    ofPushStyle();
    ofSetColor(255,255,255,200);
    ofSetLineWidth(2);
    ofPushMatrix();
    ofRotate(90,0,0,1);
    //plane.drawWireframe();
    ofDrawGridPlane(1000);
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofRotate(90, 1, 0, 0);
    ofSetColor(ofColor::slateBlue, 200);
    ofTranslate(stageRightX-stageSize/2.0, stageRightY-stageSize/2.0, stageRightZ);
    ofRect(0, 0, stageSize, stageSize);
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofRotate(90, 1, 0, 0);
    ofSetColor(ofColor::slateGray, 200);
    ofTranslate(stageLeftX-stageSize/2.0, stageLeftY-stageSize/2.0, stageLeftZ);
    ofRect(0, 0, stageSize, stageSize);
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofTranslate(stageRightX, -stageRightZ, stageRightY);
    ofPushMatrix();
    ofTranslate(p2SwitchMode);
    ofDrawAxis(10);
    ofPopMatrix();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(stageLeftX, -stageLeftZ, stageLeftY);
    ofPushMatrix();
    ofTranslate(p1SwitchMode);
    ofDrawAxis(10);
    ofPopMatrix();
    ofPopMatrix();
    
    
    ofSetColor(255, 255, 255, 255);
    for( vector<Skeleton>::iterator iter = skeletons->begin(); iter != skeletons->end(); ++iter){
        ofPushStyle();
        bodyMap[iter->getBodyId()].draw();
        ofPopStyle();
    }
    
    ofPopStyle();
    cam.end();
    
    //    if(isMain()){
    //        drawControlPanel();
    //    }
}

void SkeletonView::setButtonPos(ofVec3f p1, ofVec3f p2){
    
    
    cout<<p1<<endl;
    p1SwitchMode.set((p1.x-512)/1024*stageSize, 0, (p1.y-512)/1024*stageSize);
    cout<<p1SwitchMode<<endl;
    p2SwitchMode.set((p2.x-512)/1024*stageSize, 0, (p2.y-512)/1024*stageSize);
    
////    p1SwitchMode*=stageSize;
////    p2SwitchMode*=stageSize;
//    
//    
////    p1SwitchMode = (ofVec3f(stageLeftX, stageLeftY, stageLeftZ)) - p1SwitchMode;
////    p2SwitchMode = (ofVec3f(stageRightX, stageRightY, stageRightZ)) - p2SwitchMode;
//////    p1SwitchMode-=stageSize/2;
//////    p2SwitchMode-=stageSize/2;
//    
//    
//    
//    cout<<p1SwitchMode<<endl;
}


void SkeletonView::exit(){

}

void SkeletonView::setupGUI(){
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
    
    
    stageParams.setName("Stage Settings");
    stageParams.add(stageLeftX.set("Left Stage X", 0, -500, 500));
    stageParams.add(stageLeftY.set("Left Stage Y", 0, -500, 500));
    stageParams.add(stageLeftZ.set("Left Stage Z", 0, -500, 500));
    stageParams.add(stageRightX.set("Right Stage X", 0, -500, 500));
    stageParams.add(stageRightY.set("Right Stage Y", 0, -500, 500));
    stageParams.add(stageRightZ.set("Right Stage Z", 0, -500, 500));
    stageParams.add(stageSize.set("Stage Size", 300, 1, 500));
}