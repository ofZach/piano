#pragma once

#include "ofMain.h"
#include "ofMain.h"
#include "ofxKinectV2OSC.h"
#include "ofxGui.h"
#include "kinectSkeleton.h"
#include "kinectSkeletonAnalyzer.h"
#include "udpPacketReplayer.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxKinectV2OSC kinect;
    Skeleton* skeleton;
    vector<Skeleton>* skeletons;
    
    BodyRenderer renderer;
    
    ofParameter <ofVec3f> scale;
    ofParameter <float> rotationX;
    ofParameter <float> rotationY;
    ofParameter <float> rotationZ;
    ofParameter <ofVec3f> offset;
    
    ofParameter <bool> bUseUdpPlayer;
    ofParameter <bool> bLoadNewUDP;
    
    
    ofParameter <float> cameraHeight;
    ofParameter <float> cameraRadius;
    ofParameter <float> cameraAngle;
    
    ofParameter<float> udpDuration;
    
    ofFbo fooFbo;
    
    ofxPanel gui;
    
    ofCamera cam;
    
    ofPolyline leftHandHistory;
    ofPolyline rightHandHistory;
    ofMatrix4x4 mat;
    
    
    kinectSkeleton KS;
    
    kinectSkeletonAnalyzer KSA;
    udpPacketReplayer UDPR;
};

