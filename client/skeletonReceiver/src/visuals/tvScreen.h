

#pragma once



#include "ofMain.h"
#include "kinectBody.h"



class tvScreen {
    
    
public:
    
    void setup () {
        tvScreenView.allocate(1920, 1080, GL_RGBA, 4);
        tvScreenView.begin();
        ofClear(0,0,0,255);
        tvScreenView.end();
    }
    
    void update( kinectBody * kinectBody){
        KB = kinectBody;
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
        
        if (KB != NULL){
            
            kinectSkeleton SK = KB->getLastSkeleton();
            
            for (auto & bone : SKELETOR::Instance()->connections){
                
                cout << bone.a <<  " " << bone.b << endl;
                ofPoint a( SK.pts[bone.a]);
                ofPoint b( SK.pts[bone.b]);
                
                ofSetColor(255,255,255);
                ofLine(a,b);
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