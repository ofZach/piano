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
	ButtonBlock foo;
	foo.isOn = false;
	foo.vel = 0.0;
	_triggerBlock = foo;
	_approachBlock =_triggerBlock;
}

#pragma mark - Hitbox

void kinectButton::setRadius(float width) {
	_radius = width;
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
	_radiusSquared = pow(_radius/2, 2);
	_triggerRadiusSquared = pow(_radius/2 * _triggerScale, 2);
	_approachRadiusSquared = pow(_radius/2 * _approachScale, 2);
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
		_triggerBlock.isOn = isHit;
		_triggerBlock.vel = velocity;
	}

	if(isApproached != _isApproached) {

		_approachBlock.isOn = isHit;
		_approachBlock.vel = velocity;
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

	ofSetColor(ofColor::red, 10);
	ofDrawBox(0, 0, 0, _radius* _approachScale, _radius* _approachScale, _radius* _approachScale);

	ofSetColor(_isTriggered ? ofColor::green : ofColor::red, 50);
	ofDrawBox(0, 0, 0, _radius* _triggerScale, _radius* _triggerScale, _radius* _triggerScale);

	ofPopStyle();
}

#pragma mark - Trigger

void kinectButton::setTriggerBlock(ButtonBlock block) {
	_triggerBlock = (block);
}

void kinectButton::setApproachBlock(ButtonBlock block) {
	_approachBlock = (block);
}
