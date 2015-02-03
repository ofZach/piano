
#pragma once

#include "ofMain.h"
#include "kinectSkeleton.h"
#include "ofxHistoryPlot.h"
#include "ofxMidi.h"

class kinectBody {
    
public:
    
    int bodyId;
    
    vector < ofxHistoryPlot * > historyPlots;
    map <string, ofxHistoryPlot *  > gesturePlots;
    vector<string> gestureNames;
    map < string, ofxHistoryPlot * > nameToHistoryPlot;
    
    vector < map<string, Gesture > >  gestureHistory;
    vector < kinectSkeleton > history;
    vector < ofPoint > velocity;
    vector < float > velLen;
    vector < ofPoint > accel;
    ofParameter <float> smoothing;
    ofParameter <float> scale;
    ofParameter <float> twoDSkelCamDistance;    // I make skeletons 2d
    
    bool drawSkeletonDebug;
    float rightLegVel;
    float leftLegVel;
    float rightArmVel;
    float leftArmVel;
    float torsoAvgVel;
    float totalAvgVel;

    // ---------------------------------------------------
    // make this a parameter
    
    int nFramesHistory;
    
    kinectBody();
    bool addSkeleton( kinectSkeleton & KS);
    void update();
    void drawHistory();
    
    ofFbo historyPlotsFBO;
    ofFbo gestureFBO;
    ofFbo normFbo;
    ofCamera normCam;

    
    // ---------------------------------------------------
    
//                        kinectBody();
//    bool                addSkeleton( kinectSkeleton & KS);
    kinectSkeleton &    getLastSkeleton();
    
    // ---------------------------------------------------

    
    void draw(){
        if (history.size() > 0){
            getLastSkeleton().draw();
        }
    }
    
    void drawDebug(bool bDrawBox){
        if (history.size() > 0){
            getLastSkeleton().drawDebug( bDrawBox );
        }
    }

};