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
    void draw();
    void start(int node);
    void findNewConnection();
    void trimToLength();
    
    float trimLength;
    
    int birthday;
    
    bool hitFlag;
    
    
    gridInfo * GI;
    
};
