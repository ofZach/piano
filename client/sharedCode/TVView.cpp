//
//  TVView.cpp
//  tvDebug
//
//  Created by dantheman on 3/16/15.
//
//

#include "TVView.h"

TVView::TVView(){
    
}

TVView::~TVView(){
    
}


void TVView::setup(ofRectangle tv, ofRectangle main){
    tvViewPort = tv;
    mainViewPort = main;
    TV.setup(tvViewPort);
}
void TVView::update(){
    
}
void TVView::draw(){
    TV.draw(tvViewPort);
}
void TVView::drawControlPanel(){
    
}

void TVView::setMainScreen(bool main){
    
}
bool TVView::isMain(){
    return bMainView;
}