//
//  FloorView.cpp
//  visualsDebug
//
//  Created by dantheman on 3/14/15.
//
//

#include "FloorView.h"


FloorView::FloorView(){
    
}
FloorView::~FloorView(){
    
}

void FloorView::setup(int num, ofRectangle projector, ofRectangle main){
    p1Floor.setup(0);
    if(num > 1){
        p2Floor.setup(1);
    }
    projectionViewport = projector;
    mainViewport = main;
    numPlayers = num;
    projectionFbo.allocate(projectionViewport.width, projectionViewport.height, GL_RGBA, 4);
    setupGUI();

    
    p1Floor.idNum = 0;
    p2Floor.idNum = 1;
    
    
    bMainView = false;
    
    setupQuadWarp();
    

    
    font.loadFont(OF_TTF_MONO, 35);
}
void FloorView::update(){
    p1Floor.update();
    if(numPlayers > 1){
        p2Floor.update();
    }
    mat = warpFloor.getMatrix();
    warpFloor.update();
    
    if(bSaveWarp){
        bSaveWarp = !bSaveWarp;
        warpFloor.save("warp-settings.xml");
    }
    
    
    
    
    if(bMainView){
        if(bShowWarp){
            warpFloor.show();
        }
    }
    if(!bMainView || !bShowWarp){
        warpFloor.hide();
    }
}
void FloorView::draw(ofRectangle viewport){
    
    
    ofSetColor(255, 255, 255);
    projectionFbo.draw(viewport);
}

void FloorView::drawStageCalibration(){
    int width=p1Floor.getWidth()+floorOffset;
    int height=p1Floor.getHeight()/2;
    int squareSize = 10;
    int counter=0;
    
    ofPushStyle();
    for (int j=0; j<height;j=j+squareSize) {
        
        for (int i=0; i<width; i=i+squareSize*2){
            if (counter%2==0){
                ofSetColor(0);
                ofFill();
                ofRect(i, j, squareSize, squareSize);
                
                
                ofSetColor(255);
                ofFill();
                ofRect(i+squareSize, j, squareSize, squareSize);
                ofNoFill();
                ofCircle(i+squareSize/2, j+squareSize/2, squareSize/4*sin(i+1/150+5*ofGetElapsedTimef())+squareSize/4);
                ofSetColor(0);
                ofCircle(i+squareSize+squareSize/2, j+squareSize/2, squareSize/4*sin((i+1)/200+5*ofGetElapsedTimef())+squareSize/4);
                
                
            }else {
                ofSetColor(255);
                ofFill();
                ofRect(i, j, squareSize, squareSize);
                
                
                ofSetColor(0);
                ofFill();
                ofRect(i+squareSize, j, squareSize, squareSize);
                ofNoFill();
                ofCircle(i+squareSize/2, j+squareSize/2, squareSize/4*sin((i+1)/175+5*ofGetElapsedTimef())+squareSize/4);
                ofSetColor(255);
                ofCircle(i+squareSize+squareSize/2, j+squareSize/2, squareSize/4*sin((i+1)/250+5*ofGetElapsedTimef())+squareSize/4);
            }
            
        }
        counter++;
    }
    
    ofPopStyle();
}


void FloorView::setPlayerOne(bool populated){
    p1Floor.bPersonPresent = populated;
}
void FloorView::setPlayerTwo(bool populated){
    p2Floor.bPersonPresent = populated;
}
ofPoint FloorView::getPlayerOneButtonPos(){
    return p1Floor.getButtonPos();
}
ofPoint FloorView::getPlayerTwoButtonPos(){
    return p2Floor.getButtonPos();
}
void FloorView::drawDebug(){
    
    ofBackground(0, 0, 0);
    ofPushStyle();
    ofSetColor(255, 255, 255);
    projectionFbo.draw(0, 0);
    
    ofSetColor(ofColor::orangeRed);
    warpFloor.draw();
    
    
    ofNoFill();
    ofSetColor(ofColor::slateBlue);
    ofRect(-2.5, -2.5, projectionViewport.width+5, projectionViewport.height+5);
    ofFill();
    ofSetColor(255, 255, 255);
    ofPopStyle();
}

void FloorView::drawProjectorCalibration(){
    ofPushStyle();
    ofSetLineWidth(1);
    
    
    int height = projectionViewport.height;
    int width = projectionViewport.width;
    
    ofFill();
    ofSetColor(255, 0, 0,100);
    ofRect(0,0,width, height/3);
    
    ofSetColor(0, 255, 0,100);
    ofRect(0,height/3,width, height/3);
    
    ofSetColor(0, 0, 255,100);
    ofRect(0,height-(height/3),width, height/3);
    
    ofSetCircleResolution(40);
    ofSetColor(255);
    int spacing = 50;
    for (int i = 0; i<width; i=i+spacing) {
        if ((i/spacing)%3) {
            ofSetLineWidth(1);
            ofSetColor(127);
        }
        else{
            ofNoFill();
            ofSetColor(255);
            ofCircle(i+1.5*spacing, height/2, (height/3)/2);
            ofRect(i+1.5*spacing, height/2, (height/3)/2,(height/3)/2);
            ofRect(i+1.5*spacing-((height/3)/2), (height/2)- (height/3)/2, (height/3)/2,(height/3)/2);
            ofSetLineWidth(2);
            ofSetColor(100, 100, ofMap(i,0,width,0,255, true));
            ofLine(i,0,i+spacing*3,height);
            ofLine(i+spacing*3,0,i,height);
        }
        ofLine(i, 0, i, height);
        ofDrawBitmapString(ofToString(i), i+5,20);
    }
    //Text
    for (int i = 0; i<height; i=i+spacing) {
        if ((i/spacing)%3) {
            ofSetLineWidth(1);
        }
        else{
            ofSetLineWidth(5);
        }
        ofLine(0, i, width, i);
        ofDrawBitmapString(ofToString(i), 20,i+5);
        ofDrawBitmapString(ofToString(i), 200,i+5);
    }
    ofPopStyle();
}

void FloorView::drawProjections(){
    
    ofEnableAlphaBlending();
    projectionFbo.begin();
    ofClear(0, 0, 0, 0);
    
    if(bDrawCalibration){
        drawProjectorCalibration();
    }
    
    glPushMatrix();
    glMultMatrixf( mat.getPtr() );
    {
        
        if(!bDrawCalibration){
            
            //cout << p1Floor.getWidth()/2 << endl;
            ofSetColor(255, 255, 255);
            p1Floor.draw(0, 0, p1Floor.getWidth()/2, p1Floor.getHeight()/2);
            if(numPlayers > 1){
                p2Floor.draw(p2Floor.getWidth()/2+floorOffset, 0, p2Floor.getWidth()/2, p2Floor.getHeight()/2);
            }
        }else{
            drawStageCalibration();
            ofSetColor(255, 0, 255);
            font.drawString("FRONT OF STAGE", projectionFbo.getWidth()/2, 50);
            font.drawString("FRONT OF STAGE", 0, 50);
        }
    }
    glPopMatrix();
    //    }
    projectionFbo.end();
    ofDisableAlphaBlending();

    
    projectionFbo.draw(projectionViewport);
    if(bDrawCalibration){
        
        ofPushStyle();
        ofNoFill();
        ofRect(projectionViewport);
        ofFill();
        ofPopStyle();
    }
}

void FloorView::setupGUI(){
    
    squareOptions.setName("Floor");
    
    squareOptions.add(bShowWarp.set("bShowWarp", true));
    squareOptions.add(bSaveWarp.set("bSaveWarp", true));
    squareOptions.add(bDrawCalibration.set("bDrawCalibration", true));
    squareOptions.add(floorOffset.set("Stage Offset", 50, 0, 200));
    
    squareOptions.add(p1Floor.squareOptions);
    squareOptions.add(p2Floor.squareOptions);
}

void FloorView::setMainView(bool view){
    bMainView = view;
}
bool FloorView::isMain(){
    return bMainView;
}

void FloorView::setupQuadWarp(){
    

    float w = p1Floor.getWidth()+floorOffset;
    if(numPlayers == 1){
        w = p1Floor.getWidth()/2;
    }
    float h = p1Floor.getHeight()/2;
    float x = 0;
    float y = 0;
    
    warpFloor.setSourceRect( ofRectangle( x, y, w, h) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warpFloor.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
    warpFloor.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
    warpFloor.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
    warpFloor.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
    if(numPlayers == 2){
    warpFloor.setTargetRect(ofRectangle(projectionFbo.getWidth() - projectionFbo.getWidth()/10, projectionFbo.getHeight() - projectionFbo.getHeight()/10, -p1Floor.getWidth()+floorOffset, -p1Floor.getHeight()/2));
    }else{
            warpFloor.setTargetRect(ofRectangle(projectionFbo.getWidth() - projectionFbo.getWidth()/10, projectionFbo.getWidth()- projectionFbo.getHeight()/10, -p1Floor.getWidth()/2, -p1Floor.getHeight()/2));
    }
    warpFloor.setup();
    warpFloor.load("warp-settings"+ofToString(numPlayers)+".xml");
 
}