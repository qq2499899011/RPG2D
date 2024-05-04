#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class Bullet:public ScriptableEntity
{
public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
private:
	void movement(Timestep ts);
	float bulletSpeed = 3.0f;
	int bulletDir = 1;
};

