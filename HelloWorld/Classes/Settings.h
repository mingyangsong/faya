
#ifndef __SETTINGS_SCENE_H__
#define ___SETTINGS_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class Settings : public cocos2d::CCLayerColor
{
public:
	Settings();
	~Settings();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	LAYER_NODE_FUNC(Settings);

	void menuOKCallback(CCObject* pSender);
	void menuBackCallback(CCObject* pSender);

	void Easy(CCObject* pSender);
	void Normal(CCObject* pSender);
	void Hard(CCObject* pSender);

	void registerWithTouchDispatcher();

protected:

private:
	int Degree;
};

#endif  // __SETTINGS_H__