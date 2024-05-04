#include "Player.h"
void Player::OnCreate()
{
	//
}

void Player::OnDestroy()
{
	
}

void Player::OnUpdate(Timestep ts)
{
	//����
	movement(ts);
}

void Player::movement(Timestep ts)
{
	//��ȡtransform���
	TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
	//�������
	if (InputSystem::IsKeyPressed(Key::A))
	{
		trans.Translation.x += -1 * playerSpeed;
	}
	else if (InputSystem::IsKeyPressed(Key::D))
	{
		trans.Translation.x += 1 * playerSpeed;
	}
}

