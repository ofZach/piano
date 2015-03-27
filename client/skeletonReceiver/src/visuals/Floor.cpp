

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


void Floor::setup(int i){

    
    floor.allocate(1024, 1024, GL_RGBA, 4);
    floor.begin();
    ofClear(0, 0, 0, 0);
    floor.end();
    float w = 100;
    idNum = i;
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
    
    squareOptions.setName("Floor Controls "+ofToString(idNum));

    squareOptions.add(bShowDots.set("bShowDots", true));
    squareOptions.add(bShowGrid.set("bShowGrid", true));
    squareOptions.add(bShowButton.set("bShowButton", true));
    squareOptions.add(bPersonPresent.set("bPersonPresent", true));
    squareOptions.add(bFadeLines.set("bFadeLines", false));
    squareOptions.add(bSpawnLines.set("bSpawnLines", false));
    squareOptions.add(buttonPos.set("buttonPos", 0,0, 16));
    squareOptions.add(lineWeight.set("lineWeight", 10, 1, 30));
    squareOptions.add(lineSpeed.set("lineSpeed", 10, 0, 40));
    squareOptions.add(speed.set("speed", 10, 0, 40));
    squareOptions.add(trimLength.set("lineDistnace", 10, 1, 100));
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

ofPoint Floor::getButtonPos(){
    return buttonPoint;
}

void Floor::addLineTrace(){
    if(travelers.size() < 40){
        bSpawnLines = true;
    }
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
    
    if(bSpawnLines){
        bSpawnLines = false;
        spawnLines();
    }
    

    if (bPersonPresent){
        personEnergyLevel = 0.96f * personEnergyLevel + 0.04 * 1.0;
    } else {
        personEnergyLevel = 0.96f * personEnergyLevel + 0.04 * 0.0;
    }
    
    
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
    
    ofSetLineWidth(lineWeight);
    
    floor.begin();
    ofClear(0, 0, 0, 0);
    
    blur.draw();
    
    ofEnableAlphaBlending();
    for (int i = 0; i < travelers.size(); i++){
        travelers[i].draw(personEnergyLevel);
    }
    
    
    if (bShowButton){
        float r;
        GI.getCircle( buttonPos % 4, buttonPos/4, buttonPoint, r);
        ofSetColor(255,255,255,200);
        ofCircle(buttonPoint, r);
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
        GI.getCircle( buttonPos % 4, buttonPos/4, pos, r);
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