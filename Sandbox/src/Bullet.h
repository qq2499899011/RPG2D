#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class Bullet:public ScriptableEntity
{
public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
	void SetVelocity(glm::vec2);
	glm::vec2 GetVelocity() { return m_Velocity; };
	static Entity Assemble();
private:
	float lifeSpan = 10.0f;//�ӵ�����ʱ�䣬������ʱ���Զ�����
	glm::vec2 m_Velocity = glm::vec2(20.0f,0.0f);
	int bulletDir = 1;
};

