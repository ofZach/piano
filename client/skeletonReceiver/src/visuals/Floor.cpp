//
//  Floor.cpp
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#include "Floor.h"

void Floor::setup(){
    
    gui.setup("Floor Controls", ofGetWidth()-600-10, 10, 300, 400);
    gui.addPanel("main control",1, false);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    
    ofParameterGroup p;
    p.setName("Floor");
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
    
    floor.allocate(1280, 720, GL_RGBA, 4);
    floor.begin();
    ofClear(0, 0, 0, 0);
    floor.end();
    //    connections.push_back( connection(4,1));
    //    connections.push_back( connection(5,2));
    //    connections.push_back( connection(6,3));
    
    
    
    
    // .  .   .   .
    //
    //
    //
    //
    
    
    float w = 100;
    
    pts.push_back(ofPoint(0 * w, 0 * w));
    pts.push_back(ofPoint(1 * w, 0 * w));
    pts.push_back(ofPoint(2 * w, 0 * w));
    pts.push_back(ofPoint(3 * w, 0 * w));
    
    
    pts.push_back(ofPoint(0 * w, 1 * w));
    pts.push_back(ofPoint(1 * w, 1 * w));
    pts.push_back(ofPoint(2 * w, 1 * w));
    pts.push_back(ofPoint(3 * w, 1 * w));
    
    pts.push_back(ofPoint(2 * w, 2 * w));
    pts.push_back(ofPoint(3 * w, 2 * w));
    
    
    pts.push_back(ofPoint(0 * w, 3 * w));
    pts.push_back(ofPoint(2 * w, 3 * w));
    pts.push_back(ofPoint(3 * w, 3 * w));
    
    
    connections.push_back( connection(0,1));
    connections.push_back( connection(1,2));
    connections.push_back( connection(2,3));
    
    connections.push_back( connection(4,5));
    connections.push_back( connection(5,6));
    connections.push_back( connection(6,7));
    
    connections.push_back( connection(0,4));
    connections.push_back( connection(1,5));
    connections.push_back( connection(2,6));
    connections.push_back( connection(3,7));
    
    
    connections.push_back( connection(5,0));
    connections.push_back( connection(6,1));
    connections.push_back( connection(7,2));
    
    connections.push_back( connection(6,8));
    connections.push_back( connection(7,9));
    connections.push_back( connection(8,9));
    connections.push_back( connection(6,9));        // diag
    
    
    connections.push_back( connection(8,11));
    connections.push_back( connection(9,12));
    connections.push_back( connection(11,12));
    connections.push_back( connection(8,12));      // diag
    
    connections.push_back( connection(4,10));
    connections.push_back( connection(10,11));
    
    
    triangles.push_back(triangle(0,4,1));
    triangles.push_back(triangle(4,1,5));
    triangles.push_back(triangle(5,2,1));
    triangles.push_back(triangle(6,5,2));
    triangles.push_back(triangle(6,7,2));
    triangles.push_back(triangle(3,7,2));
    triangles.push_back(triangle(8,6,9));
    triangles.push_back(triangle(7,6,9));
    triangles.push_back(triangle(8,9,12));
    triangles.push_back(triangle(11,10,4));
    triangles.push_back(triangle(11,4,6));
    triangles.push_back(triangle(11,8,12));
    
    
    
    for (int i = 0; i < pts.size(); i++){
        for (int j = 0; j < connections.size(); j++){
            
            if (connections[j].a == i ||
                connections[j].b == i ){
                ptToConnections[i].push_back(  j );
            }
        }
    }
    
    ofAddListener(ofEvents().update, this, &Floor::update);
    ofAddListener(ofEvents().draw, this, &Floor::draw);
    ofAddListener(ofEvents().keyPressed, this, &Floor::keyPressed);
    
    floorServer.setName("Piano - Floor Projections");
}

void Floor::update(ofEventArgs &args){
    for (int i = 0; i < movers.size(); i++){
        
        movers[i].speed = speed*1.00001;
        movers[i].lineDistance = movers[i].lineDistance*0.99999;
        
    }
    
    int which = (int)ofRandom(0,10000) % connections.size();
    
    
    if(movers.size() < 2){
        connectionMover C;
        C.pts = &pts;
        C.setConnection(&(connections[which]), ofRandom(0,1) > 0.5 ? true : false);
        movers.push_back(C);
        movers.back().lineDistance = ofRandom(50, 100);
        movers.back().speed = 0.05;
    }
    
    float w = 1;
    int ct = 0;
    pts[ct++].set(ofPoint(0 * w, 0 * w));
    pts[ct++].set(ofPoint(1 * w, 0 * w));
    pts[ct++].set(ofPoint(2 * w, 0 * w));
    pts[ct++].set(ofPoint(3 * w, 0 * w));
    
    
    pts[ct++].set(ofPoint(0 * w, 1 * w));
    pts[ct++].set(ofPoint(1 * w, 1 * w));
    pts[ct++].set(ofPoint(2 * w, 1 * w));
    pts[ct++].set(ofPoint(3 * w, 1 * w));
    
    pts[ct++].set(ofPoint(2 * w, 2 * w));
    pts[ct++].set(ofPoint(3 * w, 2 * w));
    
    
    pts[ct++].set(ofPoint(0 * w, 3 * w));
    pts[ct++].set(ofPoint(2 * w, 3 * w));
    pts[ct++].set(ofPoint(3 * w, 3 * w));
    
    for (int i = 0; i < pts.size(); i++){
        pts[i] -= (ofPoint(1.5, 1.5));
        pts[i] /= 3.0;
        float height = ofMap(pts[i].y, -0.5, 0.5, 0,1);
        
        //cout << height << endl;
    }
    
    
    for (int i = 0; i < pts.size(); i++){
        pts[i] *= scale*100;
        pts[i] += ofPoint(horizOffset,verticalOffset);
        
    }
    
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
    floor.begin();
    ofPushStyle();
    ofSetLineWidth(lightWeight);
    ofClear(0, 0, 0, 0);
    ofPushMatrix();
    
    if (bShowDots){
        for (int i = 0; i < pts.size(); i++){
            ofCircle(pts[i], 3);
        }
    }
    ofPushStyle();
    for (int i = 0; i < triangles.size(); i++){
        triangles[i].draw(pts);
    }
    ofPopStyle();
    for (int i = 0; i < movers.size(); i++){
        ofSetColor(255, 255, 255);
        movers[i].draw();
    }
    

    
    ofPopMatrix();
    ofPopStyle();
    floor.end();
    ofDisableAlphaBlending();
    
    floorServer.publishTexture(&floor.getTextureReference());
}

void Floor::addLineTrace(){
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

void Floor::triggerTriangles(){
    if(ofGetElapsedTimeMillis() - lastTrigger > 750){
        for (int i = 0; i < triangles.size(); i++){
            triangles[i].impulse();
        }
        lastTrigger = ofGetElapsedTimeMillis();
    }
}

void Floor::draw(ofEventArgs &args){
    gui.draw();
}

void Floor::keyPressed(ofKeyEventArgs &key){
    if(key.key == 'l'){
        gui.toggleView();
    }
    if(key.key == 's'){
        gui.saveSettings();
    }
}

