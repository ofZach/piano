
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
    
    float rightLegVel;
    float leftLegVel;
    float rightArmVel;
    float leftArmVel;
    float torsoAvgVel;
    float totalAvgVel;
    
    // ---------------------------------------------------
    // make this a parameter
    
    int nFramesHistory;
    
    
    // ---------------------------------------------------
                        kinectBody();
    bool                addSkeleton( kinectSkeleton & KS);
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
    
    /*void drawVelocity(){
        
        
        if (history.size() > 0){
            ofPoint handPt = getLastSkeleton().getLeftPoint( hand );
            ofDrawBitmapString("arm vel = " + ofToString(leftArmVel, 3), handPt);
        }

        
    }*/
    
};