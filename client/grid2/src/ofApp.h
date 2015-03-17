#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"


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
    
    float speed;
    
    int lineDistance;
    
    ofPolyline history;
    
    connectionMover(){
        pts = NULL;
        c = NULL;
        lineDistance = 20;
        speed = 0.1;
    }
    
    void setConnection( connection * _c, bool _bFlip){
        c = _c;
        bFlip = _bFlip;
        pct = 0;
        
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
        //pts.push_back(
        
    }
    
    void draw(){
        history.draw();
        //ofCircle(p, 10);
        
        //if (!bFlip) ofCircle( (*pts)[c->b], 20);
        //else ofCircle( (*pts)[c->a], 20);
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
        
        impulseEnergy  *= 0.93;
        
        if (impulseEnergy > 0.01 && ofGetElapsedTimeMillis() - lastFlip > 50){
            randomFlip(powf(impulseEnergy, 0.3)* 0.15 + 0.85);
        }
        
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
            ofSetColor(255, 255 * powf(impulseEnergy, 0.5));
            ofTriangle (pts[a], pts[b], pts[c]);
        }
        ofPopStyle();
        
    }
    
    void impulse (){
        impulseEnergy = 1;
        randomOn(0.5);
        if (bOn) energy = 1;
    }
    
    bool bOn;
    int a, b, c;
    float energy = 0;
    float impulseEnergy;
    unsigned long long lastFlip;
};



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    
        vector < ofPoint > pts;
        vector < connection > connections;
        
        map < int, vector < int >  > ptToConnections;
    
        vector < connectionMover > movers;
    
    
        vector < triangle > triangles;
    
    

        //connectionMover  C;
        ofParameterGroup        squareOptions;
        ofParameter <float>     scale;
        ofParameter <float>     horizOffset;
        ofParameter <float>     verticalOffset;
        ofParameter <float>     verticalScaleFactor;
        ofParameter <bool>      bShowDots;
        ofParameter <float>     lightWeight;
        ofParameter <float>     lineDistance;
        ofParameter <float>     speed;
    
        ofxControlPanel gui;
    
    
    ofFbo fbo;
};
