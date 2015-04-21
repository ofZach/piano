
#include "tvScreen.h"

//--------------------------------------------------
float tvScreen::nonOfRandom(float x, float y) {
	return x + ofMap( eng(), 0, 0xFFFFFFFF, 0, 1.0, true) * (y-x);
}

void tvScreen::nonOfSeedRandom(int seed) {
	eng.seed(seed);
}

void tvScreen::setStagePos(float x, float y, float z, float size){
	stagePos.set(x, y, z);
	stageSize = size;
}

void tvScreen::addImplusePlayerTwo(){
	if(pulsesP2.size() < 10){
		pulseData data;
		data.lifetime = 1.0+0.2;
		data.noiseSeed = ofGetElapsedTimef();
		pulsesP2.push_back(data);
	}
	if(bDrawHairyManP2){
		energyHairy[1]+=energyAdder;
	}else{
		energyHistory[1]+=energyAdder;
	}

}

void tvScreen::addImplusePlayerOne(){
	if(pulsesP1.size() < 10){
		pulseData data;
		data.lifetime = 1.0+0.2;
		data.noiseSeed = ofGetElapsedTimef();
		pulsesP1.push_back(data);
	}
	if(bDrawHairyManP1){
		energyHairy[0]+=energyAdder;
	}else{
		energyHistory[0]+=energyAdder;
	}
}


void tvScreen::setup(int num, ofRectangle viewport) {
	tvViewPort = viewport;
	tvSkeletonView.allocate(tvViewPort.width, tvViewPort.height, GL_RGBA, 4);
	tvSkeletonView.begin();
	ofClear(0,0,0,255);
	tvSkeletonView.end();

	tvGridView.allocate(tvViewPort.width, tvViewPort.height, GL_RGBA, 4);
	tvGridView.begin();
	ofClear(0,0,0,255);
	tvGridView.end();

	sphere = ofSpherePrimitive(5, 10).getMesh();

	SKELETOR::Instance()->setup();  // don't know who does this first.
	connections = SKELETOR::Instance()->connections;
	connectionsScambled = connections;

	std::random_shuffle(connectionsScambled.begin(),connectionsScambled.end());

	for (int i = 0; i < connections.size(); i++){
		connectionsScambled[i].a = connections[i].a;
	}
	energyHairy.assign(2, 0.0);
	energyHistory.assign(2, 0.0);

	twist = 0;


	numPlayers = num;
}

bool removeIfZero(pulseData p) {
	return p.lifetime < 0.01;
}

void tvScreen::update( kinectBody * kinectBodyOne,  kinectBody * kinectBodyTwo){
	playerOneBody = kinectBodyOne;
	playerTwoBody = kinectBodyTwo;

	for (int i = 0; i < pulsesP1.size(); i++){
		pulsesP1[i].lifetime -= 0.01;
	}

	for (int i = 0; i < pulsesP2.size(); i++){
		pulsesP2[i].lifetime -= 0.01;
	}

	ofRemove(pulsesP1, removeIfZero);
	ofRemove(pulsesP2, removeIfZero);

	//update the camera
	ofPoint position (0 + cameraRadius * cos(ofDegToRad(cameraAngle)), cameraHeight, 0 + cameraRadius  * sin(ofDegToRad(cameraAngle)));

	if(swayCamera) {
		float t = ofGetElapsedTimef() * swayRate;
		ofVec3f swayDir(ofNoise(t * 0.9), ofNoise(t * 0.89), ofNoise(t * 0.88));
		position += swayDir * (swayAmount * 100.);
	}

	cam.setPosition(position);
	cam.lookAt( ofPoint(0,lookAtPointHeight,0));

	drawIntoFbo();
}


void tvScreen::drawHistoryMan( kinectBody & BODY, int playerID){

	ofMatrix4x4 transform;
	transform.makeIdentityMatrix();
	ofMatrix4x4 scaleMat;
	scaleMat.makeIdentityMatrix();

	// calculate twist value from spine
	kinectSkeleton& sk = BODY.getLastSkeleton();
	ofVec2f spineBase = sk.pts[SKELETOR::Instance()->centerEnumsToIndex[::spineBase]];
	ofVec2f spineTop = sk.pts[SKELETOR::Instance()->centerEnumsToIndex[::spineShoulder]];
	float currentTwist = ofMap(spineBase.x - spineTop.x, -30, 30, -1, 1, true);
	twist = ofLerp(twist, currentTwist, 0.1);

	//cout << BODY.history.size()  << endl;

	for (int i = BODY.history.size()-1; i >= 0; i--){

		if (i < BODY.history.size()/3) continue;

		float pctMap = ofMap(i, BODY.history.size()/3, BODY.history.size(), 0, 1);
		float strength = 0;
		float diff;
		if(playerID == 0){
			for (int j = 0; j < pulsesP1.size(); j++){
				diff = pulsesP1[j].lifetime - pctMap;

				float pctToLook = ofMap(pctMap, 1, 0, 0.25, 0.01);
				if (fabs(diff) < pctToLook){
					strength += 1.0 - (fabs(diff)/pctToLook);
				}
			}
		}else{
			for (int j = 0; j < pulsesP2.size(); j++){
				diff = pulsesP2[j].lifetime - pctMap;

				float pctToLook = ofMap(pctMap, 1, 0, 0.25, 0.01);
				if (fabs(diff) < pctToLook){
					strength += 1.0 - (fabs(diff)/pctToLook);
				}
			}
		}


		strength = ofClamp(strength, 0, 1);

		// float transmap = ofMap(pctMap, 0, 1, 1.5, 15.5);

		transform.glTranslate(ofPoint(0,0,5));
		transform.glRotateRad(twist * 0.05, 0, 0, 1);

		float scale = ofMap(pctMap, 0, 1, 1.01, 1.09);
		scaleMat.glScale(scale,scale, scale);

		if (BODY.history.size() == 50){
			int skipRate = ofMap(i, BODY.history.size()-1, BODY.history.size()/3, 2, 4);
			if (i % skipRate == 0) ;
			else {
				continue;
			}
		}


		kinectSkeleton & SKtemp = BODY.history[i];

		ofPoint avg;
		for (auto & pt : SKtemp.pts){
			avg += pt;
		}
		avg /= 25.0;

		ofPushMatrix();
		ofTranslate(avg);
		for (auto & bone : connections){

			ofPoint a(SKtemp.pts[bone.a]);
			ofPoint b(SKtemp.pts[bone.b]);




			float scale = energyHistory[playerID];

			ofPoint midPt = (a+b)/2.0;
			float dist = (a-b).length();
			dist -= 10;

			if (dist < 0) {
				dist = 0;
			}

			ofPoint normal = (a-b).getNormalized();

			ofPoint aNew = midPt + (dist * 0.5) * normal - avg;
			ofPoint bNew = midPt - (dist * 0.5) * normal - avg;

			ofSetColor(255,255,255,200 * strength * (pctMap*pctMap));
			ofLine (aNew*transform*scaleMat, bNew*transform*scaleMat);
			ofLine (aNew*transform.getInverse()*scaleMat.getInverse(), bNew*transform.getInverse()*scaleMat.getInverse());
		}
		ofPopMatrix();

	}

	kinectSkeleton & SK = BODY.getLastSkeleton();


	for (auto & bone : connections){

		ofPoint a( SK.pts[bone.a]);
		ofPoint b( SK.pts[bone.b]);

		float scale = energyHistory[playerID];

		ofSetColor(255,255,255);

		ofPoint midPt = (a+b)/2.0;
		float dist = (a-b).length();
		dist -= 20 * (1-scale);
		if (dist < 0) dist = 0;
		ofPoint normal = (a-b).getNormalized();

		ofPoint aNew = midPt + (dist * 0.5) * normal;
		ofPoint bNew = midPt - (dist * 0.5) * normal;

		ofLine (aNew, bNew);
	}

	for (int i= 0; i < SK.pts.size(); i++){

		float scale = 1.0 - energyHistory[playerID];;
		scale = powf(scale, 4.0);
		if (!SKELETOR::Instance()->bIsSkipPoint(i)){
			ofPushMatrix();

			ofTranslate(SK.pts[i]);

			ofScale(scale, scale, scale);
			sphere.draw();
			ofPopMatrix();
		}

	}
}

void tvScreen::drawHairyMan( kinectSkeleton & SK, int playerID){

	nonOfSeedRandom((int)(energyHairy[playerID]*100));
	for (auto & bone : connections){

		//cout << bone.a <<  " " << bone.b << endl;
		ofPoint a( SK.pts[bone.a]);
		ofPoint b( SK.pts[bone.b]);

		float scale = energyHairy[playerID];

		ofSetColor(255,255,255);

		ofPoint midPt = (a+b)/2.0;
		float dist = (a-b).length();
		dist -= 20 * (1-scale);
		if (dist < 0) dist = 0;
		ofPoint normal = (a-b).getNormalized();

		ofPoint aNew = midPt + (dist * 0.5) * normal;
		ofPoint bNew = midPt - (dist * 0.5) * normal;
		ofPoint diff = bNew - aNew;
		ofPoint diffNormalized = diff.getNormalized();
		float len = diff.length();
		for (int i = 1; i+1 < 9; i ++){

			ofPoint a1 = aNew + (diff/10.0) * i;
			ofPoint b1 = aNew + (diff/10.0) * (i+1);
			ofPoint midPT = (a1 + b1)/2.0;

			ofPushMatrix();
			ofTranslate(midPT);
			if (true){

				ofRotate(100 * scale, nonOfRandom(0,3*scale), nonOfRandom(0,3*scale),nonOfRandom(0,3*scale));
				ofTranslate(nonOfRandom(0, 0+10*scale), nonOfRandom(0, 0+10*scale), nonOfRandom(0, 0+10*scale));
				float ss =nonOfRandom(1 + 10 * powf(scale, 4.0), 1+20*powf(scale, 4.0));
				ofScale(ss,ss,ss);
			}
			ofLine (len/20.0 * diffNormalized,-len/20*diffNormalized );
			ofPopMatrix();
		}


		//ofLine(aNew,bNew);



	}

	for (int i= 0; i < SK.pts.size(); i++){

		float scale = 1.0 - scale;
		scale = powf(scale, 4.0);
		if (!SKELETOR::Instance()->bIsSkipPoint(i)){
			ofPushMatrix();

			ofTranslate(SK.pts[i]);

			ofScale(scale, scale, scale);
			sphere.draw();
			ofPopMatrix();
		}

	}

}

void tvScreen::setStageParameters(ofParameterGroup params){
    stageOne = params.getVec3f("Stage One Position");
    stageTwo = params.getVec3f("Stage Two Position");


	stageSize = params.getFloat("Stage Size");
}

void tvScreen::addParameters(ofParameterGroup & params){
	params.add(cameraAngle.set("cameraAngle", 0, 0, 360));
	params.add(cameraRadius.set("cameraRadius", 0, 0, 1000));
	params.add(cameraHeight.set("cameraHeight", 0, 0, 1000));
	params.add(lookAtPointHeight.set("View Height", 0, 0, 500));
	params.add(swayCamera.set("swayCamera", true));
	params.add(swayRate.set("swayRate", 0.1, 0.01, 10));
	params.add(swayAmount.set("swayAmount", 0.1, 0, 1));
	params.add(bDrawHairyManP1.set("bDrawHairyMan Player Two", false));
	params.add(bDrawHairyManP2.set("bDrawHairyMan Player One", false));
	params.add(energyAdder.set("energyAdder", 0.25, 0.1, 2.0));
}

void tvScreen::drawIntoFbo(){


	ofPushStyle();
	tvGridView.begin();
	ofClear(0,0,0,0);
	//ofClear(0, 0, 0);
	cam.begin(ofRectangle(ofVec2f(0, 0), tvViewPort.width, tvViewPort.height));
	ofSetColor(255,255,255,127);
	ofSetLineWidth(3);
	ofPushMatrix();
	ofSetColor(100);
	ofNoFill();
	ofRotate(90,1,0,0);
	ofTranslate(stageOne->x - stageSize/2, stageOne->z - stageSize/2, -stageOne->y);
	ofRect(0, 0, stageSize, stageSize);
	ofPopMatrix();

	if(numPlayers > 1){
		ofPushMatrix();
		ofSetColor(100);
		ofNoFill();
		ofRotate(90,1,0,0);
		ofTranslate(stageTwo->x- stageSize/2, stageTwo->z - stageSize/2, -stageTwo->y);
		ofRect(0, 0, stageSize, stageSize);
		ofPopMatrix();
	}
	ofFill();
	cam.end();
	tvGridView.end();

	tvSkeletonView.begin();
	ofClear(0,0,0,0);
	//ofClear(0, 0, 0);
	cam.begin(ofRectangle(ofVec2f(0, 0), tvViewPort.width, tvViewPort.height));



	if (playerOneBody != NULL){

		kinectSkeleton SK = playerOneBody->getLastSkeleton();

		if (bDrawHairyManP1){
			//cout << " hairy 1 " << endl;
			energyHairy[0]*= 0.9;
			if (energyHairy[0] > 1) energyHairy[0] = 1;
			if (energyHairy[0] < 0.01) energyHairy[0] = 0;
			drawHairyMan(SK, 0);
		} else {
			energyHistory[0]*= 0.9;
			if (energyHistory[0] > 1) energyHistory[0] = 1;
			if (energyHistory[0] < 0.01) energyHistory[0] = 0;
			//cout << " history 1 " << endl;
			drawHistoryMan(*playerOneBody, 0);
		}
	}


	if (playerTwoBody != NULL && numPlayers > 1){

		kinectSkeleton SK = playerTwoBody->getLastSkeleton();

		if (bDrawHairyManP2){
			//cout << " hairy 2 " << endl;
			energyHairy[1]*= 0.9;
			if (energyHairy[1] > 1) energyHairy[1] = 1;
			if (energyHairy[1] < 0.01) energyHairy[1] = 0;
			drawHairyMan(SK, 1);
		} else {

			//cout << " history 2 " << endl;
			energyHistory[1]*= 0.9;
			if (energyHistory[1] > 1) energyHistory[1] = 1;
			if (energyHistory[1] < 0.01) energyHistory[1] = 0;
			drawHistoryMan(*playerTwoBody, 1);
		}
	}
	cam.end();
	tvSkeletonView.end();
	ofPopStyle();
}

void tvScreen::draw( ofRectangle drawRect){
	ofPushStyle();
	ofSetColor(255,255,255);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	tvGridView.draw(drawRect);
	tvSkeletonView.draw(drawRect);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPopStyle();

}