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

void FloorView::setup(){
    floorTexture.setup();
    setupGUI();
    floorTexture.setParamterGroup(squareOptions);
}
void FloorView::update(){
    gui.update();
     floorTexture.update();
}
void FloorView::draw(){
    floorTexture.draw();
    gui.draw();
    
    
}

void FloorView::setupGUI(){
    gui.setup("Floor Controls", ofGetWidth()-600-10, 10, 300, 400, true, false);
    
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
    
    gui.setWhichPanel(0);
    gui.setWhichColumn(0);
    gui.addGroup(squareOptions);
    
    gui.setupEvents();
    gui.enableEvents();
    
    
    gui.loadSettings("floorTexture.xml");
}