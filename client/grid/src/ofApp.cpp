#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    gui.setup("controls", ofGetWidth()-300-10, 10, 300, 400);
    gui.addPanel("main control",1, false);
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addLabel("hello");

    
    ofParameterGroup p;
    p.setName("");
    p.add(scale.set("scale", 1,0.1, 10.0));
    p.add(horizOffset.set("horizOffset", 0, -1500, 1500));
    p.add(verticalOffset.set("verticalOffset", 0, -1500, 1500));
    p.add(verticalScaleFactor.set("verticalScaleFactor", 1, 0.1, 10.0));
    
    p.add(speed.set("speed", 0.1, 0.01, 0.5));
    p.add(lightWeight.set("lightWeight", 1,1, 20.0));
    p.add(lineDistance.set("lineDistance", 20, 1, 100));
    p.add(bShowDots.set("bShowDots", true));
    
    
    
    
    gui.addGroup(p);
    
    gui.setupEvents();
    gui.enableEvents();
    //    connections.push_back( connection(4,1));
    //    connections.push_back( connection(5,2));
    //    connections.push_back( connection(6,3));
    
    
    
    
    // .  .   .   .
    //
    //
    //
    //
    
    
    float w = 100;
    
    pts.push_back(ofPoint(0 * w, 0 * w));
    pts.push_back(ofPoint(1 * w, 0 * w));
    pts.push_back(ofPoint(2 * w, 0 * w));
    pts.push_back(ofPoint(3 * w, 0 * w));
    
    
    pts.push_back(ofPoint(0 * w, 1 * w));
    pts.push_back(ofPoint(1 * w, 1 * w));
    pts.push_back(ofPoint(2 * w, 1 * w));
    pts.push_back(ofPoint(3 * w, 1 * w));
    
    pts.push_back(ofPoint(2 * w, 2 * w));
    pts.push_back(ofPoint(3 * w, 2 * w));
    
    
    pts.push_back(ofPoint(0 * w, 3 * w));
    pts.push_back(ofPoint(2 * w, 3 * w));
    pts.push_back(ofPoint(3 * w, 3 * w));
    
    
    connections.push_back( connection(0,1));
    connections.push_back( connection(1,2));
    connections.push_back( connection(2,3));
    
    connections.push_back( connection(4,5));
    connections.push_back( connection(5,6));
    connections.push_back( connection(6,7));
    
    connections.push_back( connection(0,4));
    connections.push_back( connection(1,5));
    connections.push_back( connection(2,6));
    connections.push_back( connection(3,7));
    
    
    connections.push_back( connection(5,0));
    connections.push_back( connection(6,1));
    connections.push_back( connection(7,2));
    
    connections.push_back( connection(6,8));
    connections.push_back( connection(7,9));
    connections.push_back( connection(8,9));
    connections.push_back( connection(6,9));        // diag
    
    
    connections.push_back( connection(8,11));
    connections.push_back( connection(9,12));
    connections.push_back( connection(11,12));
    connections.push_back( connection(8,12));      // diag
    
    connections.push_back( connection(4,10));
    connections.push_back( connection(10,11));
    
    
    triangles.push_back(triangle(0,4,1));
    triangles.push_back(triangle(4,1,5));
    triangles.push_back(triangle(5,2,1));
    triangles.push_back(triangle(6,5,2));
    triangles.push_back(triangle(6,7,2));
    triangles.push_back(triangle(3,7,2));
    triangles.push_back(triangle(8,6,9));
    triangles.push_back(triangle(7,6,9));
    triangles.push_back(triangle(8,9,12));
    triangles.push_back(triangle(11,10,4));
    triangles.push_back(triangle(11,4,6));
    triangles.push_back(triangle(11,8,12));

    
    
    for (int i = 0; i < pts.size(); i++){
        for (int j = 0; j < connections.size(); j++){
            
            if (connections[j].a == i ||
                connections[j].b == i ){
                ptToConnections[i].push_back(  j );
            }
        }
    }
    
    fbo.allocate(ofGetWidth()-100, ofGetHeight()-100, GL_RGBA, 4);

    
    int x = ( ofGetWidth()  - fbo.getWidth()  ) * 0.5;   // center on screen.
    int y = ( ofGetHeight() - fbo.getHeight() ) * 0.5;   // center on screen.
    int fboW = fbo.getWidth() ;
    int fboH = fbo.getHeight();
    
    
    warper.setSourceRect( ofRectangle( 0, 0, ofGetWidth(), ofGetHeight() ) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warper.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
    warper.setTopRightCornerPosition( ofPoint( x + fboW, y ) );        // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomLeftCornerPosition( ofPoint( x, y + fboH ) );      // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomRightCornerPosition( ofPoint( x + fboW, y + fboH ) ); // this is position of the quad warp corners, centering the image on the screen.
    warper.setup();
    
    warper.load();
    
    receiver.setup(6667);
    

    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    for (int i = 0; i < movers.size(); i++){
        
        movers[i].speed = speed*1.00001;
        movers[i].lineDistance = movers[i].lineDistance*0.99999;
        
    }
    
     int which = (int)ofRandom(0,10000) % connections.size();
    
    
    
    
    if(movers.size() < 2){
        connectionMover C;
        C.pts = &pts;
        C.setConnection(&(connections[which]), ofRandom(0,1) > 0.5 ? true : false);
        movers.push_back(C);
        movers.back().lineDistance = ofRandom(50, 100);
        movers.back().speed = 0.05;
    }
    
    float w = 1;
    int ct = 0;
    pts[ct++].set(ofPoint(0 * w, 0 * w));
    pts[ct++].set(ofPoint(1 * w, 0 * w));
    pts[ct++].set(ofPoint(2 * w, 0 * w));
    pts[ct++].set(ofPoint(3 * w, 0 * w));
    
    
    pts[ct++].set(ofPoint(0 * w, 1 * w));
    pts[ct++].set(ofPoint(1 * w, 1 * w));
    pts[ct++].set(ofPoint(2 * w, 1 * w));
    pts[ct++].set(ofPoint(3 * w, 1 * w));
    
    pts[ct++].set(ofPoint(2 * w, 2 * w));
    pts[ct++].set(ofPoint(3 * w, 2 * w));
    
    
    pts[ct++].set(ofPoint(0 * w, 3 * w));
    pts[ct++].set(ofPoint(2 * w, 3 * w));
    pts[ct++].set(ofPoint(3 * w, 3 * w));
    
    for (int i = 0; i < pts.size(); i++){
        pts[i] -= (ofPoint(1.5, 1.5));
        pts[i] /= 3.0;
        float height = ofMap(pts[i].y, -0.5, 0.5, 0,1);
        
        //cout << height << endl;
        pts[i].x *=  ofMap(height, 0, 1, 1, verticalScaleFactor);
    }
    
    
    for (int i = 0; i < pts.size(); i++){
        pts[i] *= scale*100;
        pts[i] += ofPoint(horizOffset,verticalOffset);
        
    }
    
    gui.update();
    
    
    
    for (int i = 0; i < movers.size(); i++){
        movers[i].update();
    }
    
    for (int i = 0; i < movers.size(); i++){
        if (movers[i].pct > 0.99){
            connection * weJustCameFrom = movers[i].c;
            int idWeJustGotTo = movers[i].bFlip ? movers[i].c->a : movers[i].c->b;
            
            vector < int > goodConnections;
            
            for (int j = 0; j < connections.size(); j++){
                if (connections[j].a == idWeJustGotTo ||
                    connections[j].b == idWeJustGotTo ){
                    
                    if (&connections[j] != weJustCameFrom){
                        goodConnections.push_back(j);
                    }
                }
            }
            
            bool bFlip = false;
            
            if (goodConnections.size() > 0){
                int which = (int)ofRandom(0,100000) % goodConnections.size();
                if (connections[goodConnections[which]].a != idWeJustGotTo){
                    bFlip = true;
                } else {
                    bFlip = false;
                }
                movers[i].setConnection(&(connections[goodConnections[which]]), bFlip);
            }
        }
        if(movers[i].bDead){
            movers.erase(movers.begin()+i);
        }
    }
    
    
    for (int i = 0; i < triangles.size(); i++){
        triangles[i].update();
    }
    
    warper.update();
    
    
    ofEnableAlphaBlending();
    fbo.begin();
    ofPushStyle();
    ofSetLineWidth(lightWeight);
    ofClear(0, 0, 0, 0);
    ofPushMatrix();

    if (bShowDots){
        for (int i = 0; i < pts.size(); i++){
            ofCircle(pts[i], 3);
        }
    }
    
    for (int i = 0; i < movers.size(); i++){
        movers[i].draw();
    }
    
    for (int i = 0; i < triangles.size(); i++){
        triangles[i].draw(pts);
    }

    ofPopMatrix();
    ofPopStyle();
    fbo.end();
    ofDisableAlphaBlending();
    
    if(warper.isShowing()){
        mat = warper.getMatrix();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    ofBackground(0);
    
    
    //======================== get our quad warp matrix.
    
    //======================== use the matrix to transform our fbo.
    
    glPushMatrix();
    glMultMatrixf( mat.getPtr() );
    {
        fbo.draw( 0, 0);
    }
    glPopMatrix();
    
    syphonOut.publishTexture(&fbo.getTextureReference());
    
    warper.draw();
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 's'){
        gui.saveSettings("settings.xml");
    }
    
    if (key == ' '){
        movers.clear();
    }
    
    if (key == 'z'){
        int which = (int)ofRandom(0,10000) % connections.size();
        
        
        for (int i = 0; i < movers.size(); i++){
            
            movers[i].speed = speed*1.00001;
            movers[i].lineDistance = movers[i].lineDistance*0.99999;
            
        }
        
        
        connectionMover C;
        C.pts = &pts;
        C.setConnection(&(connections[which]), ofRandom(0,1) > 0.5 ? true : false);
        movers.push_back(C);
        movers.back().lineDistance = lineDistance;
        movers.back().speed = speed;
        
        
        
    }
    
    if (key == 'f'){
        ofToggleFullscreen();
    }
    
    if( key == 'l' ){
        warper.toggleShow();
        gui.toggleView();
        warper.save();
    }
    
    if (key == 't'){
        for (int i = 0; i < triangles.size(); i++){
            triangles[i].impulse();
        }
    }
}

void ofApp::oscWorkHorse(){
    while(receiver.hasWaitingMessages()){
        ofxOscMessage b;
        receiver.getNextMessage(&b);
        if(b.getAddress() == "/triggerNote"){
            speed = ofMap(b.getArgAsFloat(0), 0, 127, 0.1, 1);
            int which = (int)ofRandom(0,10000) % connections.size();
            
            connectionMover C;
            C.pts = &pts;
            C.setConnection(&(connections[which]), ofRandom(0,1) > 0.5 ? true : false);
            movers.push_back(C);
            movers.back().lineDistance = lineDistance;
            movers.back().speed = speed;
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
