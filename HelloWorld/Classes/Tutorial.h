
#ifndef __TUTORIAL_SCENE_H__
#define ___TUTORIAL_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class Tutorial : public cocos2d::CCLayerColor
{
public:
	Tutorial();
	~Tutorial();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene(int a);
	LAYER_NODE_FUNC(Tutorial);

	void menuBackCallback(CCObject* pSender);

	void registerWithTouchDispatcher();

protected:

private:

};

#endif  // __TUTORIAL_H__