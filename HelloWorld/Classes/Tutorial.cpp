#include "pch.h"
#include "Tutorial.h"
#include "MainMenu.h"


using namespace cocos2d;
using namespace Windows::Foundation;
using namespace Windows::Devices::Sensors;

int tutorialDegree;

Tutorial::~Tutorial()
{
	
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

Tutorial::Tutorial()
{
}

CCScene* Tutorial::scene(int a)
{
	tutorialDegree=a;
	CCScene * scene = NULL;
	do 
	{		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		Tutorial *layer= Tutorial::node();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Tutorial::init()
{
	bool bRet = false; //Flag

	do 
	{
		if ( !CCLayer::init() )
		{
			break;
		}

		this->setIsTouchEnabled(true);

		
		CCSprite *tuBg=CCSprite::spriteWithFile("tutorial_bg.png");
		tuBg->setAnchorPoint(CCPointZero);
		this->addChild(tuBg,0);

		CCMenuItem *Back=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("back.png" ),CCSprite::spriteWithFile("back.png"),NULL,this, menu_selector(Tutorial::menuBackCallback));
		CCMenu *menu = CCMenu::menuWithItems(Back, NULL);

		menu->alignItemsHorizontally();
		menu->setAnchorPoint(CCPointZero);
		CCSize s=CCDirector::sharedDirector()->getWinSize();
		menu->setPosition(ccp(s.width/2, 30.0f));
		this->addChild(menu,1);

		bRet = true;
	} while (0);

	return bRet;
}


void Tutorial::menuBackCallback(CCObject* pSender){
	CCScene *pScene = MainMenu::scene(tutorialDegree);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void Tutorial::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
	CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0);
}