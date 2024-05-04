#pragma once
class b2World;
namespace RPG2D {
	class Entity;
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();
		//每次加载场景后调用物理场景的调用函数。
		void Init();
		void Update(Timestep ts);
		void AddEntity(Entity);
	private:
		//迭代精度
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		//物理世界
		b2World* m_PhysicsWorld = nullptr;
	};

}
