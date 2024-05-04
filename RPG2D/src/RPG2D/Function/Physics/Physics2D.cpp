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

//将box2d中的body类型与组件中定义的body类型进行相互转换
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
	//切换场景时调用
	void PhysicsSystem::Init()
	{
		//设置重力加速度方向
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		//找到所有含有刚体组件的实体
		//直接通过SceneManager获取view
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->SetPhysicsWorld(m_PhysicsWorld);
		auto view = m_Registry->view<Rigidbody2DComponent>();
		//遍历实体，对于每个刚体实体，建立其在box2d中的对应body。
		//当生成预制体之后，需要执行一次这个过程。
		for (auto e : view)
		{
			Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
			AddEntity(entity);
		}
	}
	void PhysicsSystem::Update(Timestep ts)
	{
		//更新模拟情况。
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		// 遍历所有具有刚体组件的实体
		auto view = m_Registry->view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			//新建实体
			Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
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
	void PhysicsSystem::AddEntity(Entity entity)
	{
		//获取transform和刚体组件
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

		//设置body定义：类型 位置 角度
		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
		bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
		bodyDef.angle = transform.Rotation.z;

		//根据定义,生成一个b2Body
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