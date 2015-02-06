//
//  kinectButton.cpp
//  skeletonReceiver
//
//  Created by Adam Carlucci on 2015-02-02.
//
//

#include "kinectButton.h"
#include "of3dGraphics.h"

kinectButton::kinectButton() : _radius(10), _triggerScale(0.6), _approachScale(1.2), _isTriggered(false), _isApproached(false) {
	updateHitbox();
	_triggerBlock = Block_copy(^(bool b, float f){ });
	_approachBlock = Block_copy(_triggerBlock);
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

void kinectButton::setApproachScale(float approachScale) {
	_approachScale = approachScale;
	updateHitbox();
}

void kinectButton::updateHitbox() {
	_radiusSquared = pow(_radius, 2);
	_triggerRadiusSquared = pow(_radius * _triggerScale, 2);
	_approachRadiusSquared = pow(_radius * _approachScale, 2);
}

#pragma mark - Hit Testing

bool kinectButton::isTriggered(){
    return _isTriggered;
}

void kinectButton::update(const vector< pair<ofVec3f, float> > &points) {
	
	bool isHit = false;
	bool isApproached = false;
	
	float velocity = 0;
	float hitRadiusSquared = _isTriggered ? _radiusSquared : _triggerRadiusSquared;
	
	for(auto& p : points) {
		
		if(hitTest(p.first, hitRadiusSquared)) {
			isHit = true;
			isApproached = true;
			velocity = p.second;
			break;
		} else if(hitTest(p.first, _approachRadiusSquared)) {
			isApproached = true;
			velocity = p.second;
		}
	}
	
	if(isHit != _isTriggered) {
		_triggerBlock(isHit, velocity);
	}
	
	if(isApproached != _isApproached) {
		_approachBlock(isApproached, velocity);
	}
	
	setIsTriggered(isHit);
	setIsApproached(isApproached);
}

bool kinectButton::hitTest(const ofVec3f& v, float distSquared) const {
	return getGlobalPosition().distanceSquared(v) < distSquared;
}

void kinectButton::setIsTriggered(bool isTriggered) {
	_isTriggered = isTriggered;
}

void kinectButton::setIsApproached(bool isApproached) {
	_isApproached = isApproached;
}

#pragma mark - Debug

void kinectButton::customDraw() {
	ofPushStyle();
	
	ofSetColor(ofColor::purple, 10);
	ofDrawSphere(_radius * _approachScale);
	
	ofSetColor(_isTriggered ? ofColor::lightBlue : ofColor::white, _isTriggered ? 70 : 30);
	ofDrawSphere(_radius);
	
	ofSetColor(_isTriggered ? ofColor::red : ofColor::green, 50);
	ofDrawSphere(_radius * _triggerScale);
	
	ofPopStyle();
}

#pragma mark - Trigger

void kinectButton::setTriggerBlock(ButtonBlock block) {
	_triggerBlock = Block_copy(block);
}

void kinectButton::setApproachBlock(ButtonBlock block) {
	_approachBlock = Block_copy(block);
}
