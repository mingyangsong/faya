#ifndef __TOOL_H__
#define __TOOL_H__

#include "cocos2d.h"
#include "CCGeometry.h"
#include <string>
#include <wrl\client.h>
#include <memory>
#include <vector>


struct stemData{
	int life;
	string* imagePath;
	string* buttonImage;//0 on 1 off
	bool enabled;
	bool affordable;
	int currentSubType;
	int maxSubType;
	int moneyCost;
	int energyCost;
};

struct cellData{
	int life;
	int atk;
	string* imagePath;
	float speedHigherBound;
	float speedLowerBound;
	float angleHigherBound;
	float angleLowerBound;
	int numOfSubType;
	int moneyGet;
	int energyGet;
};

struct touchArea{
	cocos2d::CCPoint leftbot;
	cocos2d::CCPoint righttop;
	bool inSide(cocos2d::CCPoint p){
		return (p.x>leftbot.x&&p.y>leftbot.y&&p.x<righttop.x&&p.y<righttop.y);
	}
};


#endif