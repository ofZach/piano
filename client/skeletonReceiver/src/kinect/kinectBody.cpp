

#include "kinectBody.h"


//
//  body();
//  void addSkeleton( kinectSkeleton & KS);
//


kinectBody::kinectBody(){
            
    nFramesHistory = 50; // ?
    velocity.resize(25);                // ?
    velLen.resize(25);
    accel.resize(25);
    
    
}


bool kinectBody::addSkeleton( kinectSkeleton & KS){

    bool bNewFrame = true;
    
    if (history.size() > 0){
        
        vector < ofPoint > & ptsA = KS.pts;
        vector < ofPoint > & ptsB = history.back().pts;
        
        float diff = 0;
        for (int i = 0; i < ptsA.size(); i++){
            diff += (ptsA[i] - ptsB[i]).length();
        }
        if (diff > 0.1){
            bNewFrame = true;
        } else {
            bNewFrame = false;
        }
    }
    
    if (bNewFrame == false) {
        return false;
    }
    
    // calculate velocity;
    if (history.size() > 0){
    
        // -------------------------------------------------- acc setup
        // if we have 2 points, do ACCEL
        vector < ofPoint > oldVel;
        if (history.size() > 1){
            oldVel = velocity;
        }
        
        // -------------------------------------------------- vel
        vector < ofPoint > & ptsA = KS.pts;
        vector < ofPoint > & ptsB = history.back().pts;
        for (int i = 0; i < ptsA.size(); i++){
            velocity[i] = (ptsA[i] - ptsB[i]);
            velLen[i] = velocity[i].length();
        }
        
        // -------------------------------------------------- acc
        // if we have 2 points, do ACCEL
        if (history.size() > 1){
            for (int i = 0; i < velocity.size(); i++){
                accel[i] = (velocity[i] - oldVel[i]);
            }
        }
    }
    
    // store history
    history.push_back(KS);
    if (history.size() > nFramesHistory){
        history.erase(history.begin());
    }

    return true;

}
