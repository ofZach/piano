//
//  kinectButton.cpp
//  skeletonReceiver
//
//  Created by Adam Carlucci on 2015-02-02.
//
//

#include "kinectButton.h"
#include "of3dGraphics.h"

kinectButton::kinectButton() : _radius(10), _triggerScale(0.6), _isActive(false) {
	updateHitbox();
}

#pragma mark - Hitbox

void kinectButton::setRadius(float radius) {
	_radius = radius;
	updateHitbox();
}

void kinectButton::setTriggerScale(float triggerScale) {
	_triggerScale = triggerScale;
	updateHitbox();
}

void kinectButton::updateHitbox() {
	_triggerRadiusSquared = pow(_radius * _triggerScale, 2);
}

#pragma mark - Hit Testing

bool kinectButton::hitTest(const ofVec3f& v) const {
	ofVec3f self = getGlobalPosition();
	float hitDist = _isActive ? pow(_radius, 2) : _triggerRadiusSquared;
	return self.distanceSquared(v) < hitDist;
}

void kinectButton::setIsActive(bool active) {
	_isActive = active;
}

#pragma mark - Debug

void kinectButton::customDraw() {
	ofPushStyle();
	
	ofSetColor(_isActive ? ofColor::lightBlue : ofColor::white, _isActive ? 170 : 60);
	ofDrawSphere(_radius);
	
	ofSetColor(_isActive ? ofColor::red : ofColor::green, 150);
	ofDrawSphere(_radius * _triggerScale);
	
	ofPopStyle();
}
