#include "RPG2Dpch.h"
#include "ContactListener.h"
#include "RPG2D/Resource/ResType/Entity.h"
#include "RPG2D/Resource/ResType/ScriptableEntity.h"
namespace RPG2D {
	void ContactListener::BeginContact(b2Contact* contact)
	{
		//RPG2D_CORE_INFO("contact begin");
		//分别调用脚本组件中的OnCollisionBegin,并且把碰撞的对象传入。
		 // 获取碰撞的两个夹具
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		// 获取碰撞的两个物体
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();
		//获取两个实体
		uint64_t idA = static_cast<uint64_t>(bodyA->GetUserData().pointer);
		uint64_t idB = static_cast<uint64_t>(bodyB->GetUserData().pointer);
		Entity entityA = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idA, false));
		Entity entityB = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idB, false));
		//检查是否有脚本组件，如果有，调用OnConllisionBegin
		bool canA = true, canB = true;
		if (entityA.HasComponent<NativeScriptComponent>()) {
			canA = entityA.GetComponent<NativeScriptComponent>().Instance->OnCollisionBegin(entityB);
		}
		if (entityB.HasComponent<NativeScriptComponent>()) {
			canB = entityB.GetComponent<NativeScriptComponent>().Instance->OnCollisionBegin(entityA);
		}
		//contact->SetEnabled(canA&&canB);
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		//RPG2D_CORE_INFO("contact end");		
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		// 获取碰撞的两个物体,需要判断Body是否有效
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();
		//获取两个实体
		uint64_t idA = static_cast<uint64_t>(bodyA->GetUserData().pointer);
		uint64_t idB = static_cast<uint64_t>(bodyB->GetUserData().pointer);
		Entity entityA = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idA, false));
		Entity entityB = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idB, false));
		//检查是否有脚本组件，如果有，调用OnConllisionBegin
		bool canA = true, canB = true;
		if (entityA.HasComponent<NativeScriptComponent>()) {
			canA = entityA.GetComponent<NativeScriptComponent>().Instance->OnCollisionEnd(entityB);
		}
		if (entityB.HasComponent<NativeScriptComponent>()) {
			canB = entityB.GetComponent<NativeScriptComponent>().Instance->OnCollisionEnd(entityA);
		}
	}
	void ContactListener::PreSolve(b2Contact* contact, const b2Manifold*)
	{
		//碰撞过程中，每一次帧都会调用。
		RPG2D_CORE_INFO("contact happen");		
		//分别调用脚本组件中的OnCollisionEnd,并且把碰撞的对象传入。
		 // 获取碰撞的两个夹具
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		// 获取碰撞的两个物体
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();
		//获取两个实体
		uint64_t idA = static_cast<uint64_t>(bodyA->GetUserData().pointer);
		uint64_t idB = static_cast<uint64_t>(bodyB->GetUserData().pointer);
		Entity entityA = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idA, false));
		Entity entityB = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idB, false));
		//检查是否有脚本组件，如果有，调用OnConllisionBegin
		bool canA = true, canB = true;
		if (entityA.HasComponent<NativeScriptComponent>()) {
			canA = entityA.GetComponent<NativeScriptComponent>().Instance->OnCollision(entityB);
		}
		if (entityB.HasComponent<NativeScriptComponent>()) {
			canB = entityB.GetComponent<NativeScriptComponent>().Instance->OnCollision(entityA);
		}
	}
}
