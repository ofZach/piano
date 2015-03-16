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
    
    p1Floor.idNum = 0;
    p2Floor.idNum = 1;
    
    
    bMainView = false;
}
void FloorView::update(){
    
    
//    if (squareOptions.getBool("spawnLines0")){
//        p1Floor.spawnLines();
//        spawnLines0 = false;
//        
//    }
//    
//    if (squareOptions.getBool("spawnLines1")){
//        p2Floor.spawnLines();
//        
//        spawnLines1 = false;
//        
//    }
    
    
    gui.update();
    p1Floor.update();
    p2Floor.update();
    mat = warpFloor.getMatrix();
    warpFloor.update();
    
    if(bSaveWarp){
        bSaveWarp = !bSaveWarp;
        warpFloor.save("warp-settings.xml");
        //        warpTwo.save("playerTwo.xml");
        gui.saveSettings("floor-settings.xml");
    }
    
    
    ofEnableAlphaBlending();
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
    
    if(bDrawCalibration){
        drawProjectorCalibration();
    }
    
    glPushMatrix();
    glMultMatrixf( mat.getPtr() );
    {
        
        if(!bDrawCalibration){
            
            //cout << p1Floor.getWidth()/2 << endl;
            
            //p1Floor.draw(0, 0, p1Floor.getWidth()/2, p1Floor.getHeight()/2);
            p2Floor.draw(p2Floor.getWidth()/2+floorOffset, 0, p2Floor.getWidth()/2, p2Floor.getHeight()/2);
        }else{
            drawStageCalibration();
        }
    }
    glPopMatrix();
    //    }
    projectionFbo.end();
    ofDisableAlphaBlending();
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

void FloorView::drawControlPanel(){
    
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
    gui.setup("Floor Controls", mainViewport.width-300, mainViewport.height-350, 300, 350, true, false);
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    
    squareOptions.setName("Floor");
    
    squareOptions.add(lightWeight.set("lightWeight", 1,1, 20.0));
    squareOptions.add(lineDistance.set("lineDistance", 50, 1, 500));
    squareOptions.add(lineSpeed.set("lineSpeed", 5, 0.1, 30));
    
    squareOptions.add(bShowWarp.set("bShowWarp", true));
    squareOptions.add(bShowDots.set("bShowDots", true));
    squareOptions.add(bShowGrid.set("bShowGrid", true));
    squareOptions.add(bShowButton.set("bShowButton", true));
    
//    squareOptions.add(bFadeLines.set("bFadeLines", false));
//    squareOptions.add(spawnLines0.set("spawnLines0", false));
//    squareOptions.add(spawnLines1.set("spawnLines1", false));
//    
    
    
    
    squareOptions.add(buttonPos0.set("buttonPos0", 0,0, 16));
    squareOptions.add(buttonPos1.set("buttonPos1", 0,0, 16));
    
    //--------------------------------------------
    //ofParameter <float> lineSpeed;  //lastSpeed
    

    squareOptions.add(bSaveWarp.set("bSaveWarp", true));
    squareOptions.add(bDrawCalibration.set("bDrawCalibration", true));
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
bool FloorView::isMain(){
    return bMainView;
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
    
    warpFloor.load("warp-settings.xml");
    
    
    
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