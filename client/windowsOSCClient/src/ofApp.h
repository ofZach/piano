#ifndef OFAPP_H
#define OFAPP_H
#pragma once

#include "ofMain.h"
#include "Piano.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    Piano views;
    
    void addLineTracePlayerOne();
    void triggerTrianglePlayerOne();
    void addImpulsePlayerOne();
    
    void addLineTracePlayerTwo();
    void triggerTrianglePlayerTwo();
    void addImpulsePlayerTwo();
};
#endif