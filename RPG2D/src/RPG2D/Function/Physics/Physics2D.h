#pragma once

#include "RPG2D/Resource/Scene/Components.h"
#include "RPG2D/Core/Base.h"
#include "RPG2D/Core/Timestep.h"
#include "RPG2D/Resource/Scene/Scene.h"
// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
//将box2d中的body类型与组件中定义的body类型进行相互转换
namespace RPG2D {
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();
		//场景
		void Init();
		void Update(Timestep ts);

	private:
		//迭代精度
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		//物理世界
		b2World* m_PhysicsWorld = nullptr;
	};
	namespace Utils {

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

}
