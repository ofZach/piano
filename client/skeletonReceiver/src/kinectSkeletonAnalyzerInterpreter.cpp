

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
    
    
    midi.listPorts();
    midi.openPort(0);
    
    
}

bool bRemove(hitEvent & e){
    return e.energy < 0.01;
}


ofPoint velSmoothR;
ofPoint velSmoothL;
ofPoint base ;

ofPoint hands[2];
float vel[2];

void kinectSkeletonAnalyzerInterpreter::analyze( kinectSkeletonAnalyzer & KSA, kinectSkeleton & KS){
    
    //cout << KS.getCenterPoint( spineBase ) << " " << KS.pts[  KS.nameToIndex["SpineBase"] ] << endl;
    
    
    base = KSA.ks2d[  KS.nameToIndex["SpineShoulder"] ];
    hands[0] = KSA.ks2d[  KS.nameToIndex["HandRight"] ];
    hands[1] = KSA.ks2d[  KS.nameToIndex["HandLeft"] ];
    
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
    
    vel[0] = velSmoothR.length();
    vel[1] = velSmoothL.length();
    
    
    if (velSmoothR.length() > velOverThresh){
        bAbleToTrigger[0] = true;;
    }
    if (velSmoothR.length() < velBelowThresh && bAbleToTrigger[0] && KSA.armRightExtendedPct > armExtendedThreshold){
        
        if ((KSA.ks2d[  KS.nameToIndex["HandRight"] ] - KSA.ks2d[  KS.nameToIndex["SpineBase"] ]).length() > pelvisBlockoutLen){
        
            hitEvent ht;
            ht.eventPt = KSA.ks2d[  KS.nameToIndex["HandRight"] ];
            ht.energy = 1;
            
            int note = 36 + 12 * ofMap( ht.eventPt.y, 50, 250,0, 1, true);
            ht.note = note;
            
            events.push_back(ht);
            
            //cout <<ht.eventPt.y << " " <<  note << endl;
            
            midi.sendNoteOn(1, ht.note,  80);
            
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
            
            int note = 36 + 12 * ofMap( ht.eventPt.y, 50, 200,0, 1, true);
            ht.note = note;
            
            events.push_back(ht);
            
            //cout <<ht.eventPt.y << " " <<  note << endl;
            
            midi.sendNoteOn(1, ht.note,  80);
            
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
        
        if (events[i].energy < 0.01){
            midi.sendNoteOff(1, events[i].note);
        }
    }
    
    ofRemove(events, bRemove);

}

//int lastSectionL = -1;
//int lastSectionR = -1;

int lastWho[2];

void kinectSkeletonAnalyzerInterpreter::drawEvents(ofFbo & fbo){
    
    fbo.begin();
    ofNoFill();
    for (int i = 0; i < events.size(); i++){
        ofCircle(events[i].eventPt, events[i].energy * 30.0);
    }
    ofFill();
    
    
    
    //ofPoint pt =
//    
//    vector < ofRectangle > rects;
//    
//    
//    int div = 10;
//    
//    for (int i = 0; i < div; i++){
//        
//        float w = 200;
//        float h = 15;
//        
//        rects.push_back( ofRectangle(base.x + 100, base.y + (i-div/2) * h, w, h));
//        
//        ofNoFill();
//        ofRect(rects[i]);
//       // ofLine(base + dist * ofPoint(cos(angle), sin( angle)), base + (dist+100) * ofPoint(cos(angle), sin( angle)));
//
//    }
//    
//    
//    int who[2];
//    for (int i = 0; i < 2; i++){
//        
//        who[i] = -1;
//        
//        for (int j = 0; j < rects.size(); j++){
//            if (rects[j].inside(hands[i])){
//                who[i] = j;
//            }
//        }
//        
//        float vol = ofMap(vel[i], 0, 10, 0, 1, true);
//        vol =vol *vol;
//        
//        if (who[i] != lastWho[i]){
//            
//            if (lastWho[i] != -1){
//                midi.sendNoteOff(1, 36 + lastWho[i]*2);
//                //cout << 36 + lastWho[i] << endl;
//                
//            }
//            
//        
//            
//            if (who[i] != -1){
//                midi.sendNoteOn(1, 36 + who[i]*2, vol*127);
//                //cout <<  36 + who[i] << endl;
//                
//            }
//            
//            
//            
//            
//        }
//        
//        cout << vol << endl;
//        
//        if (who[i] != -1){
//            //midi.sendPolyAftertouch(1, 36 + who[i]*2, vol * 127);
//        }
//        //float vel =
//        
//        
//        lastWho[i] = who[i];
//        
//        ofDrawBitmapStringHighlight(ofToString(who[i]),hands[i]);
//        
//    }
//    //ofLine(base, base + 100 * ofPoint(cos(angleR), sin(angleR)));
//    
//    
    
    
    
    
    fbo.end();
    
}
