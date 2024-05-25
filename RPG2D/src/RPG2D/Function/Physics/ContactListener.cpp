#include "RPG2Dpch.h"
#include "ContactListener.h"
#include "RPG2D/Resource/ResType/Entity.h"
#include "RPG2D/Resource/ResType/ScriptableEntity.h"
namespace RPG2D {
	void ContactListener::BeginContact(b2Contact* contact)
	{
		//RPG2D_CORE_INFO("contact begin");
		//分别调用脚本组件中的OnCollisionEngin,并且把碰撞的对象传入。
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
		if (entityA.HasComponent<NativeScriptComponent>()) {
			entityA.GetComponent<NativeScriptComponent>().Instance->OnCollisionBegin(entityB);
		}
		if (entityB.HasComponent<NativeScriptComponent>()) {
			entityB.GetComponent<NativeScriptComponent>().Instance->OnCollisionBegin(entityA);
		}
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		//RPG2D_CORE_INFO("contact end");
	}
}
