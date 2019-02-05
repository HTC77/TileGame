#include "HelloWorldScene.h"


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

	firstLoad = true;
	_tileMap = CCTMXTiledMap::create("TileMap.tmx");
	_tileMap->retain();

	_background = _tileMap->layerNamed("Background");
	_background->retain();
	this->addChild(_tileMap);

	///////////////
	CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");

	if (objectGroup == NULL){
		CCLog("tile map has no objects object layer");
		return false;
	}

	CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");

	int x = ((CCString)*spawnPoint->valueForKey("x")).intValue();
	int y = ((CCString)*spawnPoint->valueForKey("y")).intValue();

	_player = new CCSprite();
	_player->initWithFile("Player.png");
	_player->setPosition(ccp(x, y));

	this->addChild(_player);
	this->setViewPointCenter(_player->getPosition());

	
	this->setTouchEnabled(true);
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::setViewPointCenter(CCPoint position)
{
	 CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
    
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	if (firstLoad){
		this->runAction(CCMoveTo::create(1, viewPoint));
		this->runAction(CCSequence::create(CCScaleTo::create(0.5, 0.5), CCScaleTo::create(0.5, 1), NULL));
		firstLoad = false;
	}
	else{
		this->setPosition(viewPoint);
	}
}

#pragma mark - handle touches

void HelloWorld::registerWithTouchDispatcher() {
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
	return true;
}

void HelloWorld::setPlayerPosition(CCPoint position) {
	_player->setPosition(position);
}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
	CCPoint touchLocation = touch->getLocationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	touchLocation = this->convertToNodeSpace(touchLocation);

	CCPoint playerPos = _player->getPosition();
	CCPoint diff = ccpSub(touchLocation, playerPos);

	if (abs(diff.x) > abs(diff.y)) {
		if (diff.x > 0) {
			playerPos.x += _tileMap->getTileSize().width;
		}
		else {
			playerPos.x -= _tileMap->getTileSize().width;
		}
	}
	else {
		if (diff.y > 0) {
			playerPos.y += _tileMap->getTileSize().height;
		}
		else {
			playerPos.y -= _tileMap->getTileSize().height;
		}
	}

	// safety check on the bounds of the map
	if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
		playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
		playerPos.y >= 0 &&
		playerPos.x >= 0)
	{
		this->setPlayerPosition(playerPos);
	}

	this->setViewPointCenter(_player->getPosition());
}
