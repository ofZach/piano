

#include "kinectSkeletonAnalyzerInterpreter.h"



void kinectSkeletonAnalyzerInterpreter::setup(){
    
}


void kinectSkeletonAnalyzerInterpreter::analyze( kinectSkeletonAnalyzer & KSA, kinectSkeleton & KS){

    
//    nameToHistoryPlot["elbow-angle-lt"]->setRange(0, 180);
//    nameToHistoryPlot["elbow-angle-rt"]->setRange(0, 180);
//    nameToHistoryPlot["elbow-diff-lt"]->setRange(0, 180);
//    nameToHistoryPlot["elbow-diff-rt"]->setRange(0, 180);
//    
//    nameToHistoryPlot["knee-angle-lt"]->setRange(0, 180);
//    nameToHistoryPlot["knee-angle-rt"]->setRange(0, 180);

    
    if(KSA.nameToHistoryPlot["elbow-angle-lt"]->getValues().back() > 90.0){
        
    }
    
    
}
