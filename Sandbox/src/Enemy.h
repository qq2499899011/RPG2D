#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class Enemy :public ScriptableEntity
{
	//重写各个函数
public:
	virtual void OnStart()override;
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual bool OnCollisionBegin(Entity other)override;
	void SetVelocity(glm::vec2);
	static Entity Assemble();
	static Entity AssembleFire();
	void SetIsFire(bool is) { isFire = is; }
	bool IsPlayerLeft();
private:
	bool isFire = false;
	void fire(Timestep ts,bool);
	const float firingRate =0.3f;
	float reactTime = 0.5;
	float countTime = 0.0f;
	float CD = 0;//射击冷却时间
	bool dirLeft = true;//是否朝左边
	bool canFire = false;
	glm::vec2 m_Velocity = glm::vec2(5.0f, 0);
};

