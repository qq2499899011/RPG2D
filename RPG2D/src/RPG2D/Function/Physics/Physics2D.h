#pragma once
#include <vector>
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
		void Init();
		void Update(Timestep ts);
		void AddEntity(Entity);
		void RemoveEntity(Entity);
		glm::vec2 GetPixelPosition(Entity entity);
		void SetPositionWithPixel(Entity,glm::vec2);
		void SetVelocity(Entity,glm::vec2);
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
