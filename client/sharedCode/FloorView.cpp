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

void FloorView::setup(ofRectangle viewport){
    p1Floor.setup();
    p2Floor.setup();
    projectionViewport = viewport;
    projectionFbo.allocate(projectionViewport.width, projectionViewport.height, GL_RGBA, 4);
    setupGUI();
    setupQuadWarp();
    p1Floor.setParamterGroup(&squareOptions);
    p2Floor.setParamterGroup(&squareOptions);
    
    
}
void FloorView::update(){
    gui.update();
    p1Floor.update();
    p2Floor.update();
    warpOne.update();
    warpTwo.update();
    
    if(bSaveWarp){
        bSaveWarp = !bSaveWarp;
        warpOne.save("playerOne.xml");
        warpTwo.save("playerTwo.xml");
        gui.saveSettings("floorTexture.xml");
    }
    
    
    mat2 = warpTwo.getMatrix();
    mat = warpOne.getMatrix();
    
}
void FloorView::draw(){
    ofBackground(0, 0, 0);
    ofSetColor(ofColor::slateBlue);
    ofRect(ofGetWindowWidth()-p1Floor.getWidth()/3-2.5, 0, p1Floor.getWidth()/3+5, p1Floor.getHeight()/3+5);
    ofSetColor(255, 255, 255);
    p1Floor.draw(ofGetWindowWidth()-p1Floor.getWidth()/3, 2.5, p1Floor.getWidth()/3, p1Floor.getHeight()/3);
    
    ofSetColor(ofColor::slateBlue);
    ofRect(ofGetWindowWidth()-p2Floor.getWidth()/3-2.5, p2Floor.getHeight()/3-2.5, p1Floor.getWidth()/3+5, p1Floor.getHeight()/3+5);
    ofSetColor(255, 255, 255);
    p2Floor.draw(ofGetWindowWidth()-p2Floor.getWidth()/3, p2Floor.getHeight()/3, p2Floor.getWidth()/3, p2Floor.getHeight()/3);
    
    mat2 = warpTwo.getMatrix();
    mat = warpOne.getMatrix();
    
    
    
    ofSetColor(255, 255, 255);
    
    projectionFbo.begin();
    ofClear(0, 0, 0, 0);
    glPushMatrix();
    glMultMatrixf( mat.getPtr() );
    {
        p1Floor.draw(0, 0, p1Floor.getWidth()/2, p1Floor.getHeight()/2);
    }
    glPopMatrix();
    
    
    glPushMatrix();
    glMultMatrixf( mat2.getPtr() );
    {
        p2Floor.draw(p2Floor.getWidth()/2+15, 0, p2Floor.getWidth()/2, p2Floor.getHeight()/2);
    }
    glPopMatrix();
    
    ofSetColor(ofColor::slateBlue);
    warpTwo.draw();
    warpOne.draw();
    ofSetColor(255, 255, 255);
    projectionFbo.end();
    
    projectionFbo.draw(0, 0);
    ofNoFill();
    ofSetColor(ofColor::slateBlue);
    ofRect(-2.5, -2.5, projectionViewport.width+5, projectionViewport.height+5);
    ofFill();
    ofSetColor(255, 255, 255);
    projectionFbo.draw(projectionViewport);
    
    gui.draw();
    
}

void FloorView::setupGUI(){
    gui.setup("Floor Controls", 0, ofGetHeight()-400, 300, 400, true, false);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    
    squareOptions.setName("Floor");
    squareOptions.add(scale.set("scale", 1,0.1, 10.0));
    squareOptions.add(horizOffset.set("horizOffset", 0, -1500, 1500));
    squareOptions.add(verticalOffset.set("verticalOffset", 0, -1500, 1500));
    
    squareOptions.add(speed.set("speed", 0.1, 0.01, 0.5));
    squareOptions.add(lightWeight.set("lightWeight", 1,1, 20.0));
    squareOptions.add(lineDistance.set("lineDistance", 20, 1, 100));
    squareOptions.add(bShowDots.set("bShowDots", true));
    squareOptions.add(bSaveWarp.set("bSaveWarp", true));
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addGroup(squareOptions);
    
    gui.setupEvents();
    gui.enableEvents();
    
    
    gui.loadSettings("floorTexture.xml");
    
}

void FloorView::setupQuadWarp(){
    
    
    float w = p1Floor.getWidth()/2;
    float h = p1Floor.getHeight()/2;
    float x = 0;
    float y = 0;
    
    warpOne.setSourceRect( ofRectangle( x, y, w, h) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warpOne.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
    warpOne.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
    warpOne.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
    warpOne.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
    warpOne.setup();
    warpOne.show();
    
    warpOne.load("playerOne.xml");
    
    x = (w+15);
    
    warpTwo.setSourceRect( ofRectangle(x, y, w, h) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warpTwo.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
    warpTwo.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
    warpTwo.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
    warpTwo.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
    warpTwo.setup();
    warpTwo.show();
    
    warpTwo.load("playerTwo.xml");
}