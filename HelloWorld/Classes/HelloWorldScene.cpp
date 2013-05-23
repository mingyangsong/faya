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

#include "pch.h"

#include "HelloWorldScene.h"
#include "CCCommon.h"
#include "BasicLoader.h"
#include <CCParticleExamples.h>
#include <vector>
#include <queue>
#include <time.h>
#include <map>
#include <set>
#include "cocos2d.h"
#include "CCObject.h"
#include <sys/timeb.h>
#include "MainMenu.h"


using namespace cocos2d;
using namespace Windows::Foundation;
using namespace Windows::Devices::Sensors;

b2World *world;

b2Body *groundBody;
vector<b2Body*> *targets;
vector<b2Body*> *enemies;
b2Body * lastBlock;
queue<b2Body*> *blocks;
vector<b2Body*> *attackunit;
MyContactListener *contactListener;
string blockPath;
string cellPath;
b2Body* levelGround;
b2Body* testBlock;
vector<b2Body*> *killed;
set<b2Body*> *freezeblocks;
set<b2Body*> *fireblocks;
set<b2Body*>* freezedByMe;
set<b2Body*>* isKilled;
string* stemImages;
string* cellImages;
stemData* stemDataList;
cellData* cellDataList;
touchArea* StemTouchAreaList;
CCLabelTTF* mLabel;
CCLabelTTF* eLabel;
CCLabelTTF* BLabel;
long lastTimeInMs;
int* stemAvalibleAfter;
int currentCount;
int level;
CCDictionary<string,CCAction*>* actionList;
char* moneyLabel;
int money;
int energy;
int maxNumOfBlocks;
int currentCells;
int maxEnergy;
float shouldgo;
float currentheight;
float blockHeight;
float blockWidth; 
float buttonWidth;
float buttonHeight;
int* totalCells;
int maxCells;
int cellRefreshRate;
int* blood;

CCMenu *inMenu;
CCMenu *vicMenu;
CCLayer *pauseLayer;
CCLayer *degreeLayer;
CCLayer *vicLayer;
CCLabelTTF *degreeLabel;
CCLabelTTF *pauseLabel;
CCLabelTTF *vicLabel;
int currentBotton;
int helloDegree;
float moveBy;
#define PIX_TO_MET 0.03125f
#define MET_TO_PIX 32.0f
#define PTM_RATIO MET_TO_PIX
#define MED_FULL 210.0f
#define MED_HALF 105.0f
#define SMALL_FULL 100.0f
#define SMALL_HALF 50.0f
#define LARGE_GAP 24.0f
#define SMALL_GAP 12.0f
#define FLOOR_HEIGHT 64.0f
#define attackStart 10.0f
#define initalCells 10
#define typeOfStems 3
#define typeOfCells 4
#define maxLevel 55
#define maxCellRefreshRate 3 
#define levelDelta (level/5)*(level+10)/10 //level num of cells
#define intialCellRefreshRate 50  //fresh rate
#define cellRateDelta (level%5==0)*(((intialCellRefreshRate-maxCellRefreshRate)/(maxLevel/5))+helloDegree)
#define initialBlood 30
CCSprite* stemButtonList[typeOfStems];
CCSprite* stemButtonOffList[typeOfStems];
CCSprite* healthBar,*energyBar;
double* cellDistirbution[typeOfCells];
class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		int a = fixture->GetFilterData().categoryBits;
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};


HelloWorld::~HelloWorld()
{
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}

	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}
	if (!_CCBoxes.empty())
	{
		_CCBoxes.clear();
	}
	if (!_b2Boxes.empty())
	{
		_b2Boxes.clear();
	}
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

HelloWorld::HelloWorld()
	:_targets(NULL)
	,_projectiles(NULL)
	,_projectilesDestroyed(0)
{
}

CCScene* HelloWorld::scene(int a)
{
	helloDegree=a;
	if(helloDegree==-1)helloDegree=1;
	CCScene * scene = NULL;
	do 
	{		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelloWorld *layer = HelloWorld::node();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	
	bool bRet = false;
	pause=false;
	vicFlag=false;
	moveBy=0;

	//maxAttack = 2;
	do 
	{
		if ( !CCLayer::init() )
		{
			break;
		}
		maxCells = initalCells;
		
		for(int i=0;i<typeOfCells;i++){
			cellDistirbution[i] = new double[maxLevel];
		}
		cellRefreshRate = intialCellRefreshRate-helloDegree*2;

		for(int i=0;i<5;i++){
			cellDistirbution[0][i] = 1;
			cellDistirbution[1][i] = 0;
			cellDistirbution[2][i] = 0;
			cellDistirbution[3][i] = 0;
		}

		for(int i=5;i<10;i++){
			cellDistirbution[0][i] = 0.5;
			cellDistirbution[1][i] = 0.5;
			cellDistirbution[2][i] = 0;
			cellDistirbution[3][i] = 0;
		}

		for(int i=10;i<20;i++){
			cellDistirbution[0][i] = 0.2;
			cellDistirbution[1][i] = 0.5;
			cellDistirbution[2][i] = 0.3;
			cellDistirbution[3][i] = 0;
		}

		for(int i=20;i<maxLevel;i++){
			cellDistirbution[0][i] = 0.1;
			cellDistirbution[1][i] = 0.3;
			cellDistirbution[2][i] = 0.3;
			cellDistirbution[3][i] = 0.3;
		}

		stemAvalibleAfter = new int[typeOfStems];
		stemAvalibleAfter[0]=0;
		stemAvalibleAfter[1]=10;
		stemAvalibleAfter[2]=20;

		//******************//
		maxEnergy = 10;
		blockPath = "shape0.png";
		cellPath  = "enemy1.png";
		moneyLabel = new char[10];


		stemImages = new string[typeOfStems];
		cellImages = new string[typeOfCells];
		stemDataList = new stemData[typeOfStems];
		cellDataList = new cellData[typeOfCells];
	
		//******************//
		stemDataList[0].life = 2;
		stemDataList[0].maxSubType = 5;
		stemDataList[0].currentSubType = rand()%stemDataList[0].maxSubType;
		stemDataList[0].imagePath = new string[stemDataList[0].maxSubType];
		stemDataList[0].imagePath[0] = "stem1.png";
		stemDataList[0].imagePath[1] = "stem2.png";
		stemDataList[0].imagePath[2] = "stem3.png";
		stemDataList[0].imagePath[3] = "stem4.png";
		stemDataList[0].imagePath[4] = "stem5.png";
		stemDataList[0].buttonImage = new string[2]; ;
		stemDataList[0].buttonImage[0] = "green_miao1.png";
		stemDataList[0].buttonImage[1] = "greenmiao1_btdown.png";
		//stemDataList[0].imagePath[2] = "shape0_2.png"

		stemDataList[0].enabled = true;
		stemDataList[0].energyCost = 2;
		stemDataList[0].affordable = false;

		//******************//
		stemDataList[1].life = 2;
		stemDataList[1].maxSubType = 5;
		stemDataList[1].currentSubType = rand()%stemDataList[1].maxSubType;
		stemDataList[1].imagePath = new string[stemDataList[1].maxSubType];
		stemDataList[1].imagePath[0] = "bluestem1.png";
		stemDataList[1].imagePath[1] = "bluestem2.png";
		stemDataList[1].imagePath[2] = "bluestem3.png";
		stemDataList[1].imagePath[3] = "bluestem4.png";
		stemDataList[1].imagePath[4] = "bluestem5.png";
		stemDataList[1].enabled = false;
		stemDataList[1].energyCost = 5;
		stemDataList[1].affordable = false;
		stemDataList[1].buttonImage = new string[2]; ;
		stemDataList[1].buttonImage[0] = "blue_miao_up.png";
		stemDataList[1].buttonImage[1] = "blue_miao_btdown.png";

		//******************//
		stemDataList[2].life = 2;
		stemDataList[2].maxSubType = 3;
		stemDataList[2].currentSubType = rand()%stemDataList[2].maxSubType;
		stemDataList[2].imagePath = new string[stemDataList[2].maxSubType];
		stemDataList[2].imagePath[0] = "redstem1.png";
		stemDataList[2].imagePath[1] = "redstem2.png";
		stemDataList[2].imagePath[2] = "redstem3.png";
		stemDataList[2].enabled = false;
		stemDataList[2].energyCost = 10;
		stemDataList[2].affordable = false;
		stemDataList[2].buttonImage = new string[2]; ;
		stemDataList[2].buttonImage[0] = "redmiao_up.png";
		stemDataList[2].buttonImage[1] = "red_miao_btdown.png";

		//******************//
		stemImages[0] = "shape0.png";
		cellImages[0] = "enemy0.png";
	
		//******************//
		cellDataList[0].numOfSubType = 1;
		cellDataList[0].imagePath = new string[cellDataList[0].numOfSubType];
		cellDataList[0].imagePath[0] = "enemy0.png";
		cellDataList[0].life = 1;
		cellDataList[0].atk = 1;
		cellDataList[0].moneyGet = 1;
		cellDataList[0].energyGet = 1;

		//******************//
		cellDataList[1].numOfSubType = 1;
		cellDataList[1].imagePath = new string[cellDataList[1].numOfSubType];
		cellDataList[1].imagePath[0] = "enemy_2.png";
		cellDataList[1].life = 1;
		cellDataList[1].atk = 2;
		cellDataList[1].moneyGet = 2;
		cellDataList[1].energyGet = 2;

				//******************//
		cellDataList[2].numOfSubType = 1;
		cellDataList[2].imagePath = new string[cellDataList[2].numOfSubType];
		cellDataList[2].imagePath[0] = "enemy_3.png";
		cellDataList[2].life = 2;
		cellDataList[2].atk = 1;
		cellDataList[2].moneyGet = 5;
		cellDataList[2].energyGet = 3;

				//******************//
		cellDataList[3].numOfSubType = 1;
		cellDataList[3].imagePath = new string[cellDataList[3].numOfSubType];
		cellDataList[3].imagePath[0] = "enemy_4.png";
		cellDataList[3].life = 2;
		cellDataList[3].atk = 2;
		cellDataList[3].moneyGet = 5;
		cellDataList[3].energyGet = 3;





		CCImage* tmpimg = new CCImage;
		tmpimg->initWithImageFile(stemDataList[0].imagePath[0].c_str());
		blockHeight = tmpimg->getHeight();
		blockWidth = tmpimg->getWidth();
		delete tmpimg; 
		tmpimg = new CCImage;
		tmpimg->initWithImageFile(stemDataList[0].buttonImage[0].c_str());
		buttonWidth = tmpimg->getWidth();
		buttonHeight = tmpimg->getHeight();
		delete tmpimg; 




		StemTouchAreaList = new touchArea[typeOfStems];
		CCPoint leftbottom = ccp(5,5);
		CCPoint righttop = ccp(leftbottom.x+buttonWidth,leftbottom.y+buttonHeight);
		StemTouchAreaList[0].leftbot = leftbottom;
		StemTouchAreaList[0].righttop = righttop;
		
		leftbottom = ccp(CCDirector::sharedDirector()->getWinSize().width-buttonWidth-5,5);
		righttop = ccp(leftbottom.x+buttonWidth,leftbottom.y+buttonHeight);
		StemTouchAreaList[1].leftbot = leftbottom;
		StemTouchAreaList[1].righttop = righttop;

		leftbottom = ccp(StemTouchAreaList[1].leftbot.x,StemTouchAreaList[1].leftbot.y+buttonHeight+5);
		righttop = ccp(leftbottom.x+buttonWidth,leftbottom.y+buttonHeight);
		StemTouchAreaList[2].leftbot = leftbottom;
		StemTouchAreaList[2].righttop = righttop;


		enableStem(0);
		enableStem(1);
		enableStem(2);

		
		//create action dictionary
		actionList = new CCDictionary<string,CCAction*>();
		CCFadeTo* out = CCFadeTo::actionWithDuration(0.3f,150);
		CCFadeTo* in = CCFadeTo::actionWithDuration(0.3f,255);
		actionList->setObject(out,"gradout");
		actionList->setObject(in,"gradin");
		CCSequence* inandout = CCSequence::actionOneTwo(out,in);
		actionList->setObject(inandout,"gradinout");
		CCMoveBy* move1 = CCMoveBy::actionWithDuration(1.0f,ccp(0,-150));
		CCMoveBy* move2 = CCMoveBy::actionWithDuration(0.0f,ccp(0,150));
		CCSequence* moveout = CCSequence::actionOneTwo(move1,move2);
		actionList->setObject(moveout,"moveout");
		actionList->setObject(move1,"move1");
		CCRotateBy* rotate0 = CCRotateBy::actionWithDuration(0.05f,+5);
		CCRotateBy* rotate1 = CCRotateBy::actionWithDuration(0.05f,-5);
		CCSequence* rotation1 = CCSequence::actionOneTwo(rotate0,rotate1);
		CCRepeat* rotations = CCRepeat::actionWithAction(rotation1,2);
		CCSpawn* inRotations = CCSpawn::actionOneTwo(rotations,in);
		actionList->setObject(rotations,"vibe");
		actionList->setObject(inRotations,"invibe");
		//actionList->setObject



		this->setIsTouchEnabled(true);
		b2Vec2 gravity;
		gravity.Set(0.0f, 0.0f);
		
		float h = CCDirector::sharedDirector()->getWinSize().height;
		maxNumOfBlocks = CCDirector::sharedDirector()->getWinSize().height*3/5/blockHeight+1.5;
		world = new b2World(gravity);
		world->SetContinuousPhysics(true);
		
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		currentheight = -blockHeight;
			;;
		CCSprite *sprite = CCSprite::spriteWithFile("bg_1017.png" );
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite, 0);
		
		sprintf(moneyLabel,"$: %d",money);

		
		mLabel = CCLabelTTF::labelWithString(moneyLabel, "Comic Sans MS", 12);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		mLabel->setAnchorPoint(ccp(0,1));
		mLabel->setPosition( ccp(10, size.height - 20) );
		mLabel->setColor(ccc3(255,215,0));
		this->addChild(mLabel, 0);
		sprintf(moneyLabel,"Energy: %d",energy);
		eLabel = CCLabelTTF::labelWithString(moneyLabel, "Comic Sans MS", 12);
		eLabel->setAnchorPoint(ccp(0,1));
		eLabel->setPosition( ccp(60, size.height - 20) );
		eLabel->setColor(ccc3(173,216,230));
		this->addChild(eLabel, 0);

		// Degree Show
		char* degree=NULL;
		if(helloDegree==0){
			degree="Easy!";
		}else if(helloDegree==1){
			degree="Normal!";
		}else if(helloDegree==2){
			degree="Hard!";
		}
		if(degree!=NULL){
			degreeFlag=true;
			degreeLabel=CCLabelTTF::labelWithString(degree,"Comic Sans MS",24);
			degreeLabel->setPosition(ccp(100.0f,75.0f));
			degreeLayer=CCLayerColor::layerWithColorWidthHeight(ccc4f(255,255,255,100),200.0f,150.0f);
			degreeLayer->setPosition((screenSize.width-200.0f)/2,(screenSize.height-120.0f)/2);
			
			degreeLayer->addChild(degreeLabel,0);
			this->addChild(degreeLayer,10);
		}

		//inMenu
		CCMenuItem *Pause=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("pause.png" ),CCSprite::spriteWithFile("pause.png"),this,menu_selector(HelloWorld::menuPauseCallback));
		CCMenuItem *Restart=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("restart.png" ),CCSprite::spriteWithFile("restart.png"),this,menu_selector(HelloWorld::menuRestartCallback));
		CCMenuItem *Back=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("back.png" ),CCSprite::spriteWithFile("back.png"),this,menu_selector(HelloWorld::menuBackCallback));

		inMenu = CCMenu::menuWithItems(Pause, Restart, Back, NULL);
		inMenu->alignItemsHorizontally();
		inMenu->setPosition(ccp(240.0f,500.0f));
		this->addChild(inMenu,10);


		b2BodyDef groundBodyDef;
		groundBodyDef.type = b2_staticBody;
		groundBodyDef.position.Set(0,0);
		groundBody = world->CreateBody(&groundBodyDef);
		b2EdgeShape groundBox;
		b2FixtureDef boxShapeDef;
		boxShapeDef.shape = &groundBox;
		boxShapeDef.filter.groupIndex = 7;
		double gap = 20;
		//bottom
		groundBox.Set(b2Vec2(0,-gap/PTM_RATIO), b2Vec2(screenSize.width/PTM_RATIO,-gap/PTM_RATIO));
		groundBody->CreateFixture(&boxShapeDef);
		
		//levelGroundnext
		//top
		groundBox.Set(b2Vec2(0,(screenSize.height+gap)/PTM_RATIO), b2Vec2(screenSize.width/PTM_RATIO,(screenSize.height+gap)/PTM_RATIO));
		groundBody->CreateFixture(&boxShapeDef);

		//left
		groundBox.Set(b2Vec2(-gap/PTM_RATIO,screenSize.height/PTM_RATIO), b2Vec2(-gap/PTM_RATIO,0));
		groundBody->CreateFixture(&boxShapeDef);

		//right
		groundBox.Set(b2Vec2((screenSize.width+gap)/PTM_RATIO,screenSize.height/PTM_RATIO), b2Vec2((screenSize.width+gap)/PTM_RATIO,0));
		groundBody->CreateFixture(&boxShapeDef);

		targets = new vector<b2Body*>;
		enemies = new vector<b2Body*>;
		killed = new vector<b2Body*>;
		isKilled = new set<b2Body*>;
		blocks = new queue<b2Body*>;
		freezeblocks = new set<b2Body*>;
		freezedByMe = new set<b2Body*>;
		fireblocks = new set<b2Body*>;
		currentCells = 0;
		contactListener = new MyContactListener();
		blood = new int(initialBlood);
		contactListener->count = blood;
		contactListener->celldata = cellDataList;
		world->SetContactListener(contactListener);
		sprintf(moneyLabel,"Health: %d/30",*blood);
		BLabel = CCLabelTTF::labelWithString(moneyLabel, "Comic Sans MS", 12);
		BLabel->setAnchorPoint(ccp(0,1));
		BLabel->setPosition( ccp(10, size.height - 40) );
		BLabel->setColor(ccc3(125,255, 5));


		
		


		this->addChild(BLabel, 0);

		//this->setPosition(ccp(-480,0));
		this->schedule(schedule_selector(HelloWorld::resetGame));
		this->schedule( schedule_selector(HelloWorld::tick) );
		
		

		//this->resetGame();
		//start = true;
		bRet = true;
	} while (0);

	return bRet;
}

void HelloWorld::resetGame(ccTime dt)
{

	this->unschedule(schedule_selector(HelloWorld::resetGame));


	for(int i=0;i<4;i++)
		createStem(0);
}




void HelloWorld::tick(ccTime dt)
{
	currentCount++;	
	int velocityIterations = 8;
	int positionIterations = 1;
	world->Step(dt, velocityIterations, positionIterations);
	for (b2Body* b = world->GetBodyList(); b; b=b->GetNext())
	{
		if(b->GetUserData() != NULL)
		{
			CCSprite *myActor = (CCSprite *)b->GetUserData();
			myActor->setPosition(ccp(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
	for (int i = 0; i < typeOfStems; i++){
		if(stemButtonList[i]->getIsVisible()!=stemDataList[i].enabled)
			stemButtonList[i]->setIsVisible(stemDataList[i].enabled);
	}

	if(contactListener->died){
		if(vicFlag!=true)HelloWorld::Victory();
	}

	if(shouldgo>0){
		CCPoint p = this->getPosition();
		float delta = shouldgo/8;
		p.y -=delta;

		this->setPosition(p);
		shouldgo-=delta;
		
		moveBy+=delta;

		p = mLabel->getPosition();
		p.y+=delta;
		mLabel->setPosition(p);

		p = BLabel->getPosition();
		p.y+=delta;
		BLabel->setPosition(p);

		p = eLabel->getPosition();
		p.y+=delta;
		eLabel->setPosition(p);
		




		if(vicFlag==false){
			p=inMenu->getPosition();
			p.y+=delta;
			inMenu->setPosition(p);
		}


		for(int i=0;i<typeOfStems;i++){
			p=stemButtonList[i]->getPosition();
			p.y+=delta;
			stemButtonList[i]->setPosition(p);
			stemButtonOffList[i]->setPosition(p);
		}

		b2Vec2 pp = groundBody->GetPosition();
		pp.y+=delta/PTM_RATIO;		
		groundBody->SetTransform(pp,groundBody->GetAngle());

		/*pp = testBlock->GetPosition();
		pp.y+=delta/PTM_RATIO;		
		testBlock->SetTransform(pp,testBlock->GetAngle());*/
	}
	if((currentCount%cellRefreshRate==0)&&currentCells<maxCells){
		int tlevel = level;
		int type;
		double num = 100;
		double sum = 0;
		int seed = rand()%100+1;
		if(tlevel>=maxLevel)
			tlevel = maxLevel-1;
		for(int i=0;i<typeOfCells;i++){
			double tmp = cellDistirbution[i][tlevel]*100;
			sum+=tmp;
			if (sum>=seed){
				type = i;
				break;
			}
				
		}
		createCell(type);
	}
	set<b2Body*>::iterator pos;
	
	
	
	
	for(pos = contactListener->freezed.begin(); pos != contactListener->freezed.end(); ++pos)
	{

		b2Body *body = *pos;
		body->SetAwake(false);
		freezedByMe->insert(body);

	}
	contactListener->freezed.clear();
	
	
	


	for(pos = contactListener->boomed.begin(); pos != contactListener->boomed.end(); ++pos)
	{

		b2Body *body = *pos;
		if(isKilled->find(body)!=isKilled->end())
			isKilled->erase(body);
		int type = (-(int)body->GetFixtureList()->GetFilterData().groupIndex)-1;
		money+=cellDataList[type].moneyGet;
		energy+=cellDataList[type].energyGet;
		if(energy>maxEnergy)
			energy=maxEnergy;
		if(freezedByMe->find(body)!=freezedByMe->end()){
			freezedByMe->erase(body);
		}
	}

		contactListener->boomed.clear();


	for(pos = contactListener->contacts.begin(); pos != contactListener->contacts.end(); ++pos)
	{

		b2Body *body = *pos;
		if(isKilled->find(body)!=isKilled->end()){
			int type = (-(int)body->GetFixtureList()->GetFilterData().groupIndex)-1;
			money+=cellDataList[type].moneyGet;
			energy+=cellDataList[type].energyGet;
			if(energy>maxEnergy)
				energy=maxEnergy;
			isKilled->erase(body);
		}
		if(freezedByMe->find(body)!=freezedByMe->end()){
			freezedByMe->erase(body);
		}






		CCNode *contactNode = (CCNode *) body->GetUserData();
		CCPoint position = contactNode->getPosition();
		this->removeChild(contactNode,true);
		world->DestroyBody(body);
		currentCells--;



		/*CCParticleSun* explosion = new CCParticleSun();
		explosion->initWithTotalParticles(200);
		//explosion->setTotalParticles(200);
		explosion->setIsAutoRemoveOnFinish(true);
		explosion->setStartSize(10.0f);
		explosion->setSpeed(70.0f);
		explosion->setAnchorPoint(ccp(0.5f,0.5f));
		explosion->setPosition(position);
		explosion->setDuration(1.0f);
		CCTexture2D *tex = new CCTexture2D();
		CCImage *img = new CCImage();
		img->initWithImageFile("fire.png");
		tex->initWithImage(img);
		explosion->setTexture(tex);
		this->addChild(explosion, 11);
		explosion->release();*/

	}
	contactListener->contacts.clear();
	set<b2Body*>::iterator pp;
	
	for(pp = isKilled->begin(); pp != isKilled->end(); ++pp)
	{
		b2Body *body = *pp;
		CCNode *contactNode = NULL;
		CCPoint position;
		try{
			int type = (-(int)body->GetFixtureList()->GetFilterData().groupIndex)-1;
			money+=cellDataList[type].moneyGet;
			energy+=cellDataList[type].energyGet;
			if(energy>maxEnergy)
				energy=maxEnergy;
			contactNode = (CCNode *) body->GetUserData();
			position = contactNode->getPosition();// fixme
			this->removeChild(contactNode,true);
			world->DestroyBody(body);
			currentCells--;
		}catch(exception e){
		}



		CCParticleFlower* explosion = new CCParticleFlower();
		explosion->initWithTotalParticles(40);
		//explosion->setTotalParticles(200);
		explosion->setIsAutoRemoveOnFinish(true);
		explosion->setStartSize(10.0f);
		explosion->setSpeed(70.0f);
		explosion->setAnchorPoint(ccp(0.5f,0.5f));
		explosion->setPosition(position);
		explosion->setDuration(1.0f);
		CCTexture2D *tex = new CCTexture2D();
		CCImage *img = new CCImage();
		img->initWithImageFile("fire.png");
		tex->initWithImage(img);
		explosion->setTexture(tex);
		this->addChild(explosion, 11);
		explosion->release();
		
	}
	isKilled->clear();


	//check whether should update stembotton status
	
	for(int i=0;i<typeOfStems;i++){
		if(stemDataList[i].enabled){
			if(energy>=stemDataList[i].energyCost&&!stemDataList[i].affordable)	{
				stemDataList[i].affordable = true;
				CCAction* a = (CCAction*)actionList->objectForKey("invibe")->copy();
				a->autorelease();
				stemButtonList[i]->runAction(a);
			}
				
		}
	}



	sprintf(moneyLabel,"$: %d",money);
	mLabel->setString(moneyLabel);
	sprintf(moneyLabel,"Energy: %d",energy);
	eLabel->setString(moneyLabel);
	sprintf(moneyLabel,"Health %d/30",*blood);
	BLabel->setString(moneyLabel);


	//Arm is being released
	/*if (releasingArm && bulletJoint)
	{
		if(armJoint->GetJointAngle() <= CC_DEGREES_TO_RADIANS(10))
		{
			releasingArm = false;
			world->DestroyJoint(bulletJoint);
			bulletJoint = NULL;
			this->schedule(schedule_selector(HelloWorld::resetBullet), 5.0f);
		}
	}

	//Bullet is moving
	if (bulletBody != nullptr && bulletJoint == NULL)
	{
		b2Vec2 position = bulletBody->GetPosition();
		CCPoint myPosition = this->getPosition();
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		if(position.x > screenSize.width / 2.0f / PTM_RATIO)
		{
			myPosition.x = -MIN(960.0f - screenSize.width, position.x * PTM_RATIO - screenSize.width / 2.0f);
			this->setPosition(myPosition);
		}
	}

	set<b2Body*>::iterator pos;
	for(pos = contactListener->contacts.begin(); pos != contactListener->contacts.end(); ++pos)
	{
		b2Body *body = *pos;
		CCNode *contactNode = (CCNode *) body->GetUserData();
		CCPoint position = contactNode->getPosition();
		this->removeChild(contactNode,true);
		world->DestroyBody(body);
		for(vector<b2Body*>::size_type i = 0; i >= targets->size(); i++)
		{
			try
			{
				if(targets->at(i) == body)
				{
					if(i == targets->size()-1)
					{
						printf("asdf");
					}
					targets->erase(targets->begin() + i);
				}
			}
			catch(exception e)
			{
				enemies->clear();
				break;
			}
		}
		/*for(vector<b2Body*>::size_type i = 0; i >= enemies->size(); i++)
		{
			try
			{
				if(enemies->at(i) == body)
				{
					if(i == enemies->size()-1)
					{
						printf("asdf");
					}
					enemies->erase(enemies->begin() + i);
				}
			}
			catch(exception e)
			{
				enemies->clear();
				break;
			}
		}*/

		/*CCParticleSun* explosion = new CCParticleSun();
		explosion->initWithTotalParticles(200);
		//explosion->setTotalParticles(200);
		explosion->setIsAutoRemoveOnFinish(true);
		explosion->setStartSize(10.0f);
		explosion->setSpeed(70.0f);
		explosion->setAnchorPoint(ccp(0.5f,0.5f));
		explosion->setPosition(position);
		explosion->setDuration(1.0f);
		CCTexture2D *tex = new CCTexture2D();
		CCImage *img = new CCImage();
		img->initWithImageFile("fire.png");
		tex->initWithImage(img);
		explosion->setTexture(tex);
		this->addChild(explosion, 11);
		explosion->release();
	}

	contactListener->contacts.clear();*/
}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{

}

void HelloWorld::gameLogic(ccTime dt)
{
	//	this->addTarget();
}

void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue)
{
	//float y = pAccelerationValue->y;ht
	//float x = pAccelerationValue->x;
	//float z = pAccelerationValue->z;
	//b2Vec2* gravity = new b2Vec2(x * 10, y * 10);
	//worldC.SetGravity(*gravity);
	printf("sdfg");
}

void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent *event)
{
	if(degreeFlag==true){
		this->removeChild(degreeLayer,true);
		degreeFlag=false;
	}

	int time = this->millisecondNow();


	bool add = false;
	int type = -1;
	CCSetIterator it = touches->begin();
	CCTouch* touch;
	CCPoint pt;
	for( int iTouchCount = 0; iTouchCount < touches->count(); iTouchCount++ ){
		touch = (CCTouch*)(*it);
		CCPoint p = touch->locationInView();
		float a,b;
		a=this->getPosition().x/PTM_RATIO;
		b= this->getPosition().y/PTM_RATIO;
		p = CCDirector::sharedDirector()->convertToGL( p );	
		for(int i=0;i<typeOfStems;i++){
			if(StemTouchAreaList[i].inSide(p)&&stemDataList[i].enabled){
				stemButtonOffList[i]->setIsVisible(true);
				currentBotton = i;
				iTouchCount = touches->count();
				type= i;
				break;
			}	
		}
		it++;	
	}

	if(type!=-1&&energy>=stemDataList[type].energyCost){
			energy-=stemDataList[type].energyCost;
			add= true;		
		}

		if(add){
	
			//************************//
			this->createStem(type);level++;
			for (int i = 0; i < typeOfStems; i++)
			{
				if(stemDataList[i].enabled==false&&stemAvalibleAfter[i]<=level)
					stemDataList[i].enabled = true;
			}
			maxCells+=levelDelta;
				(level%5)*(level+10)/10;
			if(level<maxLevel)
				cellRefreshRate-=cellRateDelta;
			if(blocks->size()==maxNumOfBlocks)
				shouldgo+=blockHeight;
		
			//check whether can aford stems;
		
			//fixme!!!! should be type 
			for(int i=0;i<typeOfStems;i++){
				if(stemDataList[i].enabled){
					if(energy<stemDataList[i].energyCost&&stemDataList[i].affordable){
						stemDataList[i].affordable = false;				
						CCAction* a = (CCAction*)actionList->objectForKey("gradout")->copy();
						a->autorelease();
						stemButtonList[i]->runAction(a);
					}
					
				}
			}

			//************************//
		
			/*CCSprite* old = stemButtonList[type];
			enableStem(type);
			stemButtonList[type]->setPosition(old->getPosition());
			//CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(stemDataList[type].imagePath[stemDataList[type].currentSubType].c_str());
			//stemButtonList[type]->setTexture(tex);
			CCFiniteTimeAction* a = (CCFiniteTimeAction*)actionList->objectForKey("move1")->copy();
			a->autorelease();
			CCCallFuncN *instant = new CCCallFuncN;
			instant->initWithTarget(old, callfuncN_selector(HelloWorld:: cleanitself));
			CCSequence* doAndDie = CCSequence::actionOneTwo(a,instant);
			old->runAction(doAndDie);*/


			//m_grossini->runAction(instant);

		}




	//************************//
	if(time-lastTimeInMs<400){
		//energy+=2


		
	
	}else{
		lastTimeInMs = time;
	}

	//check button clicked
	it = touches->begin();
    for( int iTouchCount = 0; iTouchCount < touches->count(); iTouchCount++ )
    {
			touch = (CCTouch*)(*it);
			CCPoint p = touch->locationInView();
			float a,b;
			a=this->getPosition().x/PTM_RATIO;
			b= this->getPosition().y/PTM_RATIO;

			p = CCDirector::sharedDirector()->convertToGL( p );	

			p = convertToNodeSpace( p );
			p.x=p.x/PTM_RATIO;
			p.y=p.y/PTM_RATIO;
			CCPoint p0 = p,p1 = p;
			p0.x-=10.0f;p0.y-=10.0f;
			p1.x+=10.0f;p1.y+=10.0f;
			b2AABB aabb;
			aabb.lowerBound = b2Vec2(p0.x,p0.y);
			aabb.upperBound = b2Vec2(p1.x,p1.y);
			b2Vec2 pp = b2Vec2(p.x,p.y);
			QueryCallback callback(pp);
			
			world->QueryAABB(&callback, aabb);
			if (callback.m_fixture)
			{
				
				if((int)callback.m_fixture->GetUserData()==1){
					b2Body* body = callback.m_fixture->GetBody();
				isKilled->insert(body);
				}
				
				int tmp = (int)callback.m_fixture->GetUserData();
				callback.m_fixture->SetUserData((void*)(tmp-1));

			}
			it++;	
	}

}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{

	/*if(mouseJoint == nullptr)
		return;
	CCTouch* touch = (CCTouch*)(touches->anyObject());
	CCPoint location = touch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
	mouseJoint->SetTarget(locationWorld);
	float angle = armJoint->GetJointAngle();*/

	printf("asdf");
}

// cpp with cocos2d-x
void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	if(currentBotton!=-1){
		stemButtonOffList[currentBotton]->setIsVisible(false);
		currentBotton = -1;
	}
	if(blocks->size()==maxNumOfBlocks){
	b2Body* b= blocks->front();
	blocks->pop();
	//currentheight += ((CCSprite*)b->GetUserData())->getContentSize().width;
	this->removeChild((CCNode*)b->GetUserData(),true);
	if(freezeblocks->find(b)!=freezeblocks->end()){
		freezeblocks->erase(b);
	}
	world->DestroyBody(b);}
}

void HelloWorld::updateBox2D(ccTime dt)
{
	//this->setIsAccelerometerEnabled(true);
	//if (start)
	//{
	world->Step(dt, 6, 2);
	world->ClearForces();
	CCMutableArray<CCSprite*>::CCMutableArrayIterator it, jt;

	for(UINT index = 0; index < _b2Boxes.size(); ++index)
	{
		try {
			b2Body *body = _b2Boxes.at(index);
			b2Vec2 position = body->GetPosition();
			float32 angle = body->GetAngle() *-1.0f;
			CCSprite *box = _CCBoxes.at(index);
			float ccspriteangle = box->getRotation();
			box->setPosition(ccp(position.x * MET_TO_PIX, position.y * MET_TO_PIX));
			box->setRotation(CC_RADIANS_TO_DEGREES(angle));
			body->SetActive(true);
			body->SetAwake(true);

		}
		catch(exception e)
		{
		}
	}
	//}
	//b2Vec2 position = bodyC->GetPosition();
	//float32 angle = bodyC->GetAngle();
	//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	//if (position.x < 1.55){
	//Resting step
	//}
	//if(bodyC->IsAsleep())
	//{
	//	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	//}

}

void HelloWorld::updateGame(ccTime dt)
{

}

void HelloWorld::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
	CCTouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0);
}



void HelloWorld::createStem(int type)
{
	const char * imageName = stemDataList[type].imagePath[stemDataList[type].currentSubType].c_str();
	CCSprite *sprite = CCSprite::spriteWithFile(imageName);
	
	



	CCPoint position = ccp( CCDirector::sharedDirector()->getWinSize().width/2-blockWidth/2,currentheight);
	this->addChild(sprite, 1);
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
	(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body *body = world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	b2PolygonShape box;
	box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
	boxDef.shape = &box;
	boxDef.userData = (void*)stemDataList[type].life;
	boxDef.filter.groupIndex=type+1;
	boxDef.density = 0.8f;
	boxDef.restitution = 0.0f;
	body->CreateFixture(&boxDef);
	targets->push_back(body);

	if(type==1){
		b2EdgeShape myedge;
		b2FixtureDef boxShapeDef;
		boxShapeDef.shape = &myedge;
		boxShapeDef.filter.groupIndex = 4;
		double gap = 4.0;
		//bottom
		float w = sprite->getContentSize().width/2.0f/PTM_RATIO;
		float h = sprite->getContentSize().width/2.0f/PTM_RATIO;

		CCAnimation *animation = new CCAnimation();
		animation->init();
 
		//************************//
 // load image file from local file system to CCSpriteFrame, then add into CCAnimation
		for (int i = 1; i <= 8; i++){
			char szImageFileName[128] = {0};
			sprintf(szImageFileName, "freeze_f%d.png",i);
			animation->addFrameWithFileName(szImageFileName);
		}
		animation->setDelay(1.4f / 14.0f);
		 // This animation contains 14 frames, will continuous 2.8 seconds.
		animation->autorelease(); // Return to the 1st frame after the 14th frame is played. 
	
		CCAnimate *action = CCAnimate::actionWithAnimation(animation); 
		sprite->runAction(action); 
		//************************//
		myedge.Set(b2Vec2(-2*w,-2*h), b2Vec2(2*w,-2*h));
		body->CreateFixture(&boxShapeDef);
		myedge.Set(b2Vec2(2*w,-2*h), b2Vec2(2*w,2*h));
		body->CreateFixture(&boxShapeDef);
		myedge.Set(b2Vec2(-2*w,-2*h), b2Vec2(-2*w,2*h));
		body->CreateFixture(&boxShapeDef);
		myedge.Set(b2Vec2(-2*w,2*h), b2Vec2(2*w,2*h));
		body->CreateFixture(&boxShapeDef);
		freezeblocks->insert(body);
		this->schedule(schedule_selector(HelloWorld::removeTheWall),2.0f);
	}



		if(type==2){
			b2EdgeShape myedge;
			b2FixtureDef boxShapeDef;
			boxShapeDef.shape = &myedge;
			boxShapeDef.filter.groupIndex = 5;
			float gap = 4.0;
			//bottom
			float w = sprite->getContentSize().width/2.0f/PTM_RATIO;
			float h = sprite->getContentSize().width/2.0f/PTM_RATIO;

			CCAnimation *animation = new CCAnimation();
			animation->init();
 
	 // load image file from local file system to CCSpriteFrame, then add into CCAnimation
			/*for (int i = 1; i <= 8; i++){
				char szImageFileName[128] = {0};
				sprintf(szImageFileName, "freeze_f%d.png",i);
				animation->addFrameWithFileName(szImageFileName);
			}
			animation->setDelay(1.4f / 14.0f);
			 // This animation contains 14 frames, will continuous 2.8 seconds.
			animation->autorelease(); // Return to the 1st frame after the 14th frame is played. 
	
			CCAnimate *action = CCAnimate::actionWithAnimation(animation); 
			sprite->runAction(action); */
			b2FixtureDef boxDef;
			b2PolygonShape box;
			box.SetAsBox(1.0f*sprite->getContentSize().width/PTM_RATIO, 1.0f*sprite->getContentSize().height/PTM_RATIO);
			boxDef.shape = &box;
			boxDef.userData = (void*)stemDataList[0].life;
			boxDef.filter.groupIndex=5;
			boxDef.density = 0.8f;
			boxDef.restitution = 0.0f;
			body->CreateFixture(&boxDef);
			fireblocks->insert(body);

			CCPoint position = ccp( CCDirector::sharedDirector()->getWinSize().width/2,currentheight);

			//************************//
			CCParticleSun* explosion = new CCParticleSun();
			explosion->initWithTotalParticles(100);
			//explosion->setTotalParticles(200);
			explosion->setIsAutoRemoveOnFinish(true);
			explosion->setStartSize(200.0f);
			explosion->setSpeed(140.0f);
			explosion->setAnchorPoint(ccp(0.5f,0.5f));
			explosion->setPosition(position);
			explosion->setDuration(0.5f);
			CCTexture2D *tex = new CCTexture2D();
			CCImage *img = new CCImage();
			img->initWithImageFile("fire.png");
			tex->initWithImage(img);
			explosion->setTexture(tex);
			this->addChild(explosion, 11);
			explosion->release();
			this->schedule(schedule_selector(HelloWorld::removeBomb),0.5f);
		}




	//myedge.Set(b2Vec2(bodyDef.position.x-3.0,bodyDef.position.y-3.0), b2Vec2(bodyDef.position.x+blockWidth+3.0,bodyDef.position.y+blockHeight+3.0));



	blocks->push(body);
	lastBlock = body;
	currentheight+=sprite->getContentSize().height;
		if(stemDataList[type].maxSubType>1)
				stemDataList[type].currentSubType = (stemDataList[type].currentSubType+ rand()%(stemDataList[type].maxSubType-1)+1)%stemDataList[type].maxSubType;
			else
				stemDataList[type].currentSubType =0;



}


void HelloWorld::createCell(int type)
{
	//const char *imageName, CCPoint position, float rotation, bool isCircle, bool isStatic, bool isEnemy
	const char *imageName = cellDataList[type].imagePath[0].c_str();
	bool left = rand()%2;
	//************************//
	

	CCPoint position;
	if(left)
		position.x = -attackStart;
	else
		position.x = CCDirector::sharedDirector()->getWinSize().width-blockWidth/2+attackStart;
	position.y = rand()%100/100.0f*(CCDirector::sharedDirector()->getWinSize().height-300.0f)+300.0f-this->getPosition().y;
	CCSprite *sprite = CCSprite::spriteWithFile(imageName);
	this->addChild(sprite, 1);
	//************************//

	float miny,maxy;
	maxy = (CCDirector::sharedDirector()->getWinSize().height-position.y);
	miny = position.y;
	int seed = rand()%1000;
	float finaly = (maxy-miny)*seed/1000+miny;
	float w = CCDirector::sharedDirector()->getWinSize().width;
	float h = CCDirector::sharedDirector()->getWinSize().height;
	float sin  = finaly/pow((finaly*finaly+w*w),0.5);
	float cos  = w/pow((finaly*finaly+w*w),0.5);
	int angle = rand()%179;
	if(position.x>20){
		angle = -angle;
		cos=-cos;
	}
	b2BodyDef bodyDef;
	bodyDef.angle = CC_DEGREES_TO_RADIANS(angle);
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
		(position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
	bodyDef.userData = sprite;
	b2Body *body = world->CreateBody(&bodyDef);
	b2FixtureDef boxDef;
	b2CircleShape circle;
	circle.m_radius = sprite->getContentSize().width/2.0f/PTM_RATIO;
	boxDef.shape = &circle;
	boxDef.userData = (void*)cellDataList[type].life;
	boxDef.filter.groupIndex=-(type+1);
	boxDef.filter.categoryBits=0x0002;
	boxDef.filter.maskBits=0xFFFD;
	boxDef.density = 0.8f;
	boxDef.restitution = 0.0f;
	body->CreateFixture(&boxDef);





	printf("asdf");
	
	targets->push_back(body);
	currentCells++;

	//************************//
	
	float force  = rand()%30+30+helloDegree*10.0;
	//body->ApplyForceToCenter(b2Vec2(force*sin(CC_DEGREES_TO_RADIANS(angle)),force*cos(CC_DEGREES_TO_RADIANS(angle))));
	body->ApplyForceToCenter(b2Vec2(force*cos,-force*sin));
	//body->ApplyAngularImpulse(force);

	
	
	
}


void HelloWorld::enableStem(int type){
	
	//************************//
		
		sprintf(moneyLabel,"%d",stemDataList[type].energyCost);
		CCLabelTTF* t = CCLabelTTF::labelWithString(moneyLabel, "Comic Sans MS", 12);
		t->setPosition(ccp(10,10));
		t->setColor(ccc3(160, 80, 5));
		
	
		stemButtonList[type] = CCSprite::spriteWithFile(stemDataList[type].buttonImage[0].c_str());
		stemButtonList[type]->setAnchorPoint(ccp(0.5,0.5));
		stemButtonList[type]->setPosition(ccp(StemTouchAreaList[type].leftbot.x+stemButtonList[type]->getContentSize().width/2,StemTouchAreaList[type].leftbot.y+stemButtonList[type]->getContentSize().height/2));
		stemButtonList[type]->setOpacity(150);
		this->addChild(stemButtonList[type], 2);
		stemButtonList[type]->addChild(t, 0);


		stemButtonOffList[type] = CCSprite::spriteWithFile(stemDataList[type].buttonImage[1].c_str());
		stemButtonOffList[type]->setAnchorPoint(ccp(0.5,0.5));
		stemButtonOffList[type]->setPosition(ccp(StemTouchAreaList[type].leftbot.x+stemButtonList[type]->getContentSize().width/2,StemTouchAreaList[type].leftbot.y+stemButtonList[type]->getContentSize().height/2));
		stemButtonOffList[type]->setOpacity(255);
		stemButtonOffList[type]->setIsVisible(false);
		this->addChild(stemButtonOffList[type], 3);
		t = CCLabelTTF::labelWithString(moneyLabel, "Comic Sans MS", 12);
		t->setPosition(ccp(10,10));
		t->setColor(ccc3(160, 80, 5));
		stemButtonOffList[type]->addChild(t, 0);



		

			
}



int HelloWorld:: millisecondNow()
{
  // Something like GetTickCount but portable
  // It rolls over every ~ 12.1 days (0x100000/24/60/60)
  // Use GetMilliSpan to correct for rollover
  timeb tb;
  ftime( &tb );
  int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
  return nCount;
}

void HelloWorld:: cleanitself(CCNode* node){
	this->removeChild(node,1);
}

void HelloWorld::removeTheWall(cocos2d::ccTime dt){
	this->unschedule(schedule_selector(HelloWorld::removeTheWall));
	set<b2Body*>::iterator pos;
	for(pos = freezeblocks->begin(); pos != freezeblocks->end(); ++pos)
	{

		b2Body *body = *pos;
		b2Fixture* a = body->GetFixtureList();
		vector<b2Fixture*> fs;
		for(b2Fixture* i = a;i;i=i->GetNext()){
			if(i->GetFilterData().groupIndex==4){
				fs.push_back(i);
				
			}
		}

		for(int i=0;i<fs.size();i++){
			body->DestroyFixture(fs.at(i));
		}
		


	}
	freezeblocks->clear();

	for(pos = freezedByMe->begin(); pos != freezedByMe->end(); ++pos)
	{

		b2Body *body = *pos;
		body->SetAwake(true);
		//************************//
		float force  = rand()%30+30;
		body->ApplyForceToCenter(b2Vec2(force*sin(body->GetAngle()),force*cos(body->GetAngle())));
	}
	freezedByMe->clear();

}



void HelloWorld::removeBomb(cocos2d::ccTime dt){
	this->unschedule(schedule_selector(HelloWorld::removeTheWall));
	set<b2Body*>::iterator pos;
	for(pos = fireblocks->begin(); pos != fireblocks->end(); ++pos)
	{

		b2Body *body = *pos;
		b2Fixture* a = body->GetFixtureList();
		vector<b2Fixture*> fs;
		for(b2Fixture* i = a;i;i=i->GetNext()){
			if(i->GetFilterData().groupIndex==5){
				fs.push_back(i);
				
			}
		}

		for(int i=0;i<fs.size();i++){
			body->DestroyFixture(fs.at(i));
		}
		


	}
	fireblocks->clear();


	
}


void HelloWorld::menuPauseCallback(CCObject* pSender){

	//if(degreeFlag==true)this->removeChild(degreeLayer,true);
	pauseLabel=CCLabelTTF::labelWithString("Pause!","Marker Felt",24);
	if(pause==false){
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		pauseLayer=CCLayerColor::layerWithColorWidthHeight(ccc4f(255,255,255,100),200.0f,150.0f);
		pauseLabel->setPosition(ccp(100.0f,75.0f));
		pauseLayer->setPosition((s.width-200.0f)/2,(s.height-120.0f)/2+moveBy);
		
		pauseLayer->addChild(pauseLabel,0);
		this->addChild(pauseLayer,10);
		this->setIsTouchEnabled(false);
		CCDirector::sharedDirector()->pause();
		pause=true;
	}else{
		this->removeChild(pauseLayer,true);
		this->setIsTouchEnabled(true);
		CCDirector::sharedDirector()->resume();
		pause=false;
	}
}

void HelloWorld::menuRestartCallback(CCObject* pSender){
	//if(vicFlag==true)this->removeChild(vicLayer,true);
	//if(degreeFlag==true)this->removeChild(degreeLayer,true);
	CCScene *pScene = HelloWorld::scene(helloDegree);
	if(pause==true){
		CCDirector::sharedDirector()->resume();
		pause=false;
	}
	this->unscheduleAllSelectors();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(0.5f, pScene));
}

void HelloWorld::menuBackCallback(CCObject* pSender){
	CCScene *pScene = MainMenu::scene(helloDegree);
	if(pause=true){
		CCDirector::sharedDirector()->resume();
		pause=false;
	}
	CCDirector::sharedDirector()->replaceScene(pScene);
}



void HelloWorld::Victory(){
	this->removeChild(inMenu,true);
	vicFlag=true;
	/*if(pause==true){
		CCDirector::sharedDirector()->resume();
		pause=false;
	}*/
	vicLabel=CCLabelTTF::labelWithString("Game Over!","Comic Sans MS",24);
	//vicLabel->setColor(ccc3(0,0,0));
	CCSize s=CCDirector::sharedDirector()->getWinSize();
	CCMenuItem *Restart=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("restart.png" ),CCSprite::spriteWithFile("restart.png"),this,menu_selector(HelloWorld::menuRestartCallback));
	CCMenuItem *Main=CCMenuItemSprite::itemFromNormalSprite(CCSprite::spriteWithFile("back.png" ),CCSprite::spriteWithFile("back.png"),this,menu_selector(HelloWorld::menuBackCallback));

	vicMenu=CCMenu::menuWithItems(Restart,Main,NULL);
	vicMenu->alignItemsHorizontally();

	vicLayer=CCLayerColor::layerWithColorWidthHeight(ccc4f(255,255,255,100),200.0f,150.0f);
	vicLabel->setPosition(ccp(100.0f,80.0f));
	vicMenu->setPosition(ccp(100.0f,30.0f));

	vicLayer->setPosition((s.width-200.0f)/2, (s.height-120.0f)/2+moveBy);
	vicLayer->addChild(vicLabel,0);
	vicLayer->addChild(vicMenu,1);
	this->addChild(vicLayer,10);
}