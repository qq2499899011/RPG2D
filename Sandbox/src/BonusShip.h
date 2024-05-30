#pragma once
#include <RPG2D.h>
#include "Player.h"
using namespace RPG2D;
class BonusShip :public ScriptableEntity
{
	//�����������͡�
public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual bool OnCollisionBegin(Entity other)override;
	virtual bool OnCollisionEnd(Entity other)override;
	void ReleaseAmo();
	void SetFireMode(FireMode);
	static Entity Assemble(FireMode);
private:
	void movement(Timestep);
	FireMode bonus = FireMode::Automatic;//Ĭ���Զ���ǹ
	const float changeTime = 1.0f;
	float timeCount = 0;//��������ʱ��
	glm::vec2 m_Velocity = glm::vec2(3.0f, 3.0f);
	bool destory = false;
};

