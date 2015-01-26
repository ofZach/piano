//
//  udpPacketReplayer.cpp
//  skeletonReceiver
//
//  Created by zach on 1/26/15.
//
//

#include "udpPacketReplayer.h"

string fileName (const std::string& str){
    unsigned found = str.find_last_of("/");
    return str.substr(found+1);
}

string timeOf (const std::string& str){
    unsigned found = str.find_last_of("_");
    return str.substr(0,found);
}


void udpPacketReplayer::setup() {
    udpConnection.Create();
	udpConnection.Connect("127.0.0.1",12345);
	udpConnection.SetNonBlocking(true);
    loadTimeMillis = 0;
};

void udpPacketReplayer::parseFolder( string folder ){
 
    folderName = folder;
    
    timeStamps.clear();
    timeToFileNames.clear();
    
    ofFile file(folder);
    
    if (!file.isDirectory() || !file.exists())  return;
    
    
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

void udpPacketReplayer::update(){

    if (timeStamps.size() > 0){
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
                cout << fileName << endl;
                
                udpConnection.Send(buff.getBinaryBuffer(), buff.size());
            }
        }
        lastWho = who;
    }
    
}

void udpPacketReplayer::draw(ofRectangle bounds){
    
    
    
    ofSetColor(255,255,255,100);
    ofRect(bounds);
    
    ofSetColor(255);
    
    
    if (timeStamps.size() > 0){
        ofDrawBitmapStringHighlight(folderName, ofPoint(bounds.x,bounds.y+20));
        
        int wholeTime = timeStamps[timeStamps.size()-1] + 30;
        int millis = (ofGetElapsedTimeMillis()- loadTimeMillis) % wholeTime;
        float pct = (float) millis  / (float) wholeTime;
        
        ofSetColor(255,0,0);
        ofNoFill();
        ofRect(bounds.x, bounds.y + bounds.height/2, bounds.width,  bounds.height/2);
        ofFill();
        ofRect(bounds.x, bounds.y + bounds.height/2, bounds.width * pct,  bounds.height/2);
    } else {
        
        ofDrawBitmapStringHighlight("no data", ofPoint(bounds.x,bounds.y+20));
        
    }
    
}