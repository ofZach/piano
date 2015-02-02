


#pragma once


#include "kinectBody.h"


class kinectBodyAnalyser {
    
public:
    
    void analyze ( kinectBody & KB );
    
    ofPoint avgVel (vector < int > pts, kinectBody & KB);
    
    
};
