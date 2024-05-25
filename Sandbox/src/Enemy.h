#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class Enemy :public ScriptableEntity
{
	//重写各个函数
public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnCollisionBegin(Entity other)override;
	void SetVelocity(glm::vec2);
	static Entity Assemble();
private:
	void fire(Timestep);
	const float firingRate =1.0f;
	float CD = 0;//射击冷却时间
	bool canFire = false;
	glm::vec2 m_Velocity = glm::vec2(5.0f, 0);
};

