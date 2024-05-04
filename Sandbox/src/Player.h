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
private:
	void movement(Timestep ts);
	void fire();
	float playerSpeed = 3.0f;
	Entity ammo;
};

