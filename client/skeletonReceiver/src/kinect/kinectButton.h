#pragma once

#include "ofNode.h"

typedef void (^ButtonBlock)(bool isOn, float vel);

class kinectButton : public ofNode {
	
public:
	
	kinectButton();
	
	void update(const std::vector< std::pair<ofPoint, float> > &points);
	bool hitTest(const ofVec3f& v, float distSquared) const;
	
    bool isTriggered();
    
	void setRadius(float width);
	float getRadius() const { return _radius; }
	
	void setTriggerScale(float triggerScale);
	float getTriggerScale() const { return _triggerScale; }
	
	void setApproachScale(float approachScale);
	float getApproachScale() const { return _approachScale; }
	
	void setIsTriggered(bool isTriggered);
	bool getIsTriggered() const { return _isTriggered; }
	
	void setIsApproached(bool isApproached);
	bool getIsApproached() const { return _isApproached; }
	
	void setTriggerBlock(ButtonBlock);
	void setApproachBlock(ButtonBlock);
	
private:
	
	void customDraw();
	void updateHitbox();
	
	float _radius;
	float _radiusSquared;
	
	float _triggerScale;
	float _approachScale;
	
	bool _isTriggered;
	bool _isApproached;
	
	float _triggerRadiusSquared;
	float _approachRadiusSquared;
	
	ButtonBlock _triggerBlock;
	ButtonBlock _approachBlock;
};
