#include "RPG2Dpch.h"
#include "PhysicsSystem.h"
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
		m_Contact = new ContactListener();
		m_PixelPerMeter = 50.0f;
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}
	//�л�����ʱ����ȡ����ָ�롣

	b2World* PhysicsSystem::Create()
	{
		//�����������ٶȷ���
		m_PhysicsWorld = new b2World({ 0.0f, 0.0f });
		//���bodyToDestroy
		m_BodiesToDestroy.clear();
		//������ײ���
		m_PhysicsWorld->SetContactListener(m_Contact);
		return m_PhysicsWorld;
	}

	void PhysicsSystem::WhenActiveScene()
	{
		//�����������ٶȷ���
		m_PhysicsWorld = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->GetPhysicsWorld();
		//���bodyToDestroy
		m_BodiesToDestroy.clear();
		//������ײ���
		m_PhysicsWorld->SetContactListener(m_Contact);
	}

	//��������ʱ����
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
	}
	void PhysicsSystem::Update(Timestep ts)
	{
		//��ɾ�������еĴ�ɾ������
		//DestroyBodies();
		//�����ٶ�
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		{
			auto view = m_Registry->view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				//�½�ʵ��
				Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get() };
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
				//��ȡ�ٶ�
				glm::vec2 vec = GetVelocity(entity);
				//�����ٶ�
				vec.x += rb2d.Acceleration.x * ts;
				vec.y += rb2d.Acceleration.y * ts;
				//д���ٶ�
				SetVelocity(entity, vec);
			}
		}
		//����ģ�������
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
		{
			auto view = m_Registry->view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				//�½�ʵ��
				Entity entity = { e, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get() };
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
		bodyDef.position.Set(transform.Translation.x / m_PixelPerMeter, transform.Translation.y / m_PixelPerMeter);
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
			boxShape.SetAsBox(bc2d.Size.x / (m_PixelPerMeter * 2), bc2d.Size.y / (2 * m_PixelPerMeter), b2Vec2(bc2d.Offset.x / m_PixelPerMeter, bc2d.Offset.y / m_PixelPerMeter), 0.0f);
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
	//Ĭ���Ѿ���������������,������ײ��Ϣ
	void PhysicsSystem::ResetBoxCollider(Entity entity)
	{
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			//��ȡbody
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
			//��ȡfixture
			b2Fixture* fixture = body->GetFixtureList();
			//���پɵ�fixture
			body->DestroyFixture(fixture);
			//�½�fixture
			b2PolygonShape boxShape;
			boxShape.SetAsBox(bc2d.Size.x / (m_PixelPerMeter * 2), bc2d.Size.y / (2 * m_PixelPerMeter), b2Vec2(bc2d.Offset.x / m_PixelPerMeter, bc2d.Offset.y / m_PixelPerMeter), 0.0f);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.isSensor = bc2d.isSensor;
			fixtureDef.density = bc2d.Density;
			fixtureDef.friction = bc2d.Friction;
			fixtureDef.restitution = bc2d.Restitution;
			fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
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
		return glm::vec2(body->GetPosition().x * m_PixelPerMeter, body->GetPosition().y * m_PixelPerMeter);
	}
	void PhysicsSystem::SetPositionWithPixel(Entity entity, glm::vec2 pos)
	{
		//��ȡ����
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rgb2d.RuntimeBody);
		b2Vec2 newPos;
		newPos.x = pos.x / m_PixelPerMeter;
		newPos.y = pos.y / m_PixelPerMeter;
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
	void PhysicsSystem::SetVelocity(Entity entity, float velocity, float angle)
	{
		// ���Ƕ�ת��Ϊ����
		float angleInRadians = angle * b2_pi / 180.0f;
		// �����ٶȵ�x��y����
		float velocityX = velocity * cos(angleInRadians);
		float velocityY = velocity * sin(angleInRadians);
		SetVelocity(entity,glm::vec2(velocityX,velocityY));
	}
	void PhysicsSystem::SetAcceleration(Entity entity, glm::vec2 acceleration)
	{
		//��ȡ����
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		//���ü��ٶ�
		rgb2d.Acceleration = acceleration;
	}
	void PhysicsSystem::CalculateCollisionBox(TransformComponent trans, glm::vec2 textureSize, BoxCollider2DComponent& box2d)
	{
		//�ж�ê��λ��
		float width = trans.Scale.x * textureSize.x;
		float height = trans.Scale.y * textureSize.y;
		box2d.Size = glm::vec2(width, height);
		//0-1 ת����Ϊ1->-1
		glm::vec2 off = glm::vec2(1.0f) - glm::vec2(trans.anchor.x*2,trans.anchor.y*2);
		box2d.Offset = glm::vec2(off.x * width / 2,off.y * height / 2);
	}
	//ReSetEntity
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
	glm::vec2 PhysicsSystem::GetAcceleration(Entity entity)
	{		//��ȡ����
		Rigidbody2DComponent& rgb2d = entity.GetComponent<Rigidbody2DComponent>();
		//���ü��ٶ�
		return rgb2d.Acceleration;
	}
	void PhysicsSystem::DestroyBodies()
	{
		for (b2Body* body : m_BodiesToDestroy) {
			m_PhysicsWorld->DestroyBody(body);
		}
		m_BodiesToDestroy.clear();
	}
	void PhysicsSystem::DestroyWorld(b2World* world)
	{
		if (!world)return;
		//��������
		// ֹͣģ��
		world->Step(0, 0, 0);

		// ������������
		b2Body* body = world->GetBodyList();
		while (body) {
			b2Body* nextBody = body->GetNext();
			world->DestroyBody(body);
			body = nextBody;
		}

		// �������йؽ�
		b2Joint* joint = world->GetJointList();
		while (joint) {
			b2Joint* nextJoint = joint->GetNext();
			world->DestroyJoint(joint);
			joint = nextJoint;
		}
		// �ͷ�����������ڴ�
		delete world;
	}
}