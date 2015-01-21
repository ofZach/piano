#include "ofApp.h"


ofDirectory dir;

//--------------------------------------------------------------
void ofApp::setup(){

    
   dir.listDir("2015-01-21-16-44-12-974");

    cout << dir.size() << endl;
    for (int i = 0; i < dir.size();  i+=27){
        string name = dir.getPath(i);
        cout << name << endl;
        string time = ofSplitString(ofSplitString(name, "/")[1], "_")[0];
        cout << time << endl;
        timeStamps.push_back(ofToInt(time));
        
    }
    
    ofSort(timeStamps);
    
    
    
    //create the socket and set to send to 127.0.0.1:11999
	udpConnection.Create();
	udpConnection.Connect("127.0.0.1",12345);
	udpConnection.SetNonBlocking(true);
    

    
}

int lastWho = -1;
//--------------------------------------------------------------
void ofApp::update(){

    
    int wholeTime = timeStamps[timeStamps.size()-1] + 30;
    int millis = ofGetElapsedTimeMillis() % wholeTime;
    
    int who = -1;
    for (int i = 0; i+1 <timeStamps.size(); i++){
        if (millis >= timeStamps[i] &&
            millis < timeStamps[i+1]){
            who = i;
        }
    }
    
    cout << timeStamps[who] << " " << who << " " << millis << endl;
    
    if (who != lastWho){
            
        
        for (int i= 0; i  < 27; i++){
            string fileName = "2015-01-21-16-44-12-974/" + ofToString(timeStamps[who]) + "_" +ofToString(i) + ".bin";
        
            ofBuffer buff = ofBufferFromFile(fileName);
            udpConnection.Send(buff.getBinaryBuffer(), buff.size());
        }
        
        
        
    }
    lastWho = who;
    
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
