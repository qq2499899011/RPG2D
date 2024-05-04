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
	//�ƶ�
	movement(ts);
	//����
}

void Player::SetAmmo(Entity entity)
{
	ammo = entity;
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

void Player::fire()
{
	if (InputSystem::IsKeyPressed(Key::J))
	{
		//�����ӵ�
		Entity bullet = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->DuplicateEntity(ammo);
		//�����ӵ���ʼλ��
		
		//�����ӵ�����
	}
}

