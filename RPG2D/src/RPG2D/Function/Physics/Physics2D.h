#pragma once
class b2World;
namespace RPG2D {
	class Entity;
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();
		//ÿ�μ��س���������������ĵ��ú�����
		void Init();
		void Update(Timestep ts);
		void AddEntity(Entity);
	private:
		//��������
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		//��������
		b2World* m_PhysicsWorld = nullptr;
	};

}
