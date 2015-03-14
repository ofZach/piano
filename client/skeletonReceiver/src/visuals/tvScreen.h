

#pragma once



#include "ofMain.h"
#include "kinectBody.h"
#include "kinectSkeletonLayout.h"
#include "prng_engine.hh"
#include "ofxSyphonServer.h"
#include <algorithm>    // std::shuffle

typedef struct {
    float lifetime;
    float noiseSeed;
}
pulseData;

bool removeIfZero(pulseData p) {
    return p.lifetime < 0.01;
}

class tvScreen {
    
    
public:
    
    ofVboMesh sphere;
    ofxSyphonServer skeletonServer;
    ofxSyphonServer gridServer;
    ofVec3f stagePos;
    float stageSize;
    vector < boneConnection > connections;
    vector < boneConnection > connectionsScambled;
    sitmo::prng_engine eng; /// don't laugh.
    
    vector < pulseData > pulses;    // go from 1 to 0
    
    float twist;
    
    //--------------------------------------------------
    float nonOfRandom(float x, float y) {
        return x + ofMap( eng(), 0, 0xFFFFFFFF, 0, 1.0, true) * (y-x);
    }
    
    float nonOfSeedRandom(int seed) {
        eng.seed(seed);
    }
    
    void setStagePos(float x, float y, float z, float size){
        stagePos.set(x, y, z);
        stageSize = size;
    }
    
    void addImpluse(){
        if(pulses.size() < 10){
            pulses.push_back((pulseData){
                .lifetime = 1.0 + 0.2,
                .noiseSeed = ofGetElapsedTimef()
            });
            
        }
    }
    bool bDrawHairyMan;
    
    void setup () {
        tvSkeletonView.allocate(1280, 720, GL_RGBA, 4);
        tvSkeletonView.begin();
        ofClear(0,0,0,255);
        tvSkeletonView.end();
        
        tvGridView.allocate(1280, 720, GL_RGBA, 4);
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
        energy = 0.0;
        
        bDrawHairyMan = false;
        skeletonServer.setName("Piano - TV - Skeleton");
        gridServer.setName("Piano - TV - Grid");
        twist = 0;
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
        
        // calculate twist value from spine
        kinectSkeleton& sk = BODY.getLastSkeleton();
        ofVec2f spineBase = sk.pts[SKELETOR::Instance()->centerEnumsToIndex[::spineBase]];
        ofVec2f spineTop = sk.pts[SKELETOR::Instance()->centerEnumsToIndex[::spineShoulder]];
        float currentTwist = ofMap(spineBase.x - spineTop.x, -30, 30, -1, 1, true);
        twist = ofLerp(twist, currentTwist, 0.1);
        
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
                
                float scale = energy;
                
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
        

        tvGridView.begin();
        ofClear(0,0,0,255);
        //ofClear(0, 0, 0);
        mainViewCam.begin(ofRectangle(ofVec2f(0, 0), tvGridView.getWidth(), tvGridView.getHeight()));
        ofSetColor(255,255,255,127);
        ofSetLineWidth(3);
        ofPushMatrix();
        ofSetColor(100);
        ofNoFill();
        ofRotate(90,1,0,0);
        ofTranslate(stagePos.x - stageSize/2, stagePos.y - stageSize/2, stagePos.z);
        ofRect(0, 0, stageSize, stageSize);
        ofPopMatrix();
        ofFill();
        mainViewCam.end();
        tvGridView.end();
        
        tvSkeletonView.begin();
        ofClear(0,0,0,255);
        //ofClear(0, 0, 0);
        mainViewCam.begin(ofRectangle(ofVec2f(0, 0), tvSkeletonView.getWidth(), tvSkeletonView.getHeight()));
        
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
        tvSkeletonView.end();
        
        skeletonServer.publishTexture(&tvSkeletonView.getTextureReference());
        gridServer.publishTexture(&tvGridView.getTextureReference());
    }
    
    void draw( ofRectangle drawRect){
        ofPushStyle();
        ofSetColor(255,255,255);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        tvGridView.draw(drawRect);
        tvSkeletonView.draw(drawRect);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    }
    
    ofFbo tvSkeletonView;
    ofFbo tvGridView;
    kinectBody * KB;
    
    
    
};