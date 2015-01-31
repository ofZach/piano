

#include "kinectSkeletonAnalyzerInterpreter.h"



void kinectSkeletonAnalyzerInterpreter::setup(){
    
    
    interpreterParams.setName("interpreterParams");
    interpreterParams.add(armExtendedThreshold.set("armExtendedThreshold", 0.8, 0.0, 1.0));
    interpreterParams.add(armNotExtendedThreshold.set("armNotExtendedThreshold", 0.8, 0.0, 1.0));
    interpreterParams.add(armNotBelowThreshold.set("armNotBelowThreshold", 0.5, 0.0, 1.0));
    interpreterParams.add(velOverThresh.set("velOverThresh", 30, 0, 80));
    interpreterParams.add(velBelowThresh.set("velBelowThresh", 10, 0, 80));
    interpreterParams.add(pelvisBlockoutLen.set("pelvisBlockoutLen", 50, 0, 200));
    

    bAbleToTrigger[0] = true;
    bAbleToTrigger[1] = true;
    
}

bool bRemove(hitEvent & e){
    return e.energy < 0.01;
}


ofPoint velSmoothR;
ofPoint velSmoothL;

void kinectSkeletonAnalyzerInterpreter::analyze( kinectSkeletonAnalyzer & KSA, kinectSkeleton & KS){

    
    float yR = KSA.ks2d[  KS.nameToIndex["HandRight"] ].y / 480.0;
    float yL = KSA.ks2d[  KS.nameToIndex["handLeft"] ].y / 480.0;
    float mid = KSA.ks2d[  KS.nameToIndex["SpineMid"] ].y / 480.0;
    
    if (KSA.velocity[KS.nameToIndex["HandRight"]].length() > 0.01){
        velSmoothR = 0.0f * velSmoothR + 1.0 * KSA.velocity[KS.nameToIndex["HandRight"]];
        //cout << velSmoothR.length() << endl;
    }
    
    if (KSA.velocity[KS.nameToIndex["HandLeft"]].length() > 0.01){
        velSmoothL = 0.0f * velSmoothL + 1.0 * KSA.velocity[KS.nameToIndex["HandLeft"]];
    }
    
    
    
    if (velSmoothR.length() > velOverThresh){
        bAbleToTrigger[0] = true;;
    }
    if (velSmoothR.length() < velBelowThresh && bAbleToTrigger[0] && KSA.armRightExtendedPct > armExtendedThreshold){
        
        if ((KSA.ks2d[  KS.nameToIndex["HandRight"] ] - KSA.ks2d[  KS.nameToIndex["SpineBase"] ]).length() > pelvisBlockoutLen){
        
            hitEvent ht;
            ht.eventPt = KSA.ks2d[  KS.nameToIndex["HandRight"] ];
            ht.energy = 1;
            events.push_back(ht);
        }
        bAbleToTrigger[0] = false;
    }
    
    
    if (velSmoothL.length() > velOverThresh){
        bAbleToTrigger[1] = true;;
    }
    if (velSmoothL.length() < velBelowThresh && bAbleToTrigger[1] && KSA.armLeftExtendedPct > armExtendedThreshold){
        
        if ((KSA.ks2d[  KS.nameToIndex["HandLeft"] ] - KSA.ks2d[  KS.nameToIndex["SpineBase"] ]).length() > pelvisBlockoutLen){
            hitEvent ht;
            ht.eventPt = KSA.ks2d[  KS.nameToIndex["HandLeft"] ];
            ht.energy = 1;
            events.push_back(ht);
        }
        bAbleToTrigger[1] = false;
    }
    
    
    
//    
//    if (KSA.armRightExtendedPct > armExtendedThreshold && bAbleToTrigger[0] && yR < mid){
//        hitEvent ht;
//        ht.eventPt = KSA.ks2d[  KS.nameToIndex["HandRight"] ];
//        ht.energy = 1;
//        events.push_back(ht);
//        
//        bAbleToTrigger[0] = false;
//    }
//    
//    if (KSA.armLeftExtendedPct > armExtendedThreshold && bAbleToTrigger[1] && yL < mid){
//        hitEvent ht;
//        ht.eventPt = KSA.ks2d[  KS.nameToIndex["HandLeft"] ];
//        ht.energy = 1;
//        events.push_back(ht);
//        bAbleToTrigger[1] = false;
//    }
//    
//    if (KSA.armRightExtendedPct < armNotExtendedThreshold){
//        bAbleToTrigger[0] = true;
//    }
//    
//    if (KSA.armLeftExtendedPct < armNotExtendedThreshold){
//        bAbleToTrigger[1] = true;
//    }
//    
    for (int i = 0; i< events.size(); i++){
        events[i].energy -= 0.05;
    }
    
    ofRemove(events, bRemove);

}

void kinectSkeletonAnalyzerInterpreter::drawEvents(ofFbo & fbo){
    
    fbo.begin();
    ofNoFill();
    for (int i = 0; i < events.size(); i++){
        ofCircle(events[i].eventPt, events[i].energy * 30.0);
    }
    ofFill();
    
    fbo.end();
    
}
