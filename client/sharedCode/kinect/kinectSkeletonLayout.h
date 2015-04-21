//
//  kinectSkeletonLayout.h
//  skeletonReceiver
//
//  Created by zach on 2/5/15.
//
//

#ifndef KINECTSKELETONLAYOUT_H
#define KINECTSKELETONLAYOUT_H
#pragma once

#include "ofMain.h"
#include "singleton.h"

enum symetricalBodyPts {
	thumb = 0,
	shoulder,
	elbow,
	wrist,
	hand,
	hip,
	knee,
	foot,
	ankle,
	handTip
};

enum nonsymeticalPts{
	spineBase = 0,
	spineMid,
	spineShoulder,
	neck,
	head
} ;

enum side {
	left = 0,
	right,
	center
};

typedef struct {
	vector < int > pointIds;
} bone;

typedef struct boneConnection {
	int a;
	int b;
};


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

	kinectSkeletonLayout();
	~kinectSkeletonLayout();
	void setup();

	int getPointIndex(int name, int side);

	int getIndexFromName(string name);

	bool bIsSkipPoint( int ptId);


};


typedef Singleton < kinectSkeletonLayout >  skelLayoutSinglton;
typedef skelLayoutSinglton SKELETOR;

#endif