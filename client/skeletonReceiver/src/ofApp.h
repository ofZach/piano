#pragma once

#include "ofMain.h"
#include "ofMain.h"
#include "ofxKinectV2OSC.h"
#include "kinectSkeleton.h"
#include "kinectSkeletonAnalyzer.h"
#include "kinectSkeletonAnalyzerInterpreter.h"
#include "udpPacketReplayer.h"
#include "ofxControlPanel.h"

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
    
    ofParameterGroup skeletonTransform;
    ofParameter <float> scaleX;
    ofParameter <float> scaleY;
    ofParameter <float> scaleZ;
    ofParameter <float> rotationX;
    ofParameter <float> rotationY;
    ofParameter <float> rotationZ;
    ofParameter <float> offsetX;
    ofParameter <float> offsetY;
    ofParameter <float> offsetZ;
    
    ofParameterGroup dataPlayer;
    ofParameter <bool> bUseUdpPlayer;
    ofParameter <bool> bLoadNewUDP;
    ofParameter<float> udpDuration;
    
    ofParameterGroup cameraControl;
    ofParameter <float> cameraHeight;
    ofParameter <float> cameraRadius;
    ofParameter <float> cameraAngle;
    
    ofFbo fooFbo;
    
    ofxControlPanel gui;
    
    ofCamera cam;
    
    ofPolyline leftHandHistory;
    ofPolyline rightHandHistory;
    ofMatrix4x4 mat;
    
    ofParameter <string> status;
    
    kinectSkeleton KS;
    kinectSkeletonAnalyzerInterpreter KSAI;
    kinectSkeletonAnalyzer KSA;
    udpPacketReplayer UDPR;
};

