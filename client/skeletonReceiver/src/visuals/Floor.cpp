

//
//  Floor.cpp
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#include "Floor.h"

Floor::Floor(){
    
    //32, float shape = .2, int passes = 1, float downsample = .5
    
    blur.setup(1024, 1024, 16, .2, 2, 0.5 );
    blur.begin();
    ofClear(0,0,0,0);
    blur.end();
    
}
Floor::~Floor(){
    
    
    
}
//
void Floor::spawnLines(){
    
    int where =  (int)ofRandom(0, 100) % 16;
    for (int i = 0; i < 20; i++){
        
        traveler t;
        t.GI = &GI;
        t.trimLength = lastTrimLength;
        t.trimLength = lastTrimLength;
        t.speed = lastSpeed;
        t.start(where);
        travelers.push_back(t);

    }
}


void Floor::setup(){
    floor.allocate(1024, 1024, GL_RGBA, 4);
    floor.begin();
    ofClear(0, 0, 0, 0);
    floor.end();
    float w = 100;
    
    GI.setupGrid(ofRectangle(30,30,1024-60,1024-60), 5);
    
    for (int i = 0; i < 20; i++){
        
        traveler t;
        t.GI = &GI;
        t.trimLength = 100;
        
        travelers.push_back(t);
    
        //travelers[i].GI = &GI;
        travelers[i].speed = ofRandom(10,15);
        travelers[i].start( (int)ofRandom(0, 100) % 16);
    }
    
    connectionEnergyLevel.resize(GI.connections.size());
    for (int i = 0; i < connectionEnergyLevel.size(); i++){
        connectionEnergyLevel[i] = 0;
    }
}


float Floor::getWidth(){
    return floor.getHeight();
}
float Floor::getHeight(){
    return floor.getHeight();
}



bool ShouldGo(traveler &p){
    return p.shouldIdie();
}




void Floor::update(){
    
    
    //----------------------------------------------- update travelers
    ofPushStyle();
    for (int i = 0; i < travelers.size(); i++){
        travelers[i].update();
        if( travelers[i].hitFlag == true){
            travelers[i].findNewConnection();
        }
        travelers[i].trimToLength();
    }
    ofRemove(travelers, ShouldGo);
    
    
    for (int i = 0; i < connectionEnergyLevel.size(); i++){
        connectionEnergyLevel[i] -= 0.01;
        if (connectionEnergyLevel[i] < 0) {
            connectionEnergyLevel[i] = 0;
        }
    }
    
    for (int i = 0; i < connectionEnergyLevel.size(); i++){
        if (ofRandom(0,1) > 0.999 ){
            connectionEnergyLevel[i] = 1.0;
        }
    }
    

    bool bShowGrid = squareOptions->getBool("bShowGrid");
    bool bShowButton = squareOptions->getBool("bShowButton");
    float lineWeight = squareOptions->getFloat("lightWeight");
    float trimLength = squareOptions->getFloat("lineDistance");
    float lineSpeed = squareOptions->getFloat("lineSpeed");
    
    string personPresent = "bPersonPresent" + ofToString(idNum);
    
    bool bPresent = squareOptions->getBool(personPresent);
    
    if (bPresent){
        personEnergyLevel = 0.96f * personEnergyLevel + 0.04 * 1.0;
    } else {
        personEnergyLevel = 0.96f * personEnergyLevel + 0.04 * 0.0;
    }
    //cout << personEnergyLevel << endl;
    
    
    
    //bool bPresent = squareOptions->getBool(personPresent);
    
    
    
    bool bFadeLines = squareOptions->getBool("bFadeLines");
    
    
    if (trimLength != lastTrimLength){
        for (int i = 0; i < travelers.size(); i++){
            travelers[i].trimLength=trimLength;
        }
        
    }
    
    if (lastSpeed != lineSpeed){
        for (int i = 0; i < travelers.size(); i++){
            travelers[i].speed=lineSpeed;
        }
        
    }
    
    lastSpeed = lineSpeed;
    lastTrimLength = trimLength;
    
    if (bFadeLines == true){
        for (int i = 0; i < travelers.size(); i++){
            travelers[i].fadeThings();
        }

        
    }
    
    //cout << travelers.size() << endl;
    
    ofSetLineWidth(lineWeight);
    
    string buttonPos = "buttonPos"  + ofToString(idNum);
    int buttonPosVal = squareOptions->getInt(buttonPos);
    
    floor.begin();
    ofClear(0);
    
    blur.draw();
    
    ofEnableAlphaBlending();
    for (int i = 0; i < travelers.size(); i++){
        travelers[i].draw(personEnergyLevel);
    }
    
    
    if (bShowButton){
        float r;
        ofPoint pos;
        GI.getCircle( buttonPosVal % 4, buttonPosVal/4, pos, r);
        ofSetColor(255,255,255,200);
        ofCircle(pos, r);
    }
    
    if (bShowGrid){
        
        ofSetColor(255, 255, 255, 100);
        
        float pulse = sin(ofGetElapsedTimeMillis()/1000.0);
        pulse = pulse * 0.1;
        
        for (int i = 0; i < GI.connections.size(); i++){
            
            int a = GI.connections[i].a;
            int b = GI.connections[i].b;
            
            float val = sin((1.0 - connectionEnergyLevel[i]) * PI);
            ofSetColor(255,255,255,50 + 100 * val + 100 * pulse);
            ofLine( GI.pts[a], GI.pts[b]);
        }
    }

    ofClearAlpha();
    
    
    floor.end();
    
    
    
    blur.begin();
    ofFill();
    ofSetColor(0,0,0,30);
    blur.draw();
    ofRect(0,0,1024, 1024);
    
    // ofClear(0,0,0,255);
    ofEnableAlphaBlending();
    for (int i = 0; i < travelers.size(); i++){
        travelers[i].draw(personEnergyLevel);
    }
    
    
    if (bShowButton){
        float r;
        ofPoint pos;
        GI.getCircle( buttonPosVal % 4, buttonPosVal/4, pos, r);
        ofSetColor(255,255,255,200);
        ofCircle(pos, r);
    }
    
    if (bShowGrid){
        
        ofSetColor(255, 255, 255, 100);
        
        float pulse = sin(ofGetElapsedTimeMillis()/1000.0);
        pulse = pulse * 0.1;
        
        for (int i = 0; i < GI.connections.size(); i++){
            
            int a = GI.connections[i].a;
            int b = GI.connections[i].b;
            
            float val = sin((1.0 - connectionEnergyLevel[i]) * PI);
            ofSetColor(255,255,255,50 + 100 * val + 100 * pulse);
            ofLine( GI.pts[a], GI.pts[b]);
        }
    }
    //ofClearAlpha();
    blur.end();
    
//    floor.begin();
//    ofDisableAlphaBlending();
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    blur.draw();
//    ofEnableAlphaBlending();
//    //ofClearAlpha();
//    floor.end();
    
   
    
    //ofDisableAlphaBlending();
    ofPopStyle();
    
}


void Floor::draw(float x, float y, float w, float h){
    floor.draw(x, y, w, h);
    
}