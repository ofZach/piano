

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
    float nonOfSeedRandom(int seed);
    
    ofVboMesh sphere;
    ofVec3f stagePos;
    vector < boneConnection > connections;
    vector < boneConnection > connectionsScambled;
    sitmo::prng_engine eng; /// don't laugh.
    vector < pulseData > pulsesP1;
    vector < pulseData > pulsesP2; // go from 1 to 0
    float twist;
    float energy;

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
    ofParameter<float> stageLeftX;
    ofParameter<float> stageLeftY;
    ofParameter<float> stageLeftZ;
    ofParameter<float> stageRightX;
    ofParameter<float> stageRightY;
    ofParameter<float> stageRightZ;
    
    ofRectangle tvViewPort;
    
    ofCamera cam;
    
    int numPlayers;
};