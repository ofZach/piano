//
//  kinectSkeleton.cpp
//  skeletonReceiver
//
//  Created by zach on 1/26/15.
//
//

#include "kinectSkeleton.h"
void kinectSkeleton::setup(){


	SKELETOR::Instance()->setup();

	for (int i = 0; i < 25; i++){
		pts.push_back(ofPoint(0,0,0));
		trackingStates.push_back(0);
		pts2d.push_back(ofPoint(0,0,0));
	}

}

ofPoint kinectSkeleton::getCenterPoint ( int name){
	return getPoint(name, ::center);
}

ofPoint kinectSkeleton::getLeftPoint ( int name ){
	return getPoint(name, ::left);
}

ofPoint kinectSkeleton::getRightPoint ( int name){
	return getPoint(name, ::right);
}

ofPoint kinectSkeleton::getPoint(int name, int side){
	if (side == center){
		if (SKELETOR::Instance()->centerEnumsToIndex.find(name) != SKELETOR::Instance()->centerEnumsToIndex.end()){
			return pts[SKELETOR::Instance()->centerEnumsToIndex[name]];
		}
	} else if (side == ::left){
		if (SKELETOR::Instance()->leftEnumsToIndex.find(name) != SKELETOR::Instance()->leftEnumsToIndex.end()){
			return pts[SKELETOR::Instance()->leftEnumsToIndex[name]];
		}
	} else if (side == ::right){
		if (SKELETOR::Instance()->rightEnumsToIndex.find(name) != SKELETOR::Instance()->rightEnumsToIndex.end()){
			return pts[SKELETOR::Instance()->rightEnumsToIndex[name]];
		}
	}
	return ofPoint(0,0,0);
}





void kinectSkeleton::setFromSkeleton( ofxKFW2::Data::Body& sk, ofMatrix4x4 transform){

	pts[SKELETOR::Instance()->nameToIndex["ThumbRight"]].set(sk.joints[JointType_ThumbRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["SpineBase"]].set(sk.joints[JointType_SpineBase].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["SpineMid"]].set(sk.joints[JointType_SpineMid].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["Neck"]].set(sk.joints[JointType_Neck].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["Head"]].set(sk.joints[JointType_Head].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["ShoulderLeft"]].set(sk.joints[JointType_ShoulderLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["ElbowLeft"]].set(sk.joints[JointType_ElbowLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["WristLeft"]].set(sk.joints[JointType_WristLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["HandLeft"]].set(sk.joints[JointType_HandLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["ShoulderRight"]].set(sk.joints[JointType_ShoulderRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["ElbowRight"]].set(sk.joints[JointType_ElbowRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["WristRight"]].set(sk.joints[JointType_WristRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["HandRight"]].set(sk.joints[JointType_HandRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["HipLeft"]].set(sk.joints[JointType_HipLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["KneeLeft"]].set(sk.joints[JointType_KneeLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["AnkleLeft"]].set(sk.joints[JointType_AnkleLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["FootLeft"]].set(sk.joints[JointType_FootLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["HipRight"]].set(sk.joints[JointType_HipRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["KneeRight"]].set(sk.joints[JointType_KneeRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["AnkleRight"]].set(sk.joints[JointType_AnkleRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["FootRight"]].set(sk.joints[JointType_FootRight].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["SpineShoulder"]].set(sk.joints[JointType_SpineShoulder].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["HandTipLeft"]].set(sk.joints[JointType_HandTipLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["ThumbLeft"]].set(sk.joints[JointType_ThumbLeft].getPosition() * transform);
	pts[SKELETOR::Instance()->nameToIndex["HandTipRight"]].set(sk.joints[JointType_HandTipRight].getPosition() * transform);


	trackingStates[SKELETOR::Instance()->nameToIndex["SpineBase"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["SpineMid"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["Neck"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["Head"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["ShoulderLeft"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["ElbowLeft"]] =sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["WristLeft"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["HandLeft"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["ShoulderRight"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["ElbowRight"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["WristRight"]] =sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["HandRight"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["HipLeft"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["KneeLeft"]] =sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["AnkleLeft"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["FootLeft"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["HipRight"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["KneeRight"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["AnkleRight"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["FootRight"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["SpineShoulder"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["HandTipLeft"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["ThumbLeft"]] = sk.joints[JointType_ThumbRight].getTrackingState();
	trackingStates[SKELETOR::Instance()->nameToIndex["HandTipRight"]] =sk.joints[JointType_ThumbRight].getTrackingState();


	//gestures = sk.gestures;
}

void kinectSkeleton::draw(){
	ofPushStyle();
	ofSpherePrimitive primitve(5,10);
	ofVboMesh mesh = primitve.getMesh();
	for (int i = 0; i < pts.size(); i++){

		if (std::find(SKELETOR::Instance()->skipList.begin(), SKELETOR::Instance()->skipList.end(), SKELETOR::Instance()->indexToName[i] )!= SKELETOR::Instance()->skipList.end()){
			continue;
		}

		if (trackingStates[i] == TrackingState_Tracked) ofSetColor(255,255,255, 255);
		else if (trackingStates[i] == TrackingState_Inferred) ofSetColor(127,127,127, 255);

		else ofSetColor(255,0,0, 100);
		ofPushMatrix();

		ofTranslate(pts[i]);
		mesh.draw();
		ofPopMatrix();
	}

	ofSetColor(255,255,255,255);
	drawBones();
	ofPopStyle();
}

void kinectSkeleton::drawDebug(bool boundingbox){

	ofPushStyle();
	ofEnableAlphaBlending();
	float alpha = 1.0;

	// draw HandToHandVector
	ofSetColor(
		ofFloatColor(ofColor::orange, ((leftHandSpan + rightHandSpan) / 2.0) * alpha));
	ofLine(getLeftPoint(hand),
		getRightPoint(hand));

	// Draw Left Arm Extended
	ofSetColor(ofFloatColor(ofColor::red, (armLeftExtendedPct*alpha)));
	ofLine(getLeftPoint(hand),
		getLeftPoint(shoulder));

	// Draw Right Arm Extended
	ofSetColor(ofFloatColor(ofColor::red, (armRightExtendedPct*alpha)));
	ofLine(getRightPoint(hand),
		getRightPoint(shoulder));

	// Draw Left Arm To Hip
	ofSetColor(ofFloatColor(ofColor::darkorange, leftHandVHip*alpha));
	ofLine(getLeftPoint(hand),
		getLeftPoint(hip));

	// Draw Left Arm To Hip
	ofSetColor(ofFloatColor(ofColor::darkorange, rightHandVHip*alpha));
	ofLine(getRightPoint(hand),
		getRightPoint(hip));

	// Draw Head to Feet
	ofSetColor(ofFloatColor(ofColor::darkRed, (1.0 - distFootLeft)*alpha));
	ofLine(getCenterPoint(head),
		getLeftPoint(ankle));

	// Draw Head to Feet
	ofSetColor(ofFloatColor(ofColor::darkRed, (1.0 - distFootRight)*alpha));
	ofLine(getCenterPoint(head),
		getRightPoint(ankle));

	// Draw Foot to Foot
	ofSetColor(
		ofFloatColor(ofColor::violet, ((leftFootSpan + rightFootSpan) / 2.0)*alpha));
	ofLine(getLeftPoint(ankle),
		getRightPoint(ankle));

	// Draw Left Hip to Foot
	ofSetColor(ofFloatColor(ofColor::blueViolet, legLeftExtendedPct*alpha));
	ofLine(getLeftPoint(hip),
		getLeftPoint(ankle));

	// Draw Right Hip to Foot
	ofSetColor(ofFloatColor(ofColor::blueViolet, legRightExtendedPct*alpha));
	ofLine(getRightPoint(hip),
		getRightPoint(ankle));

	// Draw Angle Right Knee
	ofSpherePrimitive primitve(5, 5);
	ofVboMesh mesh = primitve.getMesh();
	ofPushMatrix();
	ofSetColor(
		ofFloatColor(ofColor::orangeRed, ofMap(angleRightKnee, 0, 180, 0, 1, true)*alpha));
	ofTranslate(
		getRightPoint(knee));
	mesh.draw();
	ofPopMatrix();

	ofLine(getRightPoint(hip),
		getRightPoint(knee));
	ofLine(getRightPoint(ankle),
		getRightPoint(knee));

	// Draw Angle Left Knee
	ofPushMatrix();
	ofSetColor(
		ofFloatColor(ofColor::orangeRed, ofMap(angleLeftKnee, 0, 180, 0, 1, true)*alpha));
	ofTranslate(
		getLeftPoint(knee));
	mesh.draw();
	ofPopMatrix();

	ofLine(getLeftPoint(hip),
		getLeftPoint(knee));
	ofLine(getLeftPoint(ankle),
		getLeftPoint(knee));

	// Draw Elbow Left Elbow
	ofPushMatrix();
	ofSetColor(ofFloatColor(ofColor::orangeRed,
		ofMap(angleRightElbow, 0, 180, 0, 1, true)*alpha));
	ofTranslate(
		getLeftPoint(elbow));
	mesh.draw();
	ofPopMatrix();

	ofLine(getLeftPoint(hand),
		getLeftPoint(elbow));
	ofLine(getLeftPoint(elbow),
		getLeftPoint(shoulder));

	// Draw Elbow Right Elbow
	ofPushMatrix();
	ofSetColor(ofFloatColor(ofColor::magenta,
		ofMap(angleRightElbow, 0, 180, 0, 1, true)*alpha));
	ofTranslate(
		getRightPoint(elbow));
	mesh.draw();
	ofPopMatrix();

	ofLine(getRightPoint(hand),
		getRightPoint(elbow));
	ofLine(getRightPoint(elbow),
		getRightPoint(shoulder));

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPushMatrix();
	ofSetColor(255, 255, 255, 200);
	ofTranslate(getCenterPoint(spineMid));

	//        // Draw Bounding Cube
	//        ofRotate(0, orientation.x, orientation.y, orientation.z);
	//        
	//        //        ofBoxPrimitive p(100,100,100);
	//        //        p.drawWireframe();
	//        //
	//        //        ofDrawAxis(20);
	ofPopMatrix();

	centerPoint.draw();
	if (boundingbox) {
		ofSetLineWidth(1);
		ofPushMatrix();
		ofMultMatrix(centerPoint.getGlobalTransformMatrix());
		ofBoxPrimitive p(400, 400, 400);
		ofSetColor(255, 255, 255, 255*alpha);
		p.drawWireframe();
		ofPopMatrix();
		ofSetLineWidth(2);
		ofSetColor(255);
	}
	ofDisableAlphaBlending();
	ofPopStyle();
}




void kinectSkeleton::drawBones() {

	for(auto boneTemp : SKELETOR::Instance()->bones) {
		vector < int > & ptIds = boneTemp.second.pointIds;
		for (int i = 0; i+1 < ptIds.size(); i++){

			drawBone(ptIds[i], ptIds[i+1]);
		}
	}
}



void kinectSkeleton::drawBone(int indexA, int indexB){


	if (std::find(SKELETOR::Instance()->skipList.begin(), SKELETOR::Instance()->skipList.end(), SKELETOR::Instance()->indexToName[indexA] )!=SKELETOR::Instance()->skipList.end() ||
		std::find(SKELETOR::Instance()->skipList.begin(), SKELETOR::Instance()->skipList.end(), SKELETOR::Instance()->indexToName[indexB] )!=SKELETOR::Instance()->skipList.end() ){

			return;

	}

	int trackingState;

	trackingState = TrackingState_Tracked;

	if (trackingStates[indexA] == TrackingState_Inferred ||
		trackingStates[indexB] == TrackingState_Inferred){
			trackingState = TrackingState_Inferred;
	}

	if (trackingStates[indexA] == TrackingState_NotTracked ||
		trackingStates[indexB] == TrackingState_NotTracked){
			trackingState = TrackingState_Inferred;
	}

	if(trackingState == TrackingState_Tracked) {
		ofSetLineWidth(2);
		ofSetColor(ofColor::white, 100);
		ofLine( pts[indexA], pts[indexB] );
	} else if(trackingState == TrackingState_Inferred) {
		ofSetLineWidth(2);
		ofSetColor(ofColor::white, 100);
		ofLine( pts[indexA], pts[indexB] );
	}
}
