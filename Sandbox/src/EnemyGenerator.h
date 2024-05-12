#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class EnemyGenerator :public ScriptableEntity
{
	//��д��������
public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
	static Entity Assemble();
private:
	void GenerateEnemy(Timestep ts);
	const float enemyRate = 3.0f;
};

