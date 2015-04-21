#ifndef TVSCREEN_H
#define TVSCREEN_H

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


class tvScreen {
    
    
public:
    


    void setup(int numPlayers, ofRectangle viewport);
    void update( kinectBody * kinectBodyOne, kinectBody * kinectBodyTwo);
    void drawHistoryMan( kinectBody & BODY, int playerID);
    void drawHairyMan( kinectSkeleton & SK, int playerID);
    void drawIntoFbo();
    void draw( ofRectangle drawRect);
    void setStagePos(float x, float y, float z, float size);
    void addParameters(ofParameterGroup & params);
    void setStageParameters(ofParameterGroup params);
    
    void addImplusePlayerOne();

    void addImplusePlayerTwo();

    float nonOfRandom(float x, float y);
    void nonOfSeedRandom(int seed);
    
    ofVboMesh sphere;
    ofVec3f stagePos;
    vector < boneConnection > connections;
    vector < boneConnection > connectionsScambled;
    sitmo::prng_engine eng; /// don't laugh.
    vector < pulseData > pulsesP1;
    vector < pulseData > pulsesP2; // go from 1 to 0
    float twist;
    vector<float> energyHairy;
    vector<float> energyHistory;

    ofFbo tvSkeletonView;
    ofFbo tvGridView;
    kinectBody * playerOneBody;
    kinectBody * playerTwoBody;

    ofParameter<float>  cameraRadius;
    ofParameter<float>  cameraAngle;
    ofParameter<float>  cameraHeight;
    ofParameter<float>  lookAtPointHeight;
    ofParameter<bool>   swayCamera;
    ofParameter<float>  swayRate;
    ofParameter<float>  swayAmount;
    ofParameter<bool>   bDrawHairyManP2;
    ofParameter<bool>   bDrawHairyManP1;
    
    ofParameter<float> stageSize;
    ofParameter<ofVec3f> stageOne;
    ofParameter<ofVec3f> stageTwo;
    ofParameter<float> stageLeftZ;
    ofParameter<float> stageRightX;
    ofParameter<float> stageRightY;
    ofParameter<float> stageRightZ;
    
    
    ofParameter<float>  energyAdder;
    
    ofRectangle tvViewPort;
    
    ofCamera cam;
    
    int numPlayers;
};
#endif