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
	glm::vec2 m_Velocity = glm::vec2(5.0f, 0);
};

