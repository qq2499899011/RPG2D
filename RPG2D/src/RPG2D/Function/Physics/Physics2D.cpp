#include "RPG2Dpch.h"
#include "Physics2D.h"
#include "RPG2D/Function/Global/GlobalContext.h"
#include "RPG2D/Function/Script/Entity.h"
#include "RPG2D/Function/Script/Components.h"
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
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}
	//�����ʼ����Ҫ�����������֮����е��á�
	void PhysicsSystem::Init()
	{
		//�л�����ʱ����
		//ͨ��SceneManager��ȡ��Ӧ��view
		//�����������ٶȷ���
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		//�ҵ����к��и��������ʵ��
		//ֱ��ͨ��SceneManager��ȡview
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->SetPhysicsWorld(m_PhysicsWorld);
		auto view = m_Registry->view<Rigidbody2DComponent>();
		//����ʵ�壬����ÿ������ʵ�壬��������box2d�еĶ�Ӧbody��
		for (auto e : view)
		{
			//�������ʵ���볡����ϵ
			Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
			//��ȡtransform�͸������
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			//����һ��b2Body
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;


			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
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
	}
	void PhysicsSystem::Update(Timestep ts)
	{
		//����ģ�������
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		// �������о��и��������ʵ��
		auto view = m_Registry->view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			//�½�ʵ��
			Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
			//��ȡʵ������
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			//��ȡb2Body
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			//����b2Body��ȡ��������λ��,�������transform�����
			const auto& position = body->GetPosition();
			transform.Translation.x = position.x;
			transform.Translation.y = position.y;
			transform.Rotation.z = body->GetAngle();
		}
	}
}