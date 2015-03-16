//
//  traveler.cpp
//  gridNewWorld
//
//  Created by zach on 3/16/15.
//
//

#include "traveler.h"



void traveler::update(){
    
    bool bFlip = !bCurrentConnectionReversed;
    ofPoint a, b;
    
    if (bFlip){
        a = GI->pts[GI->connections[currentConnection].a];
        b = GI->pts[GI->connections[currentConnection].b];
        
    } else {
        a = GI->pts[GI->connections[currentConnection].b];
        b = GI->pts[GI->connections[currentConnection].a];
    }
    
    ofPoint dirOfTravel = b-a;
    dirOfTravel.normalize();
    ofPoint finalPos = b;
    
    float distance = (finalPos - pos).length();
    
    if (distance <= speed*1.5){     // would I ever miss this??
        pos = finalPos;
        hitFlag = true;
        trail.push_back(pos);
    } else {
        pos += dirOfTravel * speed;
        trail.push_back(pos);
    }
    
    ofSetCircleResolution(100);
}

void traveler::draw(){
    
    //    ofLine(GI->pts[GI->connections[currentConnection].a] + ofPoint(0,10,0),
    //           GI->pts[GI->connections[currentConnection].b] + ofPoint(0,10,0));
    //
    //    ofNoFill();
    //    ofBeginShape();
    //    for (int i = 0; i < trail.size(); i++){
    //        ofVertex(trail[i]);
    //    }
    //    ofEndShape();
    
    ofMesh m;
    m.setMode(OF_PRIMITIVE_LINE_STRIP);
    for (int i = 0; i< trail.size(); i++){
        m.addVertex(trail[i]);
        float pct = (i / (float)trail.size());
        //pct *= pct;
        
        m.addColor(ofColor(255,255,255,pct * 200));
    }
    m.draw();
    
}

void traveler::findNewConnection(){
    
    int lastConnection = currentConnection;
    
    vector < int > con = GI->connectionsPerNode[targetNode];
    
    vector < int > conGood;
    for (int i = 0; i < con.size(); i++){
        if (con[i] != lastConnection) {
            conGood.push_back(con[i]);
        }
    }
    
    int whichOne = conGood[ (int)ofRandom(0,100000) % conGood.size()];
    if (GI->connections[whichOne].a == targetNode){
        currentConnection = whichOne;
        bCurrentConnectionReversed = false;
        targetNode = GI->connections[whichOne].b;
    } else {
        currentConnection = whichOne;
        bCurrentConnectionReversed = true;
        targetNode = GI->connections[whichOne].a;
    }
    
    hitFlag = false;
    
    
}


void traveler::fadeThings(){
    
    //speed *= 0.99;
    trimLength *= 0.96;
    speed *= 0.99;
    
}



bool traveler::shouldIdie(){
    
    if (ofGetElapsedTimeMillis() - birthday < 1000) return false;
    
    else {
        
        float len = 0;
         ofPoint lastPt = trail[trail.size()-1];
        for (int i = trail.size()-1; i >= 0; i--){
            len += ( lastPt - trail[i] ).length();
        }
        //cout << len << endl;
        if (len < 10) return true;
        else return false;
        
        
    }
    
}

void traveler::trimToLength(){
    
    if (trail.size() < 2) return;
    
    float targetLen = trimLength;
    float len = 0;
    int cut = -1;
    ofPoint lastPt = trail[trail.size()-1];
    for (int i = trail.size()-1; i >= 0; i--){
        len += ( lastPt - trail[i] ).length();
        lastPt = trail[i];
        if (len > targetLen){
            cut = i;
            break;
        }
        //cout << len << endl;
    }
    
    if (cut != -1){
        trail.erase(trail.begin(), trail.begin()+cut);
    }
    
    
    
    
}



void traveler::start( int node ){
    birthday = ofGetElapsedTimeMillis();
    
    trail.clear();
    hitFlag = false;
    pos = GI->pts[node];
    vector < int > con = GI->connectionsPerNode[node];
    int whichOne = con[ (int)ofRandom(0,100000) % con.size()];
    if (GI->connections[whichOne].a == node){
        currentConnection = whichOne;
        bCurrentConnectionReversed = false;
        targetNode = GI->connections[whichOne].b;
    } else {
        currentConnection = whichOne;
        bCurrentConnectionReversed = true;
        targetNode = GI->connections[whichOne].a;
    }
}
