#pragma once
#include <vector>
#include "RPG2D/Resource/ResType/Components.h"
class b2World;
class b2Body;
namespace RPG2D {
	class Entity;
	class ContactListener;
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();
		//ÿ�μ��س���������������ĵ��ú�����
		b2World* Create();
		void WhenActiveScene();
		void Init();
		void Update(Timestep ts);
		void AddEntity(Entity);
		void ResetBoxCollider(Entity);
		void RemoveEntity(Entity);
		glm::vec2 GetPixelPosition(Entity entity);
		void SetPositionWithPixel(Entity,glm::vec2);
		void SetVelocity(Entity,glm::vec2);
		void CalculateCollisionBox(TransformComponent, glm::vec2, BoxCollider2DComponent&);
		glm::vec2 GetVelocity(Entity);
	private:
		void DestroyBodies();
		//��������
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		//��������
		b2World* m_PhysicsWorld = nullptr;
		float m_PixelPerMeter = 0.0f;
		ContactListener* m_Contact = nullptr;
		std::vector<b2Body*> m_BodiesToDestroy;
	};
}
