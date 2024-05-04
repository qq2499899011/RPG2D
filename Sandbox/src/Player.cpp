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
	//移动
	movement(ts);
	//开火
}

void Player::SetAmmo(Entity entity)
{
	ammo = entity;
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

void Player::fire()
{
	if (InputSystem::IsKeyPressed(Key::J))
	{
		//复制子弹
		Entity bullet = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->DuplicateEntity(ammo);
		//设置子弹初始位置
		
		//设置子弹朝向
	}
}

