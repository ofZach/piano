//
//  Grid.h
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//
#pragma once
#include "ofMain.h"

class connection {
    
public:
    
    int a,b;
    
    connection(){}
    
    connection(int _a, int _b){
        a = _a;
        b = _b;
    }
};


class connectionMover {
    
public:
    
    
    connection * c;
    vector < ofPoint > * pts;
    
    ofPoint p;
    
    bool bFlip;
    float pct;
    bool bDead;
    float speed;
    
    int lineDistance;
    
    ofPolyline history;
    
    connectionMover(){
        pts = NULL;
        c = NULL;
        lineDistance = 20;
        speed = 0.1;
        bDead = false;
    }
    
    void setConnection( connection * _c, bool _bFlip){
        c = _c;
        bFlip = _bFlip;
        pct = 0;
        bDead = false;
        
    }
    
    void update(){
        
        if (c == NULL) return;
        if (pts == NULL) return;
        
        pct += speed;
        if (pct > 1) pct = 1;
        
        ofPoint a,b;
        if (!bFlip){
            a = (*pts)[c->a];
            b = (*pts)[c->b];
        } else {
            a = (*pts)[c->b];
            b = (*pts)[c->a];
        }
        p = a * (1-pct) + b * pct;
        
        history.addVertex(p);
        while (history.size() > lineDistance){
            history.getVertices().erase(history.getVertices().begin());
        }
        
        if(lineDistance < 1){
            bDead = true;
        }
    }
    
    void draw(){
        history.draw();
    }
    
};


class triangle {
    
public:
    triangle (int _a, int _b, int _c){
        a = _a;
        b = _b;
        c = _c;
        bOn = false;
        impulseEnergy = 0;
        energy = 0;
    }
    
    void update(){
        energy *= 0.93;
        
        impulseEnergy  *= 0.99;
        
//        if (impulseEnergy > 0.01 && ofGetElapsedTimeMillis() - lastFlip > 700){
//            randomFlip(powf(energy, 0.3)* 0.15 + 0.85);
//        }
        
    }
    
    void randomFlip(float pct){
        lastFlip = ofGetElapsedTimeMillis();
        bOn = ofRandom(0, 1) > pct ? !bOn : bOn;
    }
    
    void randomOn(float pct){
        lastFlip = ofGetElapsedTimeMillis();
        bOn = ofRandom(0, 1) > pct ?  true : false;
    }
    
    void draw( vector < ofPoint > & pts){
        
        ofPushStyle();
        if (bOn){
            ofSetColor(255, 255 * powf(impulseEnergy, 0.75));
            ofTriangle (pts[a], pts[b], pts[c]);
        }
        ofPopStyle();
        
    }
    
    void impulse (){
//        if (ofGetElapsedTimeMillis() - lastFlip > 500){
            impulseEnergy = 1;
            randomOn(0.5);
            if (bOn) energy = 1;
//        }
    }
    
    bool bOn;
    int a, b, c;
    float energy = 0;
    float impulseEnergy;
    unsigned long long lastFlip;
};

