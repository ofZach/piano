

//
//  Floor.cpp
//  skeletonReceiver
//
//  Created by dantheman on 2/4/15.
//
//

#include "Floor.h"

Floor::Floor(){
    
}
Floor::~Floor(){
    
    
    
}
//
//void Floor::spawnLines(){
//    
////    for (int i = 0; i < 20; i++){
////        
////        traveler t;
////        t.GI = &GI;
////        t.trimLength = lastTrimLength;
////        t.trimLength = lastTrimLength;
////        
////        travelers.push_back(t);
////        
////        //travelers[i].GI = &GI;
////        travelers[i].speed = ofRandom(10,15);
////        travelers[i].start( (int)ofRandom(0, 100) % 16);
////    }
//}


void Floor::setup(){
    floor.allocate(1024, 1024, GL_RGBA, 4);
    floor.begin();
    ofClear(0, 0, 0, 0);
    floor.end();
    float w = 100;
    
    GI.setupGrid(ofRectangle(0,0,1024,1024), 4);
    
    for (int i = 0; i < 20; i++){
        
        traveler t;
        t.GI = &GI;
        t.trimLength = 100;
        
        travelers.push_back(t);
    
        //travelers[i].GI = &GI;
        travelers[i].speed = ofRandom(10,15);
        travelers[i].start( (int)ofRandom(0, 100) % 16);
    }
}


float Floor::getWidth(){
    return floor.getHeight();
}
float Floor::getHeight(){
    return floor.getHeight();
}



//bool ShouldGo(traveler &p){
//    return p.shouldIdie();
//}

void Floor::update(){
    
    ofPushStyle();
    
    for (int i = 0; i < travelers.size(); i++){
        travelers[i].update();
        if( travelers[i].hitFlag == true){
            travelers[i].findNewConnection();
            
        }
        travelers[i].trimToLength();
    }
    
    //ofRemove(travelers, ShouldGo);
    

    bool bShowGrid = squareOptions->getBool("bShowGrid");
    bool bShowButton = squareOptions->getBool("bShowButton");
    float lineWeight = squareOptions->getFloat("lightWeight");
    float trimLength = squareOptions->getFloat("lineDistance");
    float lineSpeed = squareOptions->getFloat("lineSpeed");
    //bool bFadeLines = squareOptions->getBool("bFadeLines");
    
    
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
    
//    if (bFadeLines == true){
//        for (int i = 0; i < travelers.size(); i++){
//            travelers[i].fadeThings();
//        }
//
//        
//    }
    
    //cout << travelers.size() << endl;
    
    ofSetLineWidth(lineWeight);
    
    string buttonPos = "buttonPos"  + ofToString(idNum);
    int buttonPosVal = squareOptions->getInt(buttonPos);
    
    floor.begin();
    ofClear(0);
    
    ofEnableAlphaBlending();
    for (int i = 0; i < travelers.size(); i++){
        travelers[i].draw();
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
        
        for (int i = 0; i < GI.connections.size(); i++){
            int a = GI.connections[i].a;
            int b = GI.connections[i].b;
            ofLine( GI.pts[a], GI.pts[b]);
        }
    }
    
    
    
    ofClearAlpha();
    floor.end();
    
    //ofDisableAlphaBlending();
    
    ofPopStyle();
    
}


void Floor::draw(float x, float y, float w, float h){
    floor.draw(x, y, w, h);
    
}