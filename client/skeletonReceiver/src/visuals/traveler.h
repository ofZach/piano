#pragma once

#include "ofMain.h"
#include "gridInfo.h"



class traveler {
    
public:
    
    int currentConnection;
    int targetNode;
    bool bCurrentConnectionReversed;
    float speed;
    vector < ofPoint > trail;
    ofPoint pos;
    
    bool shouldIdie();
    void fadeThings();
    
    
    void update();
    void draw( float alpha );
    void start(int node);
    void findNewConnection();
    void trimToLength();
    
    float trimLength;
    
    int birthday;
    
    bool hitFlag;
    
    float energy;
    
    gridInfo * GI;
    
};
