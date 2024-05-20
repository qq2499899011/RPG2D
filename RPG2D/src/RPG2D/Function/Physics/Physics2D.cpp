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
		m_Contact =new ContactListener();
		m_PixelPerMeter = 50.0f;
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}
	//切换场景时调用
	void PhysicsSystem::Init()
	{
		
		//设置重力加速度方向
		m_PhysicsWorld = new b2World({ 0.0f, 9.8f });
		//清空bodyToDestroy
		m_BodiesToDestroy.clear();
		//设置碰撞检测
		m_PhysicsWorld->SetContactListener(m_Contact);
		//找到所有含有刚体组件的实体
		//直接通过SceneManager获取view
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->SetPhysicsWorld(m_PhysicsWorld);
		/*
		auto view = m_Registry->view<Rigidbody2DComponent>();
		//遍历实体，对于每个刚体实体，建立其在box2d中的对应body。
		//当生成预制体之后，需要执行一次这个过程。
		for (auto e : view)
		{
			Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
			AddEntity(entity);
		}
		*/
	}
	void PhysicsSystem::Update(Timestep ts)
	{
		//先删除数组中的待删除物体
		DestroyBodies();
		//更新模拟情况。
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
		
		/*
		//获取碰撞情况
		b2Contact* contact = m_PhysicsWorld->GetContactList();
		while (contact!=nullptr)
		{
			b2Fixture* fixtureA = contact->GetFixtureA();
			b2Fixture* fixtureB = contact->GetFixtureB();

			// 获取碰撞的两个物体
			b2Body* bodyA = fixtureA->GetBody();
			b2Body* bodyB = fixtureB->GetBody();

			// 获取碰撞的两个物体的位置
			b2Vec2 positionA = bodyA->GetPosition();
			b2Vec2 positionB = bodyB->GetPosition();

			// 打印碰撞的两个物体的位置
			std::cout << "Collision occurred between objects A at (" << positionA.x << ", " << positionA.y << ")"
				<< " and B at (" << positionB.x << ", " << positionB.y << ")" << std::endl;
			contact = contact->GetNext();
			//
		}
		
		*/
		
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		// 遍历所有具有刚体组件的实体
		auto view = m_Registry->view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			if (InputSystem::IsKeyPressed(Key::K)) {
				RPG2D_CORE_INFO("good");
			}
			//新建实体
			Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
			//获取实体的组件
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			//获取b2Body
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			//根据b2Body获取物体最新位置,并输出到transform组件中
			const auto& position = body->GetPosition();
			transform.Translation.x = position.x * m_PixelPerMeter;
			transform.Translation.y = position.y * m_PixelPerMeter;
			transform.Rotation.z = body->GetAngle();
		}
	}
	//默认所有位置变量都除以10.
	void PhysicsSystem::AddEntity(Entity entity)
	{
		if (!entity.HasComponent<Rigidbody2DComponent>())return;
		//获取transform和刚体组件
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		//设置body定义：类型 位置 角度
		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
		bodyDef.position.Set(transform.Translation.x/m_PixelPerMeter, transform.Translation.y/m_PixelPerMeter);
		bodyDef.angle = transform.Rotation.z;
		bodyDef.userData.pointer = static_cast<uintptr_t>(entity.GetUID().getID());
		//根据定义,生成一个b2Body
		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.FixedRotation);
		rb2d.RuntimeBody = body;
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			//自动对相关碰撞体小大除以2，所以设置碰撞体时，直接填入实际大小即可。
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
		//删除body
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
		//加入待删除列表
		m_BodiesToDestroy.push_back(body);
	}
	//返回像素位置
	glm::vec2 PhysicsSystem::GetPixelPosition(Entity entity)
	{
		//获取刚体
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		//返回位置
		return glm::vec2(body->GetPosition().x*m_PixelPerMeter, body->GetPosition().y* m_PixelPerMeter);
	}
	void PhysicsSystem::SetPositionWithPixel(Entity entity,glm::vec2 pos)
	{
		//获取刚体
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		b2Vec2 newPos;
		newPos.x = pos.x/m_PixelPerMeter;
		newPos.y = pos.y/m_PixelPerMeter;
		body->SetTransform(newPos, 0.0f);
	}
	//这里速度和y轴方向相反，也就是向上是跳。
	void PhysicsSystem::SetVelocity(Entity entity, glm::vec2 velocity)
	{
		//获取刚体
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		//设置初始速度
		b2Vec2 v;
		v.x = velocity.x;
		v.y = -velocity.y;
		body->SetLinearVelocity(v);
	}
	//获取速度需要符合opengl的坐标轴，因此需要y取相反数
	glm::vec2 PhysicsSystem::GetVelocity(Entity entity)
	{
		//获取实体
		//获取刚体
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		//获取速度
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