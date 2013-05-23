/*
* cocos2d-x   http://www.cocos2d-x.org
*
* Copyright (c) 2010-2011 - cocos2d-x community
* 
* Portions Copyright (c) Microsoft Open Technologies, Inc.
* All Rights Reserved
* 
* Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at 
* 
* http://www.apache.org/licenses/LICENSE-2.0 
* 
* Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an 
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and limitations under the License.
*/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCGeometry.h"
#include "SimpleAudioEngine.h"
#include "MyContactListener.h"
#include "tool.h"
#include <wrl\client.h>
#include <memory>
#include <vector>

///using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayerColor
{
public:
	HelloWorld();
	~HelloWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene(int a);
	// a selector callback
	//virtual void menuCloseCallback(CCObject* pSender);

	// implement the "static node()" method manually
	LAYER_NODE_FUNC(HelloWorld);

	void spriteMoveFinished(CCNode* sender);

	void gameLogic(cocos2d::ccTime dt);

	void updateBox2D(cocos2d::ccTime dt);

	void updateGame(cocos2d::ccTime dt);
	
	void resetGame(cocos2d::ccTime dt);
	
	void resetBullet(cocos2d::ccTime dt);

	void tick(cocos2d::ccTime dt);
	void removeTheWall(cocos2d::ccTime dt);
	//void updateGravity(cocos2d::ccTime dt);
	void removeBomb(cocos2d::ccTime dt);
	void registerWithTouchDispatcher();
	void menuPauseCallback(CCObject* pSender);
	void menuRestartCallback(CCObject* pSender);
	void menuBackCallback(CCObject* pSender);
	void menuMainCallback(CCObject* pSender);

protected:
	cocos2d::CCMutableArray<cocos2d::CCSprite*> *_targets;
	cocos2d::CCMutableArray<cocos2d::CCSprite*> *_projectiles;
	
	vector<b2Body*> _b2Boxes;
	vector<cocos2d::CCSprite*> _CCBoxes;
	int _projectilesDestroyed;
	
private:
	

	void createBullet(int count);
	int millisecondNow();
	void createTarget(const char *imageName, cocos2d::CCPoint position, float rotation, bool isCircle, bool isStatic, bool isEnemy);
	void createTargets();
	void createStem(int type);
	void createCell(int type);
	void enableStem(int type);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
	void playitself(int a);
	void cleanitself(CCNode* node);
	void Victory();
	//Windows::Devices::Sensors::Accelerometer^ m_accelerometer;
	bool start;
	bool pause;
	bool degreeFlag;
	bool vicFlag;
};





#endif  // __HELLOWORLD_SCENE_H__