#include "ofApp.h"



string fileName (const std::string& str){
    unsigned found = str.find_last_of("/");
    return str.substr(found+1);
}

string timeOf (const std::string& str){
    unsigned found = str.find_last_of("_");
    return str.substr(0,found);
}


void ofApp::parseFolder( string _folderName ){
    
    folderName = _folderName;
    
    timeStamps.clear();
    timeToFileNames.clear();
    
    
    ofDirectory dir;
    
    dir.listDir(folderName);
    
    //cout << dir.size() << endl;
    
    string realFolderName = fileName(folderName);
    
    
    for (int i = 0; i < dir.size();  i++){
        string name = dir.getPath(i);
        string realFileName = fileName(name);
        string time = timeOf(realFileName);
        
        timeStamps.push_back(ofToInt(time));
        // make this unique (there's likely a better way here)
        std::vector<int>::iterator it;
        it = std::unique (timeStamps.begin(), timeStamps.end());
        timeStamps.resize( std::distance(timeStamps.begin(),it) );
        
        timeToFileNames[ofToInt(time)].push_back( name );
        
    }
    
    ofSort(timeStamps);
    
    loadTimeMillis = ofGetElapsedTimeMillis();
    
}


//--------------------------------------------------------------
void ofApp::setup(){

    
    
    parseFolder( "recordings/dancing" );

	udpConnection.Create();
	udpConnection.Connect("127.0.0.1",12345);
	udpConnection.SetNonBlocking(true);

    loadTimeMillis = 0;

    
}

int lastWho = -1;
//--------------------------------------------------------------
void ofApp::update(){

    
    int wholeTime = timeStamps[timeStamps.size()-1] + 30;
    int millis = (ofGetElapsedTimeMillis()- loadTimeMillis) % wholeTime;
    
    int who = -1;
    for (int i = 0; i+1 <timeStamps.size(); i++){
        if (millis >= timeStamps[i] &&
            millis < timeStamps[i+1]){
            who = i;
        }
    }
    
    if (who != lastWho){
            
        vector < string > & files = timeToFileNames[timeStamps[who]];
        
        for (int i= 0; i  < files.size(); i++){
            string fileName = files[i];
            ofBuffer buff = ofBufferFromFile(fileName);
            udpConnection.Send(buff.getBinaryBuffer(), buff.size());
        }
    }
    lastWho = who;
//
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    ofSetColor(255);
    ofDrawBitmapStringHighlight(folderName, ofPoint(30,30));
    
    
    int wholeTime = timeStamps[timeStamps.size()-1] + 30;
    int millis = (ofGetElapsedTimeMillis()- loadTimeMillis) % wholeTime;
    float pct = (float) millis  / (float) wholeTime;
    
    ofSetColor(255,0,0);
    ofNoFill();
    ofRect(0, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
    ofFill();
    ofRect(0, ofGetHeight()/2, ofGetWidth() * pct, ofGetHeight());
    
    
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
    
    string fileName = dragInfo.files[0];
    parseFolder(fileName);
    
}
