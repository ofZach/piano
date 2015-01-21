#include "ofApp.h"



class ofxOscMessageStr : public ofxOscMessage {
    
public:
    
    static ofxOscSender sender;
    
    ofxOscMessageStr(){
        
    }
    
    ofBuffer toBuffer(){
        
        static const int OUTPUT_BUFFER_SIZE = 327680;
        char buffer[OUTPUT_BUFFER_SIZE];
        osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
        p << osc::BeginBundleImmediate;
        p<< osc::BeginMessage( getAddress().c_str() );
        for ( int i=0; i< getNumArgs(); ++i )
        {
            if ( getArgType(i) == OFXOSC_TYPE_INT32 )
                p << getArgAsInt32( i );
            else if ( getArgType(i) == OFXOSC_TYPE_INT64 )
                p << (osc::int64)getArgAsInt64( i );
            else if ( getArgType( i ) == OFXOSC_TYPE_FLOAT )
                p << getArgAsFloat( i );
            else if ( getArgType( i ) == OFXOSC_TYPE_STRING )
                p << getArgAsString( i ).c_str();
            else if ( getArgType( i ) == OFXOSC_TYPE_BLOB ){
                ofBuffer buff = getArgAsBlob(i);
                osc::Blob b(buff.getBinaryBuffer(), (unsigned long)buff.size());
                p << b;
            }else
            {
                ofLogError("ofxOscSender") << "appendMessage(): bad argument type " << getArgType( i );
                assert( false );
            }
        }
        p << osc::EndMessage;
        p << osc::EndBundle;
        ofBuffer temp;
        temp.append(p.Data(), p.Size());
        return temp;
        
    }
    
    void fromOfBuffer(){
        
        
        
    }
    
    
};





//--------------------------------------------------------------
void ofApp::setup(){

    
    recvr.setup(12345);
    
}

//--------------------------------------------------------------
void ofApp::update(){

    //cout << ofGetFrameRate() << endl;
    
    int timeStamp = ofGetElapsedTimeMillis();
    int count = 0;
    
    vector < ofBuffer > buffers;
    while (recvr.hasWaitingMessages()){
        
        ofxOscMessage message;
        ofxOscMessageStr messageStr;
        recvr.getNextMessage(&message);
        
        messageStr.copy(message);
        ofBuffer b = messageStr.toBuffer();
        buffers.push_back(messageStr.toBuffer());
        //ofBufferToFile(file, b, true);
        
        count ++;
        
    }
    
    if (buffers.size() == 27 ||
        buffers.size() == 54 ){
        
        
        for (int i = buffers.size()-27; i < buffers.size(); i++){
            string file = folder + "/"  + ofToString((int)(timeStamp - ofRecordTimeStart)) + "_" + ofToString(i) +  ".bin";
            ofBufferToFile(file, buffers[i], true);
        }
    } else {
        
        cout << buffers.size() << endl;
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    if(bRcording){
        ofBackground(255,0,0);
    } else {
        ofBackground(0,255,0);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    if (key == ' '){
        bRcording = !bRcording;
        
        if (bRcording){
            folder = ofGetTimestampString();
            ofDirectory f(folder);
            f.create();
            ofRecordTimeStart = ofGetElapsedTimeMillis();
        }
    }
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
