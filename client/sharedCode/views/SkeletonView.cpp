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
void SkeletonView::setup(int numPlayer, ofRectangle viewport){
    setupGUI();
    kinect.setup(12345);
    kinect.setSmoothing(SIMPLE_MOVING_AVERAGE);
    skeletons = kinect.getSkeletons();
    KS.setup();
    
    cam.setPosition(0, 500, -1000);
    cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
    
    
    this->viewPort = viewport;
    
    numPlayers = numPlayer;
    bMainView = false;
    
    plane.set(1000, 1000, 20, 20);
    
    
    rect[0].set(ofVec2f(0, 0), viewPort.getWidth()/2, viewPort.getHeight()/2);
    rect[1].set(ofVec2f(viewPort.getWidth()/2, 0), viewPort.getWidth()/2, viewPort.getHeight()/2);
    rect[2].set(ofVec2f(0, viewPort.getHeight()/2), viewPort.getWidth()/2, viewPort.getHeight()/2);
    rect[3].set(ofVec2f(viewPort.getWidth()/2, viewPort.getHeight()/2), viewPort.getWidth()/2, viewPort.getHeight()/2);
    
    
    
    frontCam.setPosition(0, 1, -1000);
    frontCam.lookAt(ofVec3f(0, 0, 0));
    //    frontCam.enableOrtho();
    
    topCam.setPosition(0, 1000, 1);
    topCam.lookAt(ofVec3f(0, 0, 0));
    //    topCam.enableOrtho();
    
    leftCam.setPosition(1000, 1, 0);
    leftCam.lookAt(ofVec3f(0, 0, 0));
    //    leftCam.enableOrtho();
    
    playerOneSwitchMode.setTriggerScale(1.0);
    playerOneSwitchMode.setApproachScale(1.26154);
    
    playerTwoSwitchMode.setTriggerScale(1.0);
    playerTwoSwitchMode.setApproachScale(1.26154);
    
    
    playerOneMode = 0;
    playerTwoMode = 0;
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
            bool bNewFrame = body.addSkeleton(KS);
            if (bNewFrame){
                KSA.analyze(body.getLastSkeleton());
                KBA.analyze(body);
                
                int whichLeft = SKELETOR::Instance()->getPointIndex(ankle, ::left);
                int whichRight = SKELETOR::Instance()->getPointIndex(ankle, ::right);
                vector<std::pair<ofPoint, float> > feet;
                
                feet.push_back(make_pair(body.getLastSkeleton().pts[whichLeft], 0.f));
                feet.push_back(make_pair(body.getLastSkeleton().pts[whichRight], 0.f));
                //                if( i == 0){
                playerOneSwitchMode.update(feet);
                if (playerOneSwitchMode.isTriggered() && !changeTriggeredOne) {
                    if(playerOneMode == 1){
                        playerOneMode = 0;
                    }else{
                        playerOneMode = 1;
                    }
                    changeTriggeredOne = true;
                }else if(!playerOneSwitchMode.isTriggered()){
                    changeTriggeredOne = false;
                }
                //                }
                //                else if(i == 1){
                playerTwoSwitchMode.update(feet);
                
                if (playerTwoSwitchMode.isTriggered() && !changeTriggeredTwo) {
                    if(playerTwoMode == 1){
                        playerTwoMode = 0;
                    }else{
                        playerTwoMode = 1;
                    }
                    changeTriggeredTwo = true;
                }else if(!playerTwoSwitchMode.isTriggered()){
                    changeTriggeredTwo = false;
                }
                //                }
            }
        }
    }else {
        if(bodyMap.size() > 0){
            bodyMap.clear();
        }
    }
    
    if(!bMainView && bCalibrate){
        bCalibrate = false;
    }
    
    
    stageOne.setPosition(playerOneStagePosition->x, playerOneStagePosition->y+stageSize/4, playerOneStagePosition->z);
    stageTwo.setPosition(playerTwoStagePosition->x, playerTwoStagePosition->y+stageSize/4, playerTwoStagePosition->z);
    stageOne.setRadius(stageSize*presencesZoneSizeP1/100);
    stageTwo.setRadius(stageSize*presencesZoneSizeP2/100);
    stageOne.setApproachScale(1);
    stageTwo.setApproachScale(1);
    stageOne.setTriggerScale(0.8);
    stageTwo.setTriggerScale(0.8);
    
    
    
}

int SkeletonView::getPlayerOneMode(){
    return playerOneMode;
}


int SkeletonView::getPlayerTwoMode(){
    return playerTwoMode;
}

kinectBody * SkeletonView::getBody(int stageID){
    if(skeletons->size() > 0){
        if(stageID == 0){
            for(int i = 0; i < skeletons->size(); i++){
                kinectBody & body  = bodyMap[skeletons->at(i).getBodyId()];
                int kneeLeftBase = SKELETOR::Instance()->getPointIndex(knee, ::left);
                int kneeRightBase = SKELETOR::Instance()->getPointIndex(knee, ::right);
                vector<std::pair<ofPoint, float> > center;
                center.push_back(make_pair(body.getLastSkeleton().pts[kneeLeftBase], 0.f));
                center.push_back(make_pair(body.getLastSkeleton().pts[kneeRightBase], 0.f));
                stageOne.update(center);
                if(stageOne.isTriggered()){
                    //                    cout<<"Body Found"<<endl;
                    return &bodyMap[skeletons->at(i).getBodyId()];
                }
            }
            return NULL;
        }
        if(stageID == 1){
            for(int i = 0; i < skeletons->size(); i++){
                kinectBody & body  = bodyMap[skeletons->at(i).getBodyId()];
                int kneeLeftBase = SKELETOR::Instance()->getPointIndex(knee, ::left);
                int kneeRightBase = SKELETOR::Instance()->getPointIndex(knee, ::right);
                vector<std::pair<ofPoint, float> > center;
                center.push_back(make_pair(body.getLastSkeleton().pts[kneeLeftBase], 0.f));
                center.push_back(make_pair(body.getLastSkeleton().pts[kneeRightBase], 0.f));
                stageTwo.update(center);
                if(stageTwo.isTriggered()){
                    //                    cout<<"Body Found"<<endl;
                    return &bodyMap[skeletons->at(i).getBodyId()];
                }
            }
            return NULL;
        }
    }else{
        return NULL;
    }
}

void SkeletonView::setMainView(bool mainView){
    bMainView = mainView;
}

void SkeletonView::drawScene(ofRectangle view, ofCamera camera){
    
    camera.begin(view);
    
    ofPushStyle();
    {
        ofSetColor(255,255,255,200);
        ofSetLineWidth(2);
        if(bMainView){
            ofPushMatrix();
            {
                ofRotate(90,0,0,1);
                ofDrawGridPlane(1000);
            }
            ofPopMatrix();
        }
        
        ofPushMatrix();
        {
            
            ofRotate(90, 1, 0, 0);
            ofSetColor(ofColor::slateBlue, 200);
            ofTranslate(playerOneStagePosition->x - stageSize/2, playerOneStagePosition->z - stageSize/2, -playerOneStagePosition->y);
            ofRect(0, 0, stageSize, stageSize);
        }
        ofPopMatrix();
        
        if(numPlayers > 1){
            ofPushMatrix();
            {
                ofRotate(90, 1, 0, 0);
                ofSetColor(ofColor::slateGray, 200);
                ofTranslate(playerTwoStagePosition->x - stageSize/2, playerTwoStagePosition->z - stageSize/2, -playerTwoStagePosition->y);
                ofRect(0, 0, stageSize, stageSize);
            }
            ofPopMatrix();
        }
        
        
        if(bCalibrate){
            if(numPlayers > 1){
                stageTwo.draw();
            }
            stageOne.draw();
        }
        playerOneSwitchMode.draw();
        if(numPlayers > 1){
            playerTwoSwitchMode.draw();
        }
        
        
        ofSetColor(255, 255, 255, 255);
        for( vector<Skeleton>::iterator iter = skeletons->begin(); iter != skeletons->end(); ++iter){
            ofPushStyle();
            bodyMap[iter->getBodyId()].draw();
            ofPopStyle();
        }
    }
    ofPopStyle();
    
    camera.end();
    
}


void SkeletonView::draw(ofRectangle viewport){
    
    if(bCalibrate){
        drawScene(rect[0], cam);
        ofDrawBitmapString("3D View", rect[0].x+50, rect[0].y+50);
        drawScene(rect[1], frontCam);
        ofDrawBitmapString("Front View", rect[1].x+50, rect[1].y+50);
        drawScene(rect[2], leftCam);
        ofDrawBitmapString("Left View", rect[2].x+50, rect[2].y+50);
        drawScene(rect[3], topCam);
        ofDrawBitmapString("Top View", rect[3].x+50, rect[3].y+50);
        
    }else{
        drawScene(viewport, cam);
    }
}

void SkeletonView::setButtonPos(ofVec3f p1, ofVec3f p2){
    
    
    p1SwitchMode.set((p1.x-512)/1024*stageSize, 0, (p1.y-512)/1024*stageSize);
    p1SwitchMode+=ofVec3f(playerOneStagePosition->x, playerOneStagePosition->y, playerOneStagePosition->z);
    p2SwitchMode.set((p2.x-512)/1024*stageSize, 0, (p2.y-512)/1024*stageSize);
    p2SwitchMode+=ofVec3f(playerTwoStagePosition->x, playerTwoStagePosition->y, playerTwoStagePosition->z);
    
    
    if(overRideLeftTrigger){
        playerOneSwitchMode.setPosition(ofVec3f(playerOneTriggerPosition->x, playerOneTriggerPosition->y, playerOneTriggerPosition->z));
        playerOneSwitchMode.setRadius(stageLeftTriggerScale);
    }else{
        p1SwitchMode+=ofVec3f(playerOneTriggerPosition->x, playerOneTriggerPosition->y, playerOneTriggerPosition->z);
        playerOneSwitchMode.setPosition(p1SwitchMode);
        playerOneSwitchMode.setRadius(stageSize/4/2+stageLeftTriggerScale);
    }
    
    if(overRideRightTrigger){
        playerTwoSwitchMode.setPosition(ofVec3f(playerTwoTriggerPosition->x, playerTwoTriggerPosition->y, playerTwoTriggerPosition->z));
        playerTwoSwitchMode.setRadius(stageRightTriggerScale);
    }else{
        p2SwitchMode+=ofVec3f(playerTwoTriggerPosition->x, playerTwoTriggerPosition->y, playerTwoTriggerPosition->z);
        playerTwoSwitchMode.setPosition(p2SwitchMode);
        playerTwoSwitchMode.setRadius(stageSize/4/2+stageRightTriggerScale);
    }
    
    
    
}


void SkeletonView::exit(){
    
}

void SkeletonView::setupGUI(){
    skeletonTransform.setName("skeleton transform");
    skeletonTransform.add(offsetX.set("offsetX", 0, -2000,2000));
    skeletonTransform.add(offsetY.set("offsetY", 0, -2000,2000));
    skeletonTransform.add(offsetZ.set("offsetZ", 0, -2000,2000));
    
    stageParams.setName("Stage Settings");
    stageParams.add(playerOneStagePosition.set("Stage One Position", ofVec3f(250, 0, 0), ofVec3f(-500, 0, -500), ofVec3f(500, 0, 500)));
    
    stageParams.add(playerTwoStagePosition.set("Stage Two Position", ofVec3f(-250, 0, 0), ofVec3f(-500, 0, -500), ofVec3f(500, 0, 500)));
    
    stageParams.add(playerOneTriggerPosition.set("Stage One Button Position", ofVec3f(0, 0, 0), ofVec3f(-500, 0, -500), ofVec3f(500, 0, 500)));
    
    stageParams.add(playerTwoTriggerPosition.set("Stage Two Button Position", ofVec3f(0, 0, 0), ofVec3f(-500, 0, -500), ofVec3f(500, 0, 500)));
    
    
    
    
    stageParams.add(stageSize.set("Stage Size", 300, 1, 500));
    stageParams.add(bCalibrate.set("Calibration View", true));
    
    stageParams.add(overRideLeftTrigger.set("overRideLeftTrigger", true));
    stageParams.add(overRideRightTrigger.set("overRideRightTrigger", true));
    
    stageParams.add(stageLeftTriggerScale.set("stageLeftTriggerScale", 0, 0, 500));
    
    
    stageParams.add(stageRightTriggerScale.set("stageRightTriggerScale", 0, 0, 500));
    
    stageParams.add(presencesZoneSizeP1.set("presencesZoneSizeP1", 0, 100, 300));
    stageParams.add(presencesZoneSizeP2.set("presencesZoneSizeP2", 0, 100, 300));
    
    
    hiddenSettings.setName("skeleton transform");
    hiddenSettings.add(rotationX.set("rotationX", 0,-180,180));
    hiddenSettings.add(rotationY.set("rotationY", 0,-180,180));
    hiddenSettings.add(rotationZ.set("rotationZ", 0,-180,180));
    hiddenSettings.add(scaleX.set("scaleX", 1.0,0.01, 20));
    hiddenSettings.add(scaleY.set("scaleY", 1.0,0.01, 20));
    hiddenSettings.add(scaleZ.set("scaleZ", 1.0,0.01, 140));
}