

#pragma once



#include "ofMain.h"
#include "kinectBody.h"
#include "kinectSkeletonLayout.h"
#include "prng_engine.hh"
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
    
    
    void setup(ofRectangle viewport, ofParameterGroup* params);
    void update( kinectBody * kinectBodyOne, kinectBody * kinectBodyTwo);
    void drawHistoryMan( kinectBody & BODY);
    void drawHairyMan( kinectSkeleton & SK);
    void drawIntoFbo();
    void draw( ofRectangle drawRect);
    void setStagePos(float x, float y, float z, float size);
    void addImpluse();
    
    float nonOfRandom(float x, float y);
    float nonOfSeedRandom(int seed);
    
    ofVboMesh sphere;
    ofVec3f stagePos;
    float stageSize;
    vector < boneConnection > connections;
    vector < boneConnection > connectionsScambled;
    sitmo::prng_engine eng; /// don't laugh.
    vector < pulseData > pulses;    // go from 1 to 0
    float twist;
    bool bDrawHairyMan;
    float energy;

    ofFbo tvSkeletonView;
    ofFbo tvGridView;
    kinectBody * playerOneBody;
    kinectBody * playerTwoBody;

    ofParameterGroup * tvParams;
    
    ofRectangle tvViewPort;
    
    ofCamera cam;
};