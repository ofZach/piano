

#pragma once



#include "ofMain.h"
#include "kinectBody.h"

#include <algorithm>    // std::shuffle


class tvScreen {
    
    
public:
    
    ofVboMesh sphere;
    
    
    vector < boneConnection > connections;
    vector < boneConnection > connectionsScambled;
    
    
    
    void setup () {
        tvScreenView.allocate(1920, 1080, GL_RGBA, 4);
        tvScreenView.begin();
        ofClear(0,0,0,255);
        tvScreenView.end();
        
        sphere = ofSpherePrimitive(5, 10).getMesh();
        
        SKELETOR::Instance()->setup();
        
        connections = SKELETOR::Instance()->connections;
        connectionsScambled = connections;
        
        std::random_shuffle(connectionsScambled.begin(),connectionsScambled.end());
        
        for (int i = 0; i < connections.size(); i++){
            connectionsScambled[i].a = connections[i].a;
        }
        energy = 0.0;
        
    }
    
    void update( kinectBody * kinectBody){
        KB = kinectBody;
    }
    
    float energy;
    
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
            
            
            ofSeedRandom((int)(energy*100));
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
                        
                        ofRotate(100 * energy, ofRandom(0,3*energy), ofRandom(0,3*energy),ofRandom(0,3*energy));
                        ofTranslate(ofRandom(0, 0+10*scale), ofRandom(0, 0+10*scale), ofRandom(0, 0+10*scale));
                        float ss =ofRandom(1 + 10 * powf(scale, 4.0), 1+20*powf(scale, 4.0));
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