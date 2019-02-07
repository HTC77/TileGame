#ifndef __HUDLayer__
#define __HUDLayer__

#pragma once

#include "cocos2d.h"
USING_NS_CC;

class HUDLayer : public CCLayer
{
public:
	HUDLayer();
	~HUDLayer();
	static HUDLayer* create();
	bool init();
	void numCollectedChanged(int numCollected);

private:
	CCLabelTTF *_label;
};

#endif

