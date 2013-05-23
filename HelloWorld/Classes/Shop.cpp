#include "pch.h"
#include "Shop.h"
#include "MainMenu.h"


using namespace cocos2d;
using namespace Windows::Foundation;
using namespace Windows::Devices::Sensors;

int ShopDegree;
CCLabelTTF *item1;
CCLabelTTF *item5;
char *item1_str;
char *item5_str;
int i1;
int i5;

Shop::~Shop()
{
	
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

Shop::Shop()
{
}

CCScene* Shop::scene(int a)
{
	ShopDegree=a;
	CCScene * scene = NULL;
	do 
	{		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		Shop *layer= Shop::node();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Shop::init()
{
	bool bRet = false; //Flag
	i1=0;
	i5=0;

	do 
	{
		if ( !CCLayer::init() )
		{
			break;
		}

		this->setIsTouchEnabled(true);

		CCSprite *shopBg=CCSprite::spriteWithFile("shop.png");
		shopBg->setAnchorPoint(CCPointZero);
		this->addChild(shopBg,0);

		CCMenuItem *Back=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("back.png" ),CCSprite::spriteWithFile("back.png"),NULL,this, menu_selector(Shop::menuBackCallback));	
		CCMenu *menu = CCMenu::menuWithItems(Back, NULL);

		menu->alignItemsHorizontally();
		menu->setAnchorPoint(CCPointZero);

		CCSize s=CCDirector::sharedDirector()->getWinSize();

		item1_str=new char[10];
		item5_str=new char[10];
		sprintf(item1_str,"%d",i1);
		sprintf(item5_str,"%d",i5);

		CCMenuItem *Item1=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("item1.png" ),CCSprite::spriteWithFile("item1_s.png"),NULL,this, menu_selector(Shop::menuItem1Callback));	
		CCMenuItem *Item2=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("item5.png" ),CCSprite::spriteWithFile("item5_s.png"),NULL,this, menu_selector(Shop::menuItem5Callback));	
		item1 = CCLabelTTF::labelWithString(item1_str, "Comic Sans MS", 12);
		item1->setPosition(ccp(s.width*2 / 3, s.height/2 +30.0f));
		item1->setColor(ccc3(255,255,255));
		item5 = CCLabelTTF::labelWithString(item5_str, "Comic Sans MS", 12);
		item5->setPosition(ccp(s.width*2 / 3, s.height/2 - 120.0f));
		item5->setColor(ccc3(255,255,255));
		item1->setString(item1_str);
		item5->setString(item5_str);
		this->addChild(item1, 0);
		this->addChild(item5, 0);

		CCMenu *items=CCMenu::menuWithItems(Item1,Item2,NULL);
		items->alignItemsVertically();
		items->setPosition(ccp(s.width/3, 230.0f));
		menu->setPosition(ccp(s.width/2, 30.0f));
		this->addChild(items,1);
		this->addChild(menu,1);

		bRet = true;
	} while (0);

	return bRet;
}

void Shop::menuBackCallback(CCObject* pSender){
	CCScene *pScene = MainMenu::scene(ShopDegree);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void Shop::menuItem1Callback(CCObject* pSender){
	i1++;
	sprintf(item1_str,"%d",i1);
	item1->setString(item1_str);
}

void Shop::menuItem5Callback(CCObject* pSender){
	i5++;
	sprintf(item5_str,"%d",i5);
	item5->setString(item5_str);
}
void Shop::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
	CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0);
}