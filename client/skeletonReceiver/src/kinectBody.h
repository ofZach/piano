
#pragma once

#include "ofMain.h"
#include "kinectSkeleton.h"


class kinectBody {
    
public:
    
    int bodyId;
    
    vector < kinectSkeleton > history;
    vector < ofPoint > velocity;
    vector < float > velLen;
    vector < ofPoint > accel;
    
    int nFramesHistory;
    
    kinectBody();
    bool addSkeleton( kinectSkeleton & KS);

    
    kinectSkeleton & getLastSkeleton(){
        if (history.size() > 0){
            return history.back();
        } else {
            return;
        }
        
        
    }
};