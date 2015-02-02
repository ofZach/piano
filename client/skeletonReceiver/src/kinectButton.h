#pragma once

#include "ofNode.h"

class kinectButton : public ofNode {
	
public:
	
	kinectButton();
	
	void setRadius(float radius);
	float getRadius() const { return _radius; }
	
	void setTriggerScale(float triggerScale);
	float getTriggerScale() const { return _triggerScale; }
	
	bool hitTest(const ofVec3f& v) const;
	
	void setIsActive(bool active);
	bool getIsActive() const { return _isActive; }
	
private:
	
	void customDraw();
	void updateHitbox();
	
	float _radius;
	float _triggerScale;
	bool _isActive;
	
	float _triggerRadiusSquared;
};
