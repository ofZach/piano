

#pragma once



#include "ofMain.h"
#include "kinectBody.h"
#include "prng_engine.hh"
#include <algorithm>    // std::shuffle

struct pulseData {
	float lifetime;
	float noiseSeed;
};

bool removeIfZero(pulseData p) {
	return p.lifetime < 0.01;
}

class tvScreen {
    
    
public:
    
    ofVboMesh sphere;
    
    vector < boneConnection > connections;
    vector < boneConnection > connectionsScambled;
    sitmo::prng_engine eng; /// don't laugh.
    
    vector < pulseData > pulses;    // go from 1 to 0
    
    //--------------------------------------------------
    float nonOfRandom(float x, float y) {
        return x + ofMap( eng(), 0, 0xFFFFFFFF, 0, 1.0, true) * (y-x);
    }
    
    float nonOfSeedRandom(int seed) {
        eng.seed(seed);
    }
    
    
    void addImpluse(){
		pulses.push_back((pulseData){
			.lifetime = 1.0 + 0.2,
			.noiseSeed = ofGetElapsedTimef()
		});
    }
    
    bool bDrawHairyMan;
    
    void setup () {
        tvScreenView.allocate(1920, 1080, GL_RGBA, 4);
        tvScreenView.begin();
        ofClear(0,0,0,255);
        tvScreenView.end();
        sphere = ofSpherePrimitive(5, 10).getMesh();
        
        
        SKELETOR::Instance()->setup();  // don't know who does this first.
        connections = SKELETOR::Instance()->connections;
        connectionsScambled = connections;
        
        std::random_shuffle(connectionsScambled.begin(),connectionsScambled.end());
        
        for (int i = 0; i < connections.size(); i++){
            connectionsScambled[i].a = connections[i].a;
        }
        energy = 0.0;
        
        bDrawHairyMan = false;
        
    }
    
    void update( kinectBody * kinectBody){
        KB = kinectBody;
		
        for (int i = 0; i < pulses.size(); i++){
            pulses[i].lifetime -= 0.01;
        }
		
        ofRemove(pulses, removeIfZero);   
    }
    
    float energy;
    
    
    
    void drawHistoryMan( kinectBody & BODY){
        
        ofMatrix4x4 transform;
        transform.makeIdentityMatrix();
        ofMatrix4x4 scaleMat;
        scaleMat.makeIdentityMatrix();
		ofMatrix4x4 negScaleMat;
		negScaleMat.makeIdentityMatrix();
		ofMatrix4x4 rotation;
		rotation.makeIdentityMatrix();
        
        //cout << BODY.history.size() << endl;
        
        for (int i = BODY.history.size()-1; i >= 0; i--){
            
            if (i < BODY.history.size()/3) continue;
            
            float pctMap = ofMap(i, BODY.history.size()/3, BODY.history.size(), 0, 1);
            float strength = 0;
            for (int j = 0; j < pulses.size(); j++){
                float diff = pulses[j].lifetime - pctMap;
                
                float pctToLook = ofMap(pctMap, 1, 0, 0.25, 0.01);
                if (fabs(diff) < pctToLook){
                    strength += 1.0 - (fabs(diff)/pctToLook);
                }
            }
            
            strength = ofClamp(strength, 0, 1);
            
           // float transmap = ofMap(pctMap, 0, 1, 1.5, 15.5);
            
            transform.glTranslate(ofPoint(0,0,5));
            //transform.glRotate(1.1 * sin(pctMap*8), 1, 0,1);
            float scale = ofMap(pctMap, 0, 1, 1.01, 1.09);
            scaleMat.glScale(scale,scale, scale);
			negScaleMat.glScale(-scale, -scale, -scale);
			
			rotation.glRotateRad(ofSignedNoise(ofGetElapsedTimef() * 0.2) * 0.1, -1, 0.1, 1);
            
            if (BODY.history.size() == 50){
            int skipRate = ofMap(i, BODY.history.size()-1, BODY.history.size()/3, 2, 4);
            if (i % skipRate == 0) ;
            else{
                
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
                
                ofPoint a( SKtemp.pts[bone.a]);
                ofPoint b( SKtemp.pts[bone.b]);
                
                float scale = energy;
                
                ofPoint midPt = (a+b)/2.0;
                float dist = (a-b).length();
                dist -= 15;
                if (dist < 0) dist = 0;
                ofPoint normal = (a-b).getNormalized();
                
                ofPoint aNew = midPt + (dist * 0.5) * normal - avg;
                ofPoint bNew = midPt - (dist * 0.5) * normal - avg;
                
                ofSetColor(255,255,255,200 * strength * (pctMap*pctMap));
//                ofLine (aNew*rotation*transform*scaleMat, bNew*rotation*transform*scaleMat);
				ofLine (aNew*rotation*transform*negScaleMat, bNew*rotation*transform*negScaleMat);
            }
            ofPopMatrix();
        
        }
        
        kinectSkeleton & SK = BODY.getLastSkeleton();
        
        
        for (auto & bone : connections){
            
            //cout << bone.a <<  " " << bone.b << endl;
            ofPoint a( SK.pts[bone.a]);
            ofPoint b( SK.pts[bone.b]);
            
            float scale = energy;
            
            ofSetColor(255,255,255);
            
            ofPoint midPt = (a+b)/2.0;
            float dist = (a-b).length();
            dist -= 20 * (1-energy);
            if (dist < 0) dist = 0;
            ofPoint normal = (a-b).getNormalized();
            
            ofPoint aNew = midPt + (dist * 0.5) * normal;
            ofPoint bNew = midPt - (dist * 0.5) * normal;
            
            ofLine (aNew, bNew);
        }
        
        for (int i= 0; i < SK.pts.size(); i++){
            
            float scale = 1.0 - energy;
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
    
    void drawHairyMan( kinectSkeleton & SK){
        
        nonOfSeedRandom((int)(energy*100));
        for (auto & bone : connections){
            
            //cout << bone.a <<  " " << bone.b << endl;
            ofPoint a( SK.pts[bone.a]);
            ofPoint b( SK.pts[bone.b]);
            
            float scale = energy;
            
            ofSetColor(255,255,255);
            
            ofPoint midPt = (a+b)/2.0;
            float dist = (a-b).length();
            dist -= 20 * (1-energy);
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
                    
                    ofRotate(100 * energy, nonOfRandom(0,3*energy), nonOfRandom(0,3*energy),nonOfRandom(0,3*energy));
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
            
            float scale = 1.0 - energy;
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
    
    void drawIntoFbo( ofCamera & mainViewCam){
        
        tvScreenView.begin();
        ofClear(0,0,0,255);
        //ofClear(0, 0, 0);
        mainViewCam.begin(ofRectangle(ofVec2f(0, 0), tvScreenView.getWidth(), tvScreenView.getHeight()));
        ofSetColor(255,255,255,127);
        ofSetLineWidth(3);
        ofPushMatrix();
        ofSetColor(100);
        ofRotate(90,0,0,1);
        ofDrawGridPlane(1000);
        ofPopMatrix();
        
        
        energy*= 0.9;
        if (ofGetMousePressed()){
            energy += 0.1;
        }
        if (energy > 1) energy = 1;
        if (energy < 0.01) energy = 0;
        
        if (KB != NULL){
            
            kinectSkeleton SK = KB->getLastSkeleton();
            
            if (bDrawHairyMan == true){
                drawHairyMan(SK);
            } else {
                
                drawHistoryMan(*KB);
            }
            
            
            //KB->draw();
        }
        
        mainViewCam.end();
        ofClearAlpha();
        tvScreenView.end();
    }
    
    void draw( ofRectangle drawRect){
        ofSetColor(255,255,255);
        tvScreenView.draw(drawRect);
    }
    
    ofFbo tvScreenView;
    kinectBody * KB;
    
    
    
};