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

#include <set>
#include <algorithm>
#include "tool.h"

class MyContactListener : public b2ContactListener {
public:
	set<b2Body*> contacts;
	set<b2Body*> freezed;
	set<b2Body*> boomed;
	cellData* celldata;
	//cellData* celldata;
	bool died;
	int* count;
	MyContactListener();
	~MyContactListener();
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);   
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};