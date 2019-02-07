#include "HudLayer.h"

HUDLayer::HUDLayer()
{

}
HUDLayer::~HUDLayer()
{

}

HUDLayer* HUDLayer::create()
{
	HUDLayer* obj = new HUDLayer();
	if (obj && obj->init())
	{
		obj->autorelease();
		return obj;
	}

	CC_SAFE_DELETE(obj);
	return NULL;
}

bool HUDLayer::init()
{
	if (CCLayer::init()) {
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		_label = new CCLabelTTF();
		_label->initWithString("0", "Verdana-Bold", 18.0);
		_label->setColor(ccc3(0, 0, 0));

		int margin = 10;
		_label->setPosition(ccp(winSize.width - (_label->getContentSize().width / 2) - margin, _label->getContentSize().height / 2 + margin));
		this->addChild(_label);
	}
	return true;
}

void HUDLayer::numCollectedChanged(int numCollected)
{
	CCString *labelCollected = new CCString();
	labelCollected->initWithFormat("%d", numCollected);
	_label->setString(labelCollected->getCString());
}