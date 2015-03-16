
#pragma once 

#include "ofMain.h"


class connection{
    
public:
    connection(){
        
    }
    
    connection(int _a, int _b){
        a = _a;
        b = _b;
    }
    int a, b;
};



class gridInfo {
    
public:
    
    vector < ofPoint > pts;
    vector < connection > connections;
    map < int, vector < int > > connectionsPerNode;
    int nDivs;
    
    
    void getCircle(int x, int y, ofPoint & center, float & radius){
        
        x %= (nDivs-1);
        y %= (nDivs-1);
        
        
        int who = x * nDivs + y;
        int who1 = (x+1) * nDivs + y;
        int who2 = x * nDivs + (y+1);
        int who3 = (x+1) * nDivs + (y+1);
        
        ofPoint a = pts[who];
        
        
        radius = (pts[who] - pts[who + 1]).length()/2;
        
        a +=  ofPoint(radius, radius);
        
        
        center = a;
    }
    
    
    void setupGrid( ofRectangle r, int nDivisions ){
        
        nDivs = nDivisions;
        
        for (int i = 0; i < nDivisions; i++){
            for (int j = 0; j < nDivisions; j++){
                
                float pctx = ofMap(i, 0, nDivisions-1, 0, 1, true);
                float pcty = ofMap(j, 0, nDivisions-1, 0, 1, true);
                
                float x = r.x + r.width * pctx;
                float y = r.y + r.height * pcty;
                
                ofPoint newPt(x,y);
                //cout << pctx << endl;
                
                
                pts.push_back(newPt);
                
                if (true){
                    int posMe = j * nDivisions + i;
                    int posAbove = (j+1) * nDivisions + i;
                    int posAside = (j) * nDivisions + (i+1);
                    
                    int posAngle = (j+1) * nDivisions + (i+1);
                    
                    int posAngle2 = (j+1) * nDivisions + (i-1);
                    
                    
                    connection a(posMe, posAbove);
                    connection b(posMe, posAside);
                    connection c(posMe, posAngle);
                    connection d(posMe, posAngle2);
                    
                    
                    if (j < nDivisions - 1) connections.push_back(  a );
                    if (i < nDivisions - 1) connections.push_back(  b );
                    if (i < nDivisions - 1 &&
                        j < nDivisions - 1) connections.push_back(  c );
                    
                    if (i > 0 &&
                        j < nDivisions - 1) connections.push_back(  d );
                    
                    
                }
            }
        }
        
        for (int i = 0; i < connections.size(); i++){
            int a = connections[i].a;
            int b = connections[i].b;
            
            connectionsPerNode[a].push_back(i);
            connectionsPerNode[b].push_back(i);
        }
        
        
    }
    
    
};

