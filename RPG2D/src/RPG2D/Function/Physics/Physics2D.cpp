#include "RPG2Dpch.h"
#include "Physics2D.h"
#include "RPG2D/Function/Global/GlobalContext.h"
#include "RPG2D/Resource/Scene/Entity.h"
namespace RPG2D {
	PhysicsSystem::PhysicsSystem()
	{
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}
	void PhysicsSystem::Init()
	{
		//通过SceneManager获取相应的view
		//根据
		//设置重力加速度方向
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		//找到所有含有刚体组件的实体
		//直接通过SceneManager获取view
		Ref<entt::registry> m_Registry = GlobalContext::GetInstace()->m_SceneManager->GetRegistry();
		auto view = m_Registry->view<Rigidbody2DComponent>();
		//遍历实体，对于每个刚体实体，建立其在box2d中的对应body。
		for (auto e : view)
		{
			//清晰表达实体与场景关系
			Entity entity = { e, GlobalContext::GetInstace()->m_SceneManager->GetSceneActive().get()};
			//获取transform和刚体组件
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			//生成一个b2Body
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
		//更新模拟情况。
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
		Ref<entt::registry> m_Registry = GlobalContext::GetInstace()->m_SceneManager->GetRegistry();
		// 遍历所有具有刚体组件的实体
		auto view = m_Registry->view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			//新建实体
			Entity entity = { e, GlobalContext::GetInstace()->m_SceneManager->GetSceneActive().get()};
			//获取实体的组件
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			//获取b2Body
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			//根据b2Body获取物体最新位置,并输出到transform组件中
			const auto& position = body->GetPosition();
			transform.Translation.x = position.x;
			transform.Translation.y = position.y;
			transform.Rotation.z = body->GetAngle();
		}
	}
}