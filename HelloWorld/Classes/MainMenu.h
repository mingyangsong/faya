
#ifndef __MAINMENU_SCENE_H__
#define ___MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
class MainMenu : public cocos2d::CCLayerColor
{
public:
	MainMenu();
	~MainMenu();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene(int a);
	LAYER_NODE_FUNC(MainMenu);
	
	void menuCloseCallback(CCObject* pSender);
	void menuStartCallback(CCObject* pSender);
	void menuTutorialCallback(CCObject* pSender);
	void menuShopCallback(CCObject* pSender);
	void menuSettingsCallback(CCObject* pSender);
	void registerWithTouchDispatcher();

protected:
	cocos2d::CCMutableArray<cocos2d::CCSprite*> *_projectiles;
	
	int _projectilesDestroyed;

private:
	bool clicked;
};

#endif  // __MAINMENU_H__