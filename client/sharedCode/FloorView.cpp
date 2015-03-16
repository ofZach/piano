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

void FloorView::setup(ofRectangle projector, ofRectangle main){
    p1Floor.setup();
    p2Floor.setup();
    projectionViewport = projector;
    mainViewport = main;
    projectionFbo.allocate(projectionViewport.width, projectionViewport.height, GL_RGBA, 4);
    setupGUI();
    setupQuadWarp();
    p1Floor.setParamterGroup(&squareOptions);
    p2Floor.setParamterGroup(&squareOptions);
    
    bMainView = false;
}
void FloorView::update(){
    gui.update();
    p1Floor.update();
    p2Floor.update();
    
    
    if(bSaveWarp){
        bSaveWarp = !bSaveWarp;
        warpFloor.save("warp-settings.xml");
        //        warpTwo.save("playerTwo.xml");
        gui.saveSettings("floor-settings.xml");
    }
    
    
    
    projectionFbo.begin();
    ofClear(0, 0, 0, 0);
    
    //    if(dualWarp){
    //
    //        glPushMatrix();
    //        glMultMatrixf( mat1.getPtr() );
    //        {
    //            p1Floor.draw(0, 0, p1Floor.getWidth()/2, p1Floor.getHeight()/2);
    //        }
    //        glPopMatrix();
    //
    //        glPushMatrix();
    //        glMultMatrixf( mat2.getPtr() );
    //        {
    //            p2Floor.draw(0, 0, p2Floor.getWidth()/2, p2Floor.getHeight()/2);
    //        }
    //        glPopMatrix();
    //    }else{
    
    glPushMatrix();
    glMultMatrixf( mat.getPtr() );
    {
        
        
        p1Floor.draw(0, 0, p1Floor.getWidth()/2, p1Floor.getHeight()/2);
        
        
        p2Floor.draw(p2Floor.getWidth()/2+floorOffset, 0, p2Floor.getWidth()/2, p2Floor.getHeight()/2);
    }
    glPopMatrix();
    //    }
    projectionFbo.end();
}
void FloorView::draw(){
    if(bMainView){
        if(bShowWarp){
            warpFloor.show();
        }
    }
    if(!bMainView || !bShowWarp){
        warpFloor.hide();
    }
    projectionFbo.draw(0, 0);
    
}

void FloorView::drawDebug(){
    
    //    mat2 = warpTwo.getMatrix();
    mat = warpFloor.getMatrix();
    
    
    //    if(dualWarp){
    //        warpOne.update();
    //        warpTwo.update();
    //    }else{
    warpFloor.update();
    //    }
    
    
    ofBackground(0, 0, 0);
    ofSetColor(ofColor::slateBlue);
    ofNoFill();
    ofRect(mainViewport.width-p1Floor.getWidth()/3-2.5, 0, p1Floor.getWidth()/3+5, p1Floor.getHeight()/3+5);
    ofSetColor(255, 255, 255);
    ofFill();
    p1Floor.draw(ofGetScreenWidth()-p1Floor.getWidth()/3, 2.5, p1Floor.getWidth()/3, p1Floor.getHeight()/3);
    
    ofSetColor(ofColor::slateBlue);
    ofNoFill();
    ofRect(mainViewport.width-p2Floor.getWidth()/3-2.5, p2Floor.getHeight()/3+20-2.5, p1Floor.getWidth()/3+5, p1Floor.getHeight()/3+5);
    ofSetColor(255, 255, 255);
    ofFill();
    p2Floor.draw(mainViewport.width-p2Floor.getWidth()/3, p2Floor.getHeight()/3+20, p2Floor.getWidth()/3, p2Floor.getHeight()/3);
    
    projectionFbo.draw(0, 0);
    
    warpFloor.draw();
    //    warpOne.draw();
    
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofSetColor(ofColor::slateBlue);
    ofRect(-2.5, -2.5, projectionViewport.width+5, projectionViewport.height+5);
    ofFill();
    ofSetColor(255, 255, 255);
    
    gui.draw();
    
}
void FloorView::drawProjections(){
    projectionFbo.draw(projectionViewport);
}

void FloorView::setupGUI(){
    gui.setup("Floor Controls", ofGetWidth()-300, ofGetHeight()-350, 300, 350, true, false);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    
    squareOptions.setName("Floor");
    squareOptions.add(scale.set("scale", 1,0.1, 10.0));
    squareOptions.add(horizOffset.set("horizOffset", 0, -1500, 1500));
    squareOptions.add(verticalOffset.set("verticalOffset", 0, -1500, 1500));
    squareOptions.add(speed.set("speed", 0.1, 0.01, 0.5));
    squareOptions.add(lightWeight.set("lightWeight", 1,1, 20.0));
    squareOptions.add(lineDistance.set("lineDistance", 20, 1, 100));
    squareOptions.add(bShowWarp.set("bShowWarp", true));
    squareOptions.add(bShowDots.set("bShowDots", true));
    squareOptions.add(bSaveWarp.set("bSaveWarp", true));
    squareOptions.add(floorOffset.set("Stage Offset", 50, 0, 200));
    
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addGroup(squareOptions);
    
    gui.setupEvents();
    gui.enableEvents();
    
    
    gui.loadSettings("floor-settings.xml");
}

void FloorView::setMainView(bool view){
    bMainView = view;
}

void FloorView::setupQuadWarp(){
    
    
    float w = p1Floor.getWidth()+floorOffset;
    float h = p1Floor.getHeight()/2;
    float x = 0;
    float y = 0;
    
    warpFloor.setSourceRect( ofRectangle( x, y, w, h) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warpFloor.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
    warpFloor.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
    warpFloor.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
    warpFloor.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
    warpFloor.setTargetRect(ofRectangle(projectionFbo.getWidth()/10, projectionFbo.getHeight()/10, 8*projectionFbo.getWidth()/10, 8*projectionFbo.getHeight()/10));
    warpFloor.setup();
    
    warpFloor.load("stage-warp.xml");
    
    
    
    //    x = (w+15);
    //
    
    //
    //    warpOne.setSourceRect( ofRectangle(x, y, w, h) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    //    warpOne.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
    //    warpOne.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
    //    warpOne.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
    //    warpOne.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
    //    warpOne.setup();
    //    warpOne.show();
    //
    //    warpOne.load("playerOne.xml");
    //
    //    warpTwo.setSourceRect( ofRectangle(x, y, w, h) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    //    warpTwo.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
    //    warpTwo.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
    //    warpTwo.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
    //    warpTwo.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
    //    warpTwo.setup();
    //    warpTwo.show();
    //    
    //    warpTwo.load("playerTwo.xml");
}