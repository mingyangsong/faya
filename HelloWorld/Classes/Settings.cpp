#include "pch.h"
#include "Settings.h"
#include "MainMenu.h"
#include "CCRadioMenu.h"


using namespace cocos2d;
using namespace Windows::Foundation;
using namespace Windows::Devices::Sensors;

CCMenuItemSprite *r1;
CCMenuItemSprite *r2;
CCMenuItemSprite *r3;
CCRadioMenu *radioMenu;

Settings::~Settings()
{
	
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

Settings::Settings()
{
}

CCScene* Settings::scene()
{
	CCScene * scene = NULL;
	do 
	{		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		Settings *layer= Settings::node();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Settings::init()
{
	bool bRet = false; //Flag

	do 
	{
		if ( !CCLayer::init() )
		{
			break;
		}

		this->setIsTouchEnabled(true);

		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		r1=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("brick.png" ),CCSprite::spriteWithFile("enemy0.png"),NULL,this, menu_selector(Settings::Easy));
		r2=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("brick.png" ),CCSprite::spriteWithFile("enemy1.png"),NULL,this, menu_selector(Settings::Normal));
		r3=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("brick.png" ),CCSprite::spriteWithFile("enemy2.png"),NULL,this, menu_selector(Settings::Hard));
		radioMenu=CCRadioMenu::radioMenuWithItems(r1, r2, r3, NULL);
		radioMenu->alignItemsVertically();
		radioMenu->setSelectedRadioItem(r1);
		radioMenu->setPosition(ccp(screenSize.width/2+20.0f,screenSize.height/2));
		this->addChild(radioMenu,1);

		CCLabelTTF *t1=CCLabelTTF::labelWithString("Easy","Marker Felt",20);
		t1->setColor(ccc3(200,100,100));
		CCLabelTTF *t2=CCLabelTTF::labelWithString("Normal","Marker Felt",20);
		t2->setColor(ccc3(200,100,100));
		CCLabelTTF *t3=CCLabelTTF::labelWithString("Hard","Marker Felt",20);
		t3->setColor(ccc3(200,100,100));

		t3->setPosition(ccp(screenSize.width/3,screenSize.height/2-30.0f));
		t2->setPosition(ccp(screenSize.width/3,screenSize.height/2));
		t1->setPosition(ccp(screenSize.width/3,screenSize.height/2+30.0f));
		this->addChild(t1,1);
		this->addChild(t2,1);
		this->addChild(t3,1);

		CCMenuItemSprite *OK=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("OK.png" ),CCSprite::spriteWithFile("OK.png"),NULL,this,menu_selector(Settings::menuOKCallback));
		CCMenuItemSprite *Back=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("back.png" ),CCSprite::spriteWithFile("back.png"),NULL,this,menu_selector(Settings::menuBackCallback));

		CCMenu *menu = CCMenu::menuWithItems(OK, Back, NULL);

		CCSprite *setBg=CCSprite::spriteWithFile("settings.png");
		setBg->setAnchorPoint(CCPointZero);
		this->addChild(setBg,0);
		
		menu->alignItemsHorizontally();
		menu->setPosition(ccp(screenSize.width/2, 30.0f));
		this->addChild(menu,1);

		bRet = true;
	} while (0);

	return bRet;
}

void Settings::menuOKCallback(CCObject* pSender){
	CCScene *pScene = MainMenu::scene(Degree);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void Settings::menuBackCallback(CCObject* pSender){
	Degree=-1;
	CCScene *pScene = MainMenu::scene(Degree);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void Settings::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
	CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0);
}

void Settings::Easy(CCObject* pSender){
	Degree=0;
}

void Settings::Normal(CCObject* pSender){
	Degree=1;
}

void Settings::Hard(CCObject* pSender){
	Degree=2;
}
