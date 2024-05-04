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
	//操作
	movement(ts);
}

void Player::movement(Timestep ts)
{
	//获取transform组件
	TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
	//操作玩家
	if (InputSystem::IsKeyPressed(Key::A))
	{
		trans.Translation.x += -1 * playerSpeed;
	}
	else if (InputSystem::IsKeyPressed(Key::D))
	{
		trans.Translation.x += 1 * playerSpeed;
	}
}

