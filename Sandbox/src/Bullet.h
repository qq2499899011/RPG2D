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
	static Entity Assemble(bool isPlayer = true);
	bool isPlayer() { return m_isPlayer; }
	void SetPlayer(bool is) { m_isPlayer = is; }
private:
	float lifeSpan = 10.0f;//子弹存在时间，超出此时间自动销毁
	float lifeTime = 0.0f;
	glm::vec2 m_Velocity = glm::vec2(20.0f,0.0f);
	int bulletDir = 1;
	bool m_isPlayer = false;
};

