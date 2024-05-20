#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class Ground :public ScriptableEntity
{
	//��д��������
public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
	static Entity Assemble();
private:
	float playerSpeed = 500.0f;
	const float firingRate = 3.0f;
	float CD = 0;//�����ȴʱ��
	bool canFire = false;
	bool dirLeft = true;
	glm::vec2 m_Velocity = glm::vec2(8.0f, 0);
	Entity ammo;
};

