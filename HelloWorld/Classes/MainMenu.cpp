#include "pch.h"
#include "MainMenu.h"
#include "HelloWorldScene.h"
#include "Settings.h"
#include "Tutorial.h"
#include "Shop.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace Windows::Foundation;
using namespace Windows::Devices::Sensors;

b2World *menu_world;
int mainDegree;
//CCLayer *menuLayer;


MainMenu::~MainMenu()
{
	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

MainMenu::MainMenu()
	:_projectiles(NULL)
	,_projectilesDestroyed(0)
{
}

CCScene* MainMenu::scene(int a)
{
	mainDegree=a;
	CCScene * scene = NULL;
	do 
	{		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		MainMenu *layer= MainMenu::node();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
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


		////Menu	
		CCMenuItemFont *Start = CCMenuItemFont::itemFromString("Start",this,menu_selector(MainMenu::menuStartCallback));
		Start->setFontSizeObj(12);
		Start->setColor(ccc3(250,250,250));
		Start->setFontName("Comic Sans MS");

		CCMenuItemFont *Exit = CCMenuItemFont::itemFromString("Exit",this,menu_selector(MainMenu::menuCloseCallback));
		Exit->setFontSizeObj(12);
		Exit->setColor(ccc3(250,250,250));
		Exit->setFontName("Comic Sans MS");

		CCMenuItemFont *Shop = CCMenuItemFont::itemFromString("Shop",this,menu_selector(MainMenu::menuShopCallback));
		Shop->setFontSizeObj(12);
		Shop->setColor(ccc3(250,250,250));
		Shop->setFontName("Comic Sans MS");

		CCMenuItemFont *Tutorial = CCMenuItemFont::itemFromString("Tutorial",this,menu_selector(MainMenu::menuTutorialCallback));
		Tutorial->setFontSizeObj(12);
		Tutorial->setColor(ccc3(250,250,250));
		Tutorial->setFontName("Comic Sans MS");

		CCMenuItemFont *Settings= CCMenuItemFont::itemFromString("Settings",this,menu_selector(MainMenu::menuSettingsCallback));
		Settings->setFontSizeObj(12);
		Settings->setColor(ccc3(250,250,250));
		Settings->setFontName("Comic Sans MS");

		CCMenu *menu = CCMenu::menuWithItems(Start, Tutorial, Shop, Settings, Exit, NULL);
		menu->alignItemsVertically();

		menu->setAnchorPoint(CCPointZero);
		menu->setPosition(ccp(0.0f,20.0f));
		
		CCNode *nodeCombine=CCNode::node();
		CCSprite *bg=CCSprite::spriteWithFile("menu_board.png");
		CCSprite *top=CCSprite::spriteWithFile("menu_bg.png");
		top->setAnchorPoint(CCPointZero);
		CCSize s=CCDirector::sharedDirector()->getWinSize();
		top->setPosition(CCPointZero);
		top->setOpacity(250.0f);
		CCFadeTo *fade=CCFadeTo::actionWithDuration(0.8f,0.0f);

		nodeCombine->addChild(bg,1);
		nodeCombine->addChild(menu,2);
		nodeCombine->setPosition(ccp(160.0f,150.0f));

		CCPoint pos =ccp(160.0f, 200.0f );
		
		CCMoveTo *moveTo=CCMoveTo::actionWithDuration(0.8f,pos);
		
		CCFiniteTimeAction *seq=CCSequence::actions(moveTo,NULL);
		nodeCombine->runAction(seq);//CC::actionWithAction(CCMoveBy::actionWithDuration(1.5, CCPointMake(5.0f,5.0f)), 0.7f));
		
		top->runAction(fade);
		
		this->addChild(top,2);
		this->addChild(nodeCombine,1);

		CCSprite *mainBg = CCSprite::spriteWithFile("menu_bg.png");
		mainBg->setAnchorPoint(CCPointZero);
		this->addChild(mainBg, 0);

		bRet = true;
	} while (0);

	return bRet;
}

void MainMenu::menuStartCallback(CCObject* pSender){
	CCScene *pScene = HelloWorld::scene(mainDegree);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void MainMenu::menuCloseCallback(CCObject* pSender){
	CCDirector::sharedDirector()->end();
}

void MainMenu::menuTutorialCallback(CCObject* pSender){
	CCScene *pScene = Tutorial::scene(mainDegree);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void MainMenu::menuShopCallback(CCObject* pSender){
	CCScene *pScene = Shop::scene(mainDegree);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void MainMenu::menuSettingsCallback(CCObject* pSender){
	CCScene *pScene = Settings::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void MainMenu::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
	CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0);
}