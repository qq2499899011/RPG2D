#include "RPG2Dpch.h"
#include "ContactListener.h"
#include "RPG2D/Resource/ResType/Entity.h"
#include "RPG2D/Resource/ResType/ScriptableEntity.h"
namespace RPG2D {
	void ContactListener::BeginContact(b2Contact* contact)
	{
		//RPG2D_CORE_INFO("contact begin");
		//�ֱ���ýű�����е�OnCollisionBegin,���Ұ���ײ�Ķ����롣
		 // ��ȡ��ײ�������о�
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		// ��ȡ��ײ����������
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();
		//��ȡ����ʵ��
		uint64_t idA = static_cast<uint64_t>(bodyA->GetUserData().pointer);
		uint64_t idB = static_cast<uint64_t>(bodyB->GetUserData().pointer);
		Entity entityA = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idA, false));
		Entity entityB = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idB, false));
		//����Ƿ��нű����������У�����OnConllisionBegin
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
		// ��ȡ��ײ����������,��Ҫ�ж�Body�Ƿ���Ч
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();
		//��ȡ����ʵ��
		uint64_t idA = static_cast<uint64_t>(bodyA->GetUserData().pointer);
		uint64_t idB = static_cast<uint64_t>(bodyB->GetUserData().pointer);
		Entity entityA = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idA, false));
		Entity entityB = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idB, false));
		//����Ƿ��нű����������У�����OnConllisionBegin
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
		//��ײ�����У�ÿһ��֡������á�
		RPG2D_CORE_INFO("contact happen");		
		//�ֱ���ýű�����е�OnCollisionEnd,���Ұ���ײ�Ķ����롣
		 // ��ȡ��ײ�������о�
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		// ��ȡ��ײ����������
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();
		//��ȡ����ʵ��
		uint64_t idA = static_cast<uint64_t>(bodyA->GetUserData().pointer);
		uint64_t idB = static_cast<uint64_t>(bodyB->GetUserData().pointer);
		Entity entityA = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idA, false));
		Entity entityB = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetEntityByUID(UID(idB, false));
		//����Ƿ��нű����������У�����OnConllisionBegin
		bool canA = true, canB = true;
		if (entityA.HasComponent<NativeScriptComponent>()) {
			canA = entityA.GetComponent<NativeScriptComponent>().Instance->OnCollision(entityB);
		}
		if (entityB.HasComponent<NativeScriptComponent>()) {
			canB = entityB.GetComponent<NativeScriptComponent>().Instance->OnCollision(entityA);
		}
	}
}
