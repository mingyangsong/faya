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
#include "MyContactListener.h"


MyContactListener::MyContactListener()
{
	died=false;
}
MyContactListener::~MyContactListener()
{
}
void MyContactListener::BeginContact(b2Contact* contact)
{
}

void MyContactListener::EndContact(b2Contact* contact)
{
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void MyContactListener::PostSolve(b2Contact* contact,const b2ContactImpulse* impulse)
{
	bool isAEnemy = contact->GetFixtureA()->GetFilterData().groupIndex<0;
	bool isBenemy = contact->GetFixtureB()->GetFilterData().groupIndex<0;
	bool metBlock = ((contact->GetFixtureA()->GetFilterData().groupIndex>0&&contact->GetFixtureA()->GetFilterData().groupIndex<4)
		||(contact->GetFixtureB()->GetFilterData().groupIndex>0&&contact->GetFixtureB()->GetFilterData().groupIndex<4));
	if ((isAEnemy || isBenemy)&&metBlock)
	{

		/*int count = contact->GetManifold()->pointCount;
		float maxImpulse = 0.0f;
		for (int i = 0; i < count ; ++i)
		{
			maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
		}*/
		/*(*count)--;
		if(*count==0){
			died = true;
		}*/
			
		if(isAEnemy){
			contacts.insert(contact->GetFixtureA()->GetBody());
			(*count)-=celldata[-contact->GetFixtureA()->GetFilterData().groupIndex-1].atk;

					if(*count<0){
						*count=0;
			died = true;
		}
			/*int a =(int)contact->GetFixtureB()->GetUserData();
			a--;
			contact->GetFixtureB()->SetUserData((void*)(a));
			if(a<0)
				died = true;*/
		}
				
			if(isBenemy){
				contacts.insert(contact->GetFixtureB()->GetBody());
				(*count)-=celldata[-contact->GetFixtureB()->GetFilterData().groupIndex-1].atk;

					if(*count<0){
						*count=0;
			died = true;
		}
				/*	int a =(int)contact->GetFixtureA()->GetUserData();
				a--;
				contact->GetFixtureA()->SetUserData((void*)(a));
				if(a<0)
					died = true;*/


			}
	}else if(contact->GetFixtureA()->GetFilterData().groupIndex==4||contact->GetFixtureB()->GetFilterData().groupIndex==4){
			if(isAEnemy)
				freezed.insert(contact->GetFixtureA()->GetBody());
			if(isBenemy)
				freezed.insert(contact->GetFixtureB()->GetBody());
	}
	else if(contact->GetFixtureA()->GetFilterData().groupIndex==5||contact->GetFixtureB()->GetFilterData().groupIndex==5){
			if(isAEnemy)
				boomed.insert(contact->GetFixtureA()->GetBody());
			if(isBenemy)
				boomed.insert(contact->GetFixtureB()->GetBody());
	}else if(contact->GetFixtureA()->GetFilterData().groupIndex==7||contact->GetFixtureB()->GetFilterData().groupIndex==7){
	
		if(isAEnemy){
			contacts.insert(contact->GetFixtureA()->GetBody());
		}
				
			if(isBenemy){
				contacts.insert(contact->GetFixtureB()->GetBody());

			}
	
	
	}
}

