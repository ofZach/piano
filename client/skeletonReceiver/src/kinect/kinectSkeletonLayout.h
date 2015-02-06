//
//  kinectSkeletonLayout.h
//  skeletonReceiver
//
//  Created by zach on 2/5/15.
//
//

#ifndef skeletonReceiver_kinectSkeletonLayout_h
#define skeletonReceiver_kinectSkeletonLayout_h


#include "ofMain.h"
#include "singleton.h"

enum {
    thumb,
    shoulder,
    elbow,
    wrist,
    hand,
    hip,
    knee,
    foot,
    ankle,
    handTip
} symetricalBodyPts;

enum {
    spineBase,
    spineMid,
    spineShoulder,
    neck,
    head
} nonsymeticalPts;

enum side {
    left,
    right,
    center
};

typedef struct {
    vector < int > pointIds;
} bone;

typedef struct {
    int a;
    int b;
} boneConnection;


class kinectSkeletonLayout {
    
    
public:
    
    map < int, int > rightEnumsToIndex;
    map < int, int > leftEnumsToIndex;
    map < int, int > centerEnumsToIndex;
    map < string, bone > bones;
    map < string, bone > extraBones;
    vector < string > skipList;
    vector < boneConnection > connections;
    map < string, int > nameToIndex;
    map < int, string> indexToName;
    
    bool bSetup;
    
    kinectSkeletonLayout(){
        bSetup = false;
    }
    
    void setup(){
        
        if (bSetup == true) return;
        
        
        cout << "I GET DONE ONCE HELL YEAH " << endl;
        
        
        bSetup = true;
        
        string jointNames[25] = { "ThumbRight", "SpineBase", "SpineMid", "Neck", "Head", "ShoulderLeft", "ElbowLeft", "WristLeft", "HandLeft", "ShoulderRight", "ElbowRight", "WristRight", "HandRight", "HipLeft", "KneeLeft", "AnkleLeft", "FootLeft", "HipRight", "KneeRight", "AnkleRight", "FootRight", "SpineShoulder", "HandTipLeft", "ThumbLeft", "HandTipRight"};
        
        for (int i = 0; i < 25; i++){
            nameToIndex[jointNames[i]] = i;
            indexToName[i] = jointNames[i];
        }
        
        rightEnumsToIndex[ thumb ]      = nameToIndex["ThumbRight"];
        rightEnumsToIndex[ shoulder ]   = nameToIndex["ShoulderRight"];
        rightEnumsToIndex[ elbow ]      = nameToIndex["ElbowRight"];
        rightEnumsToIndex[ wrist ]      = nameToIndex["WristRight"];
        rightEnumsToIndex[ hand ]       = nameToIndex["HandRight"];
        rightEnumsToIndex[ hip ]        = nameToIndex["HipRight"];
        rightEnumsToIndex[ knee ]       = nameToIndex["KneeRight"];
        rightEnumsToIndex[ foot ]       = nameToIndex["FootRight"];
        rightEnumsToIndex[ ankle ]      = nameToIndex["AnkleRight"];
        rightEnumsToIndex[ handTip ]    = nameToIndex["HandTipRight"];
        
        leftEnumsToIndex[ thumb ]      = nameToIndex["ThumbLeft"];
        leftEnumsToIndex[ shoulder ]   = nameToIndex["ShoulderLeft"];
        leftEnumsToIndex[ elbow ]      = nameToIndex["ElbowLeft"];
        leftEnumsToIndex[ wrist ]      = nameToIndex["WristLeft"];
        leftEnumsToIndex[ hand ]       = nameToIndex["HandLeft"];
        leftEnumsToIndex[ hip ]        = nameToIndex["HipLeft"];
        leftEnumsToIndex[ knee ]       = nameToIndex["KneeLeft"];
        leftEnumsToIndex[ foot ]       = nameToIndex["FootLeft"];
        leftEnumsToIndex[ ankle ]      = nameToIndex["AnkleLeft"];
        leftEnumsToIndex[ handTip ]    = nameToIndex["HandTipLeft"];
        
        centerEnumsToIndex[ spineBase ] = nameToIndex["SpineBase"];
        centerEnumsToIndex[ spineShoulder ] = nameToIndex["SpineShoulder"];
        centerEnumsToIndex[ neck ] = nameToIndex["Neck"];
        centerEnumsToIndex[ head ] = nameToIndex["Head"];
        centerEnumsToIndex[ spineMid ] = nameToIndex["SpineMid"];
        //---------------------------------------------------------------------
        
        skipList.push_back("WristRight");
        skipList.push_back("WristLeft");
        skipList.push_back("HandTipRight");
        skipList.push_back("HandTipLeft");
        skipList.push_back("ThumbLeft");
        skipList.push_back("ThumbRight");
        skipList.push_back("FootLeft");
        skipList.push_back("FootRight");
        
        
        bones["leftLeg"].pointIds.push_back(getPointIndex( hip, ::left ));
        bones["leftLeg"].pointIds.push_back(getPointIndex( knee, ::left ));
        bones["leftLeg"].pointIds.push_back(getPointIndex( ankle, ::left ));
        
        bones["rightLeg"].pointIds.push_back(getPointIndex( hip, ::right ));
        bones["rightLeg"].pointIds.push_back(getPointIndex( knee, ::right ));
        bones["rightLeg"].pointIds.push_back(getPointIndex( ankle, ::right ));
        
        bones["leftArm"].pointIds.push_back(getPointIndex( shoulder, ::left ));
        bones["leftArm"].pointIds.push_back(getPointIndex( elbow, ::left ));
        bones["leftArm"].pointIds.push_back(getPointIndex( hand, ::left ));
        
        bones["rightArm"].pointIds.push_back(getPointIndex( shoulder, ::right ));
        bones["rightArm"].pointIds.push_back(getPointIndex( elbow, ::right ));
        bones["rightArm"].pointIds.push_back(getPointIndex( hand, ::right ));
        
        bones["torso"].pointIds.push_back(getPointIndex( head, ::center ));
        bones["torso"].pointIds.push_back(getPointIndex( neck, ::center ));
        bones["torso"].pointIds.push_back(getPointIndex( spineShoulder, ::center ));
        bones["torso"].pointIds.push_back(getPointIndex( spineMid, ::center ));
        bones["torso"].pointIds.push_back(getPointIndex( spineBase, ::center ));
        
        extraBones["leftShoulder"].pointIds.push_back(getPointIndex( spineShoulder, ::center ));
        extraBones["leftShoulder"].pointIds.push_back(getPointIndex( shoulder, ::left ));
        
        extraBones["rightShoulder"].pointIds.push_back(getPointIndex( spineShoulder, ::center ));
        extraBones["rightShoulder"].pointIds.push_back(getPointIndex( shoulder, ::right ));
        
        extraBones["leftHip"].pointIds.push_back(getPointIndex( spineBase, ::center ));
        extraBones["leftHip"].pointIds.push_back(getPointIndex( hip, ::left ));
        
        extraBones["rightHip"].pointIds.push_back(getPointIndex( spineBase, ::center ));
        extraBones["rightHip"].pointIds.push_back(getPointIndex( hip, ::right ));
        
        
        for(auto boneTemp : bones) {
            vector < int > & ptIds = boneTemp.second.pointIds;
            for (int i = 0; i+1 < ptIds.size(); i++){
                boneConnection BC;
                BC.a = ptIds[i];
                BC.b = ptIds[i+1];
                connections.push_back(BC);
            }
        }
        
        for(auto boneTemp : extraBones) {
            vector < int > & ptIds = boneTemp.second.pointIds;
            for (int i = 0; i+1 < ptIds.size(); i++){
                boneConnection BC;
                BC.a = ptIds[i];
                BC.b = ptIds[i+1];
                connections.push_back(BC);
            }
        }
        
        cout << connections.size() << endl;
    }
    
    int getPointIndex(int name, int side){
        if (side == center){
            if (centerEnumsToIndex.find(name) != centerEnumsToIndex.end()){
                return centerEnumsToIndex[name];
            }
        } else if (side == ::left){
            if (leftEnumsToIndex.find(name) != leftEnumsToIndex.end()){
                return leftEnumsToIndex[name];
            }
        } else if (side == ::right){
            if (rightEnumsToIndex.find(name) != rightEnumsToIndex.end()){
                return rightEnumsToIndex[name];
            }
        }
        return -1;
    }
    
    int getIndexFromName(string name){
        return nameToIndex[name];
    }
    
    bool bIsSkipPoint( int ptId){
        
        if(find(skipList.begin(),skipList.end(),indexToName[ptId]) == skipList.end()){
            return false;
        } else {
            return true;
        }
        
    }
    
    
};


typedef Singleton < kinectSkeletonLayout >  skelLayoutSinglton;
typedef skelLayoutSinglton SKELETOR;




#endif
