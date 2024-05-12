#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class Player:public ScriptableEntity
{
	//重写各个函数
public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
	void SetAmmo(Entity entity);
	static Entity Assemble();
private:
	void movement(Timestep ts);
	void fire(Timestep);
	float playerSpeed = 500.0f;
	const float firingRate = 3.0f;
	float CD = 0;//射击冷却时间
	bool canFire = false;
	bool dirLeft = true;
	glm::vec2 m_Velocity = glm::vec2(8.0f, 0);
	Entity ammo;
};

