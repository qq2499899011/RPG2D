#include "Bullet.h"

void Bullet::OnCreate()
{
}

void Bullet::OnDestroy()
{
}

void Bullet::OnUpdate(Timestep ts)
{
	
}

void Bullet::movement(Timestep ts)
{
	//获取transform组件
	TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
	trans.Translation.x += ts * bulletDir * bulletSpeed;
}
