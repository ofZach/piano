
#pragma once

#include "ofMain.h"
#include "kinectSkeleton.h"
#include "ofxHistoryPlot.h"

class kinectBody {
    
public:
    
    int bodyId;
    
    vector < ofxHistoryPlot * > historyPlots;
    map < string, ofxHistoryPlot * > nameToHistoryPlot;
    vector < kinectSkeleton > history;
    vector < ofPoint > velocity;
    vector < float > velLen;
    vector < ofPoint > accel;
    ofParameter <float> smoothing;
    ofParameter <float> scale;
    ofParameter <float> twoDSkelCamDistance;    // I make skeletons 2d

    
    int nFramesHistory;
    
    kinectBody();
    bool addSkeleton( kinectSkeleton & KS);
    void update();
    void draw();
    
    ofFbo historyPlotsFBO;
    ofFbo normFbo;
    ofCamera normCam;
    
    kinectSkeleton & getLastSkeleton(){
        if (history.size() > 0){
            return history.back();
        } else {
            return;
        }
        
    }
    
    
    
};