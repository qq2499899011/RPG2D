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
	//��ȡtransform���
	TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
	trans.Translation.x += ts * bulletDir * bulletSpeed;
}
