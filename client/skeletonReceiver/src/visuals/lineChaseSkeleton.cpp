//
//  lineChaseSkeleton.cpp
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#include "lineChaseSkeleton.h"


void lineChaseSkeleton::setup(){
    
    gui.setup("tv Controls", ofGetWidth()-600-10, 10, 300, 400);
    gui.addPanel("main control",1, false);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    
    ofParameterGroup p;
    p.setName("lineChaseSkeleton");
    p.add(scale.set("scale", 1,0.1, 10.0));
    p.add(horizOffset.set("horizOffset", 0, -1500, 1500));
    p.add(verticalOffset.set("verticalOffset", 0, -1500, 1500));
    
    p.add(speed.set("speed", 0.1, 0.01, 0.5));
    p.add(lightWeight.set("lightWeight", 1,1, 20.0));
    p.add(lineDistance.set("lineDistance", 20, 1, 100));
    p.add(bShowDots.set("bShowDots", true));
    
    
    
    gui.setWhichPanel(6);
    gui.setWhichColumn(0);
    gui.addGroup(p);
    
    gui.setupEvents();
    gui.enableEvents();
    
    
    gui.loadSettings("controlPanelSettings.xml");
    
    tv.allocate(1280, 720, GL_RGBA, 4);
    tv.begin();
    ofClear(0, 0, 0, 0);
    tv.end();
    
    

    for (int i = 0; i < 27; i++) {
        connections.push_back(connection(i, (i+1)%27));
    }
    
    
    
    //    ofAddListener(ofEvents().update, this, &lineChaseSkeleton::update);
    ofAddListener(ofEvents().draw, this, &lineChaseSkeleton::draw);
    ofAddListener(ofEvents().keyPressed, this, &lineChaseSkeleton::keyPressed);
}

void lineChaseSkeleton::update( kinectBody * kinectBody, ofCamera mainViewCam){
    KB = kinectBody;
    pts.clear();
    for(int i = 0; i < KB->getLastSkeleton().pts.size(); i++){
        if(find(SKELETOR::Instance()->skipList.begin(), SKELETOR::Instance()->skipList.end(),SKELETOR::Instance()->indexToName[i]) == SKELETOR::Instance()->skipList.end()){
            pts.push_back(KB->getLastSkeleton().pts[i]);
        }
    }
    
    for (int i = 0; i < pts.size(); i++){
        for (int j = 0; j <SKELETOR::Instance()->connections.size(); j++){
            
            if (SKELETOR::Instance()->connections[j].a == i ||
                SKELETOR::Instance()->connections[j].b == i ){
                ptToConnections[i].push_back(  j );
            }
        }
    }
    
    
    
    for (int i = 0; i < movers.size(); i++){
        movers[i].speed = speed*1.00001;
        movers[i].lineDistance = movers[i].lineDistance*0.99999;
    }
    
    //int which = (int)ofRandom(0,10000) % SKELETOR::Instance()->connections.size();
    
    
    if(movers.size() < 20){
        int which = (int)ofRandom(0,10000) % SKELETOR::Instance()->connections.size();
        connectionMover C;
        C.pts = &pts;
        C.setConnection(new connection(SKELETOR::Instance()->connections[which].a, SKELETOR::Instance()->connections[which].b), ofRandom(0,1) > 0.5 ? true : false);
        movers.push_back(C);
        movers.back().lineDistance = ofRandom(10, 25);
        movers.back().speed = 0.05;
    }
    
    //    float w = 1;
    //    int ct = 0;
    //    pts[ct++].set(ofPoint(0 * w, 0 * w));
    //    pts[ct++].set(ofPoint(1 * w, 0 * w));
    //    pts[ct++].set(ofPoint(2 * w, 0 * w));
    //    pts[ct++].set(ofPoint(3 * w, 0 * w));
    //
    //
    //    pts[ct++].set(ofPoint(0 * w, 1 * w));
    //    pts[ct++].set(ofPoint(1 * w, 1 * w));
    //    pts[ct++].set(ofPoint(2 * w, 1 * w));
    //    pts[ct++].set(ofPoint(3 * w, 1 * w));
    //
    //    pts[ct++].set(ofPoint(2 * w, 2 * w));
    //    pts[ct++].set(ofPoint(3 * w, 2 * w));
    //
    //
    //    pts[ct++].set(ofPoint(0 * w, 3 * w));
    //    pts[ct++].set(ofPoint(2 * w, 3 * w));
    //    pts[ct++].set(ofPoint(3 * w, 3 * w));
    //
    //    for (int i = 0; i < pts.size(); i++){
    //        pts[i] -= (ofPoint(1.5, 1.5));
    //        pts[i] /= 3.0;
    //        float height = ofMap(pts[i].y, -0.5, 0.5, 0,1);
    //
    //        //cout << height << endl;
    //    }
    //
    //
    //    for (int i = 0; i < pts.size(); i++){
    //        pts[i] *= scale*100;
    //        pts[i] += ofPoint(horizOffset,verticalOffset);
    //
    //    }
    
    gui.update();
    
    
    
    for (int i = 0; i < movers.size(); i++){
        movers[i].update();
    }
    
    for (int i = 0; i < movers.size(); i++){
        if (movers[i].pct > 0.99){
            connection * weJustCameFrom = movers[i].c;
            int idWeJustGotTo = movers[i].bFlip ? movers[i].c->a : movers[i].c->b;
            
            vector < int > goodConnections;
            
            for (int j = 0; j < connections.size(); j++){
                if (connections[j].a == idWeJustGotTo ||
                    connections[j].b == idWeJustGotTo ){
                    
                    if (&connections[j] != weJustCameFrom){
                        goodConnections.push_back(j);
                    }
                }
            }
            
            bool bFlip = false;
            
            if (goodConnections.size() > 0){
                int which = (int)ofRandom(0,100000) % goodConnections.size();
                if (connections[goodConnections[which]].a != idWeJustGotTo){
                    bFlip = true;
                } else {
                    bFlip = false;
                }
                movers[i].setConnection(&(connections[goodConnections[which]]), bFlip);
            }
        }
        if(movers[i].bDead){
            movers.erase(movers.begin()+i);
        }
    }
    
    
    for (int i = 0; i < triangles.size(); i++){
        triangles[i].update();
    }
    
    
    ofEnableAlphaBlending();
    tv.begin();
    mainViewCam.begin(ofRectangle(0, 0, tv.getWidth(), tv.getHeight()));
    ofPushStyle();
    ofSetLineWidth(lightWeight);
    ofClear(0, 0, 0, 0);
    ofPushMatrix();
    
    if (bShowDots){
        for (int i = 0; i < pts.size(); i++){
            ofCircle(pts[i], 3);
        }
    }
    //    ofPushStyle();
    //    for (int i = 0; i < triangles.size(); i++){
    //        triangles[i].draw(pts);
    //    }
    ofPopStyle();
    for (int i = 0; i < movers.size(); i++){
        ofSetColor(255, 255, 255);
        movers[i].draw();
    }
    
    
    
    ofPopMatrix();
    ofPopStyle();
    mainViewCam.end();
    tv.end();
    ofDisableAlphaBlending();
    
    tvTexture.publishTexture(&tv.getTextureReference());
}

void lineChaseSkeleton::addLineTrace(){
    int which = (int)ofRandom(0,10000) % connections.size();
    
    
    for (int i = 0; i < movers.size(); i++){
        
        movers[i].speed = ofRandom(0.1, 0.5)*0.01;
        movers[i].lineDistance = ofRandom(25, 30);
        
    }
    
    connectionMover C;
    C.pts = &pts;
    C.setConnection(&(connections[which]), ofRandom(0,1) > 0.5 ? true : false);
    movers.push_back(C);
    movers.back().lineDistance = lineDistance;
    movers.back().speed = speed;
}

void lineChaseSkeleton::triggerTriangles(){
    if(ofGetElapsedTimeMillis() - lastTrigger > 750){
        for (int i = 0; i < triangles.size(); i++){
            triangles[i].impulse();
        }
        lastTrigger = ofGetElapsedTimeMillis();
    }
}

void lineChaseSkeleton::draw(ofEventArgs &args){
    gui.draw();
}

void lineChaseSkeleton::keyPressed(ofKeyEventArgs &key){
    if(key.key == 'l'){
        gui.toggleView();
    }
    if(key.key == 's'){
        gui.saveSettings();
    }
}

