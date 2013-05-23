
#ifndef __Shop_SCENE_H__
#define ___Shop_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class Shop : public cocos2d::CCLayerColor
{
public:
	Shop();
	~Shop();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene(int a);
	LAYER_NODE_FUNC(Shop);

	void menuItem1Callback(CCObject* pSender);
	void menuItem5Callback(CCObject* pSender);
	void menuBackCallback(CCObject* pSender);

	void registerWithTouchDispatcher();

protected:

private:

};

#endif  // __Shop_H__