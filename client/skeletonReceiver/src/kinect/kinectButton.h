#pragma once

#include "ofNode.h"

typedef void (^ButtonBlock)(bool isOn, float vel);

class kinectButton : public ofNode {
	
public:
	
	kinectButton();
	
	void update(const std::vector< std::pair<ofPoint, float> > &points);
	bool hitTest(const ofVec3f& v) const;
	
	void setRadius(float radius);
	float getRadius() const { return _radius; }
	
	void setTriggerScale(float triggerScale);
	float getTriggerScale() const { return _triggerScale; }
	
	void setIsActive(bool active);
	bool getIsActive() const { return _isActive; }
	
	void setTriggerBlock(ButtonBlock);
	
private:
	
	void customDraw();
	void updateHitbox();
	
	float _radius;
	float _triggerScale;
	bool _isActive;
	bool _triggered;
	
	float _triggerRadiusSquared;
	ButtonBlock _block;
};
