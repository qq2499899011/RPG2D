#include "Player.h"
#include "Bullet.h"
void Player::OnCreate()
{
}

void Player::OnDestroy()
{
}

void Player::OnUpdate(Timestep ts)
{
	//�ƶ�
	movement(ts);
	//����
	fire(ts);
}

void Player::OnCollisionBegin(Entity other)
{
	//����͵��淢����ײ���ſ��Լ�����Լ

}

void Player::SetAmmo(Entity entity)
{
	ammo = entity;
}

Entity Player::Assemble()
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	//
	Entity player = scene->CreateEntity("Player");
	//����animatior
	Ref<Animatior> playerAnimatiorRun = CreateRef<Animatior>();
	playerAnimatiorRun->AddFrame(0.0f, assetManager->GetTexture("player1"));
	playerAnimatiorRun->AddFrame(0.1f, assetManager->GetTexture("player2"));
	playerAnimatiorRun->AddFrame(0.2f, assetManager->GetTexture("player3"));
	Ref<Animatior> playerAnimatiorIdle = CreateRef<Animatior>();
	playerAnimatiorIdle->AddFrame(0.0f, assetManager->GetTexture("player1"));
	//����Controller
	Ref<AnimatiorController> playerAC = CreateRef<AnimatiorController>();
	playerAC->AddAnimation(AnimationState::Running, playerAnimatiorRun);
	playerAC->AddAnimation(AnimationState::Idle, playerAnimatiorIdle);
	playerAC->SetState(AnimationState::Idle);
	//�����������
	AnimatiorControllerComponent playerACcomponent;
	playerACcomponent.animatiorController = playerAC;
	scene->AddComponentWithName<AnimatiorControllerComponent>("Player", playerACcomponent);
	//�½�sprite���
	SpriteRendererComponent playerSprite;
	playerSprite.Texture = assetManager->GetTexture("player1");
	//����sprite���
	scene->AddComponentWithName<SpriteRendererComponent>("Player", playerSprite);
	//����transform���
	TransformComponent playerTrans;
	playerTrans.Translation = glm::vec3{ 0,0,0.3f };
	//��С��scale*���� = ʵ�ʴ�С��
	//scale = ʵ�ʴ�С/����
	//playerTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
	playerTrans.Scale = glm::vec3{1,1,1};
	scene->GetComponentWithName<TransformComponent>("Player") = playerTrans;
	//�ű����
	NativeScriptComponent playerScript;
	playerScript.Bind<Player>();
	scene->AddComponentWithName<NativeScriptComponent>("Player", playerScript);
	//�������
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	scene->AddComponentWithName<Rigidbody2DComponent>("Player", rgb2d);
	//��ײ��
	BoxCollider2DComponent box2d;
	//����size��offst��
	//��ȡ����ʵ�ʴ�С
	float width = playerTrans.Scale.x * playerSprite.Texture->GetWidth();
	float height = playerTrans.Scale.y * playerSprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//��Ϊbox2d y�����ϣ�opengl y�����£�offset.yΪ��ֵ
	box2d.Offset = glm::vec2(width / 2, height / 2);
	scene->AddComponentWithName<BoxCollider2DComponent>("Player", box2d);
	//��������뵽���������С�
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(player);
	return player;
}

void Player::movement(Timestep ts)
{
	//��ȡ��������
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	//��ӡ�����ɫ�ٶ�
	glm::vec2 vec = physics->GetVelocity(m_Entity);
	//RPG2D_INFO("playerSpeed:{0},{1}", vec.x, vec.y);
	//��ȡλ��
	glm::vec2 pos =physics->GetPixelPosition(m_Entity);
	//�������
	//�����ƶ����޸��ƶ��ٶ�
	if (InputSystem::IsKeyPressed(Key::A))
	{
		dirLeft = true;
		//����״̬
		m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController->SetState(AnimationState::Running);
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = !dirLeft;
		//���ٶȶ������޸�λ��
		//����ˮƽ�ٶ�
		physics->SetVelocity(m_Entity,glm::vec2(-m_VecX,vec.y));
	}
	else if (InputSystem::IsKeyPressed(Key::D))
	{
		dirLeft = false;
		m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController->SetState(AnimationState::Running);
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = !dirLeft;
		//����ˮƽ�ٶ�
		physics->SetVelocity(m_Entity, glm::vec2(m_VecX, vec.y));
	}
	else {
		//�����ٶ�Ϊ0
		m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController->SetState(AnimationState::Idle);
		//��ȡ��ǰ�ٶ�
		glm::vec2 vec = physics->GetVelocity(m_Entity);
		physics->SetVelocity(m_Entity,glm::vec2(0,vec.y));
	}

	if (InputSystem::IsKeyPressed(Key::K)&&canJump) {
		canJump = false;
		//����K������Ծ
		//����ɫһ��������Ծ���ٶ�
		//���ô�ֱ�ٶ�
		physics->SetVelocity(m_Entity, glm::vec2(vec.x, m_VecX));
	}
}

void Player::fire(Timestep ts)
{
	if (CD < 1 / firingRate)CD+=ts;
	else {
		if (InputSystem::IsKeyPressed(Key::J))
		{
			//�����ӵ�
			Entity bullet = Bullet::Assemble();
			//�����ӵ���ʼλ��;
			//��ȡ��ɫ��ǰλ��
			TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
			GlobalContext::GetInstance()->m_PhysicsSystem->SetPositionWithPixel(bullet,glm::vec2(trans.Translation.x,trans.Translation.y));
			//�����ӵ�����
			Bullet* bulletScript = dynamic_cast<Bullet*>(bullet.GetComponent<NativeScriptComponent>().Instance);
			if (dirLeft) {
				bulletScript->SetVelocity(glm::vec2(-bulletScript->GetVelocity().x,bulletScript->GetVelocity().y));
			}
			else {
				bulletScript->SetVelocity(glm::vec2(bulletScript->GetVelocity().x,bulletScript->GetVelocity().y));
			}
			//�����ӵ�����
			CD -= 1 / firingRate;
		}
	}
}

