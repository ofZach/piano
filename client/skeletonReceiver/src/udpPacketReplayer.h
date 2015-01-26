
#pragma once 
#include "ofMain.h"
#include "ofxUDPManager.h"


class udpPacketReplayer {
    
public:
    
    void setup();
    void parseFolder( string folder );
    void draw(ofRectangle bounds);
    void update();
    
    vector < int > timeStamps;
    map < int, vector < string >  > timeToFileNames;
    ofxUDPManager udpConnection;
    string folderName;
    int loadTimeMillis;
    int lastWho = -1;
};