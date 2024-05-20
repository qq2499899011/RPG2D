#include "RPG2Dpch.h"
#include "Physics2D.h"
#include "RPG2D/Function/Global/GlobalContext.h"
#include "RPG2D/Resource/ResType/Entity.h"
#include "RPG2D/Resource/ResType/Components.h"
#include "ContactListener.h"
// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

//��box2d�е�body����������ж����body���ͽ����໥ת��
namespace RPG2D {
	inline b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
		case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
		case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		RPG2D_CORE_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}

	inline Rigidbody2DComponent::BodyType Rigidbody2DTypeFromBox2DBody(b2BodyType bodyType)
	{
		switch (bodyType)
		{
		case b2_staticBody:    return Rigidbody2DComponent::BodyType::Static;
		case b2_dynamicBody:   return Rigidbody2DComponent::BodyType::Dynamic;
		case b2_kinematicBody: return Rigidbody2DComponent::BodyType::Kinematic;
		}

		RPG2D_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}
}
namespace RPG2D {
	PhysicsSystem::PhysicsSystem()
	{
		m_Contact =new ContactListener();
		m_PixelPerMeter = 50.0f;
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}
	//�л�����ʱ����
	void PhysicsSystem::Init()
	{
		
		//�����������ٶȷ���
		m_PhysicsWorld = new b2World({ 0.0f, 9.8f });
		//���bodyToDestroy
		m_BodiesToDestroy.clear();
		//������ײ���
		m_PhysicsWorld->SetContactListener(m_Contact);
		//�ҵ����к��и��������ʵ��
		//ֱ��ͨ��SceneManager��ȡview
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->SetPhysicsWorld(m_PhysicsWorld);
		/*
		auto view = m_Registry->view<Rigidbody2DComponent>();
		//����ʵ�壬����ÿ������ʵ�壬��������box2d�еĶ�Ӧbody��
		//������Ԥ����֮����Ҫִ��һ��������̡�
		for (auto e : view)
		{
			Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
			AddEntity(entity);
		}
		*/
	}
	void PhysicsSystem::Update(Timestep ts)
	{
		//��ɾ�������еĴ�ɾ������
		DestroyBodies();
		//����ģ�������
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
		
		/*
		//��ȡ��ײ���
		b2Contact* contact = m_PhysicsWorld->GetContactList();
		while (contact!=nullptr)
		{
			b2Fixture* fixtureA = contact->GetFixtureA();
			b2Fixture* fixtureB = contact->GetFixtureB();

			// ��ȡ��ײ����������
			b2Body* bodyA = fixtureA->GetBody();
			b2Body* bodyB = fixtureB->GetBody();

			// ��ȡ��ײ�����������λ��
			b2Vec2 positionA = bodyA->GetPosition();
			b2Vec2 positionB = bodyB->GetPosition();

			// ��ӡ��ײ�����������λ��
			std::cout << "Collision occurred between objects A at (" << positionA.x << ", " << positionA.y << ")"
				<< " and B at (" << positionB.x << ", " << positionB.y << ")" << std::endl;
			contact = contact->GetNext();
			//
		}
		
		*/
		
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		// �������о��и��������ʵ��
		auto view = m_Registry->view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			if (InputSystem::IsKeyPressed(Key::K)) {
				RPG2D_CORE_INFO("good");
			}
			//�½�ʵ��
			Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
			//��ȡʵ������
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			//��ȡb2Body
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			//����b2Body��ȡ��������λ��,�������transform�����
			const auto& position = body->GetPosition();
			transform.Translation.x = position.x * m_PixelPerMeter;
			transform.Translation.y = position.y * m_PixelPerMeter;
			transform.Rotation.z = body->GetAngle();
		}
	}
	//Ĭ������λ�ñ���������10.
	void PhysicsSystem::AddEntity(Entity entity)
	{
		if (!entity.HasComponent<Rigidbody2DComponent>())return;
		//��ȡtransform�͸������
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		//����body���壺���� λ�� �Ƕ�
		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
		bodyDef.position.Set(transform.Translation.x/m_PixelPerMeter, transform.Translation.y/m_PixelPerMeter);
		bodyDef.angle = transform.Rotation.z;
		bodyDef.userData.pointer = static_cast<uintptr_t>(entity.GetUID().getID());
		//���ݶ���,����һ��b2Body
		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.FixedRotation);
		rb2d.RuntimeBody = body;
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			//�Զ��������ײ��С�����2������������ײ��ʱ��ֱ������ʵ�ʴ�С���ɡ�
			b2PolygonShape boxShape;
			boxShape.SetAsBox(bc2d.Size.x/(m_PixelPerMeter*2), bc2d.Size.y/(2*m_PixelPerMeter), b2Vec2(bc2d.Offset.x/m_PixelPerMeter, bc2d.Offset.y/m_PixelPerMeter), 0.0f);
			//boxShape.SetAsBox(bc2d.Size.x, bc2d.Size.y);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.isSensor = bc2d.isSensor;
			fixtureDef.density = bc2d.Density;
			fixtureDef.friction = bc2d.Friction;
			fixtureDef.restitution = bc2d.Restitution;
			fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
			body->CreateFixture(&fixtureDef);
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

			b2CircleShape circleShape;
			circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
			circleShape.m_radius = transform.Scale.x * cc2d.Radius;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = cc2d.Density;
			fixtureDef.friction = cc2d.Friction;
			fixtureDef.restitution = cc2d.Restitution;
			fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
			body->CreateFixture(&fixtureDef);
		}
	}
	void PhysicsSystem::RemoveEntity(Entity entity)
	{
		if (!entity.HasComponent<Rigidbody2DComponent>())return;
		//ɾ��body
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
		//�����ɾ���б�
		m_BodiesToDestroy.push_back(body);
	}
	//��������λ��
	glm::vec2 PhysicsSystem::GetPixelPosition(Entity entity)
	{
		//��ȡ����
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		//����λ��
		return glm::vec2(body->GetPosition().x*m_PixelPerMeter, body->GetPosition().y* m_PixelPerMeter);
	}
	void PhysicsSystem::SetPositionWithPixel(Entity entity,glm::vec2 pos)
	{
		//��ȡ����
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		b2Vec2 newPos;
		newPos.x = pos.x/m_PixelPerMeter;
		newPos.y = pos.y/m_PixelPerMeter;
		body->SetTransform(newPos, 0.0f);
	}
	//�����ٶȺ�y�᷽���෴��Ҳ��������������
	void PhysicsSystem::SetVelocity(Entity entity, glm::vec2 velocity)
	{
		//��ȡ����
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		//���ó�ʼ�ٶ�
		b2Vec2 v;
		v.x = velocity.x;
		v.y = -velocity.y;
		body->SetLinearVelocity(v);
	}
	//��ȡ�ٶ���Ҫ����opengl�������ᣬ�����Ҫyȡ�෴��
	glm::vec2 PhysicsSystem::GetVelocity(Entity entity)
	{
		//��ȡʵ��
		//��ȡ����
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		//��ȡ�ٶ�
		b2Vec2 v = body->GetLinearVelocity();
		glm::vec2 vec(v.x, -v.y);
		return vec;
	}
	void PhysicsSystem::DestroyBodies()
	{
		for (b2Body* body : m_BodiesToDestroy) {
			m_PhysicsWorld->DestroyBody(body);
		}
		m_BodiesToDestroy.clear();
	}
}