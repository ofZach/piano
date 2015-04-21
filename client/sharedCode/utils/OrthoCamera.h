#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H
#pragma once

#include "ofMain.h"

// Ortho camera is a custom
//	camera we've created in
//  this example
//
// We inherit from ofCamera

class orthoCamera : public ofCamera {
	public:
		orthoCamera();
		void begin(ofRectangle rect);
		float scale;
};
#endif