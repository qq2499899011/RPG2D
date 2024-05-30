#include "Player.h"
#include "Bullet.h"
void Player::OnCreate()
{

}

Entity Player::Assemble()
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	Ref<RendererManager> renderer = GlobalContext::GetInstance()->m_RendererManager;
	//
	Entity player = scene->CreateEntity("Player");
	//����animatior
	//�ܲ�
	Ref<Animatior> playerAnimatiorRun = CreateRef<Animatior>();
	playerAnimatiorRun->AddFrame(0.0f, assetManager->GetTexture("player_run1"));
	playerAnimatiorRun->AddFrame(0.1f, assetManager->GetTexture("player_run2"));
	playerAnimatiorRun->AddFrame(0.2f, assetManager->GetTexture("player_run3"));
	//��ֹվ��
	Ref<Animatior> playerAnimatiorIdle = CreateRef<Animatior>();
	playerAnimatiorIdle->AddFrame(0.0f, assetManager->GetTexture("player_run3"));
	//����
	Ref<Animatior> playerAnimatiorDead = CreateRef<Animatior>();
	playerAnimatiorDead->AddFrame(0.0f, assetManager->GetTexture("player_dead1"));
	playerAnimatiorDead->AddFrame(0.1f, assetManager->GetTexture("player_dead2"));
	playerAnimatiorDead->AddFrame(0.2f, assetManager->GetTexture("player_dead3"));
	//uprun
	Ref<Animatior> playerAnimatiorUpRun = CreateRef<Animatior>();
	playerAnimatiorUpRun->AddFrame(0.0f, assetManager->GetTexture("player_uprun1"));
	playerAnimatiorUpRun->AddFrame(0.1f, assetManager->GetTexture("player_uprun2"));
	playerAnimatiorUpRun->AddFrame(0.2f, assetManager->GetTexture("player_uprun3"));
	//downrun
	Ref<Animatior> playerAnimatiorDownRun = CreateRef<Animatior>();
	playerAnimatiorDownRun->AddFrame(0.0f, assetManager->GetTexture("player_downrun1"));
	playerAnimatiorDownRun->AddFrame(0.1f, assetManager->GetTexture("player_downrun2"));
	playerAnimatiorDownRun->AddFrame(0.2f, assetManager->GetTexture("player_downrun3"));
	//jump
	Ref<Animatior> playerAnimatiorJump = CreateRef<Animatior>();
	playerAnimatiorJump->AddFrame(0.0f, assetManager->GetTexture("player_jump1"));
	playerAnimatiorJump->AddFrame(0.1f, assetManager->GetTexture("player_jump2"));
	//up
	Ref<Animatior> playerAnimatiorUp = CreateRef<Animatior>();
	playerAnimatiorUp->AddFrame(0.0f, assetManager->GetTexture("player_up1"));
	//down
	Ref<Animatior> playerAnimatiorDown = CreateRef<Animatior>();
	playerAnimatiorDown->AddFrame(0.0f, assetManager->GetTexture("player_down1"));
	//��ǹ
	//����Controller
	Ref<AnimatiorController> playerAC = CreateRef<AnimatiorController>();
	playerAC->AddAnimation("run", playerAnimatiorRun);
	playerAC->AddAnimation("idle", playerAnimatiorIdle);
	playerAC->AddAnimation("dead", playerAnimatiorDead);
	playerAC->AddAnimation("uprun", playerAnimatiorUpRun);
	playerAC->AddAnimation("downrun", playerAnimatiorDownRun);
	playerAC->AddAnimation("jump", playerAnimatiorJump);
	playerAC->AddAnimation("down", playerAnimatiorDown);
	playerAC->AddAnimation("up", playerAnimatiorUp);
	playerAC->SetState("idle");
	//�����������
	AnimatiorControllerComponent playerACcomponent;
	playerACcomponent.animatiorController = playerAC;
	scene->AddComponentWithName<AnimatiorControllerComponent>("Player", playerACcomponent);
	//�½�sprite���
	SpriteRendererComponent playerSprite;
	playerSprite.Texture = assetManager->GetTexture("player_run3");
	//����sprite���
	scene->AddComponentWithName<SpriteRendererComponent>("Player", playerSprite);
	//����transform���
	TransformComponent playerTrans;
	playerTrans.Translation = glm::vec3{ 40,0,0.3f };
	playerTrans.Scale = glm::vec3{3,3,1};
	playerTrans.anchor = glm::vec2{0,1};
	scene->GetComponentWithName<TransformComponent>("Player") = playerTrans;
	//�ű����
	NativeScriptComponent playerScript;
	playerScript.Bind<Player>();
	scene->AddComponentWithName<NativeScriptComponent>("Player", playerScript);
	//�������
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rgb2d.Acceleration.y = -19.6f;//���ü��ٶ�
	scene->AddComponentWithName<Rigidbody2DComponent>("Player", rgb2d);
	//��ײ��
	BoxCollider2DComponent box2d;
	physics->CalculateCollisionBox(playerTrans,glm::vec2(playerSprite.Texture->GetWidth(),playerSprite.Texture->GetHeight()),box2d);
	scene->AddComponentWithName<BoxCollider2DComponent>("Player", box2d);
	//��������뵽���������С�
	physics->AddEntity(player);
	//��������������
	renderer->LookAtX(player);
	return player;
}

void Player::OnDestroy()
{

}

void Player::OnUpdate(Timestep ts)
{
	//���������ȡ״̬
	state_update();
	//�ƶ�
	movement(ts);
	//����
	fire(ts);
}

bool Player::OnCollisionBegin(Entity other)
{
	if (other.GetName().compare("Ground") == 0) {
		//�������׹�䣬���Ҽ��ٶȵ���0
		Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
		glm::vec2 vec = physics->GetVelocity(m_Entity);
		glm::vec2 acc = physics->GetAcceleration(m_Entity);
		if (isFallen && (acc.y == 0)) {
			//�պ����䣬���ü��ٶ�
			physics->SetAcceleration(m_Entity, glm::vec2(0, m_AccY));
			RPG2D_INFO("fallen");
		}
		else {
			//��ײʱ����
			float playerY = m_Entity.GetComponent<TransformComponent>().Translation.y;
			float groundY = other.GetComponent<TransformComponent>().Translation.y;
			//��ȡ���ߵ�yֵ
			RPG2D_INFO("player.y={0},ground.y={1}",playerY,groundY);
			if (playerY < groundY+10.0f) {
				groundID = other.GetUID();
				//�൱��վ���˵����ϣ�����վ������++,���Ҳ�����idle
				isFallen = false;
				canJump = true;	
				//˵�����Ϸ�����ʱ���ü��ٶ�Ϊ0��������ֱ�����ٶ�Ϊ0
				physics->SetVelocity(m_Entity, glm::vec2(vec.x, 0));
				physics->SetAcceleration(m_Entity, glm::vec2(0, 0));
			}
		}
	}
	return true;
}

bool Player::OnCollisionEnd(Entity other)
{
	//������������
	if (other.GetName().compare("Ground") == 0) {
		//�뿪�˵��棬ʩ���������ٶ�
		Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
		//�����ǰ�����ĺ�֮ǰ��¼����һ��ƽ̨���ᴥ������.
		if (groundID == other.GetUID()) {
			if (state == "run" || state_old == "downrun" || state == "uprun") {
				//��������̬�뿪ʱ����Ҫ���ü��ٶ�
				physics->SetAcceleration(m_Entity, glm::vec2(0, m_AccY));
			}
		}
		//isFallen = true;
		//physics->SetAcceleration(m_Entity, glm::vec2(0, m_AccY));
	}
	return true;
}

void Player::SwitchFireMode(FireMode fire)
{
	if (fire == FireMode::Automatic) {
		fireCount = 30;
	}
	else if (fire == FireMode::ShotGun) {
		fireCount = 10;
	}
	fireMode = fire;
}


void Player::SetAmmo(Entity entity)
{
	ammo = entity;
}

//�������״̬�����ö���
void Player::state_update()
{
	bool keyPressedA = InputSystem::IsKeyPressed(Key::A);
	bool keyPressedD = InputSystem::IsKeyPressed(Key::D);
	bool keyPressedW = InputSystem::IsKeyPressed(Key::W);
	bool keyPressedS = InputSystem::IsKeyPressed(Key::S);
	bool keyPressedK = InputSystem::IsKeyPressed(Key::K);
	state_old = state;
	state = "idle";
	//�ж�����
	if(keyPressedA)
	{
		dirLeft = true;
		state = "run";
	}
	else if(keyPressedD)
	{
		dirLeft = false;
		state = "run";
	}
	if((keyPressedA || keyPressedD) && keyPressedW) {
		//б�Ϸ�
		state = "uprun";
	}
	else if((keyPressedA || keyPressedD) && keyPressedS) {
		state = "downrun";
	}
	else if(keyPressedW) {
		state = "up";
	}
	else if(keyPressedS) {
		state = "down";
	}
	if(!canJump) {
		state = "jump";
	}
	//����ڵ��棬Ȼ��ͬʱ������S��K��֮��
	//���ö���
	m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController->SetState(state);
	if (state != state_old) {
		if (state == "jump" ||state_old== "jump" || state== "down" ||state_old == "down" ) {
			//����л���jump����down ���ߴ�jump����down�л���ȥ����ô��������box
			//��ȡ��ǰ���ʣ����¼���box2d
			Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
			Ref<AnimatiorController> ac = m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController;
			glm::vec2 size = ac->GetCurFrame()->GetSize();
			physics->CalculateCollisionBox(m_Entity.GetComponent<TransformComponent>(),size,m_Entity.GetComponent<BoxCollider2DComponent>());
			physics->ResetBoxCollider(m_Entity);
			//����任���
			RPG2D_INFO("change state{0} to {1}",state_old,state);
		}
	}
}

void Player::movement(Timestep ts)
{
	//������������ֱ������ٶȡ�
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	glm::vec2 vec = physics->GetVelocity(m_Entity);
	glm::vec2 pos =physics->GetPixelPosition(m_Entity);
	bool keyPressedA = InputSystem::IsKeyPressed(Key::A);
	bool keyPressedD = InputSystem::IsKeyPressed(Key::D);
	bool keyPressedW = InputSystem::IsKeyPressed(Key::W);
	bool keyPressedS = InputSystem::IsKeyPressed(Key::S);
	bool keyPressedK = InputSystem::IsKeyPressed(Key::K);
	//��ȡ�ٶȺ�λ��
	//RPG2D_INFO("player's vec={0},{1} pos = {2},{3}", vec.x, vec.y, pos.x, pos.y);
	if (keyPressedA)
	{
		dirLeft = true;
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = dirLeft;
		physics->SetVelocity(m_Entity,glm::vec2(-m_VecX,vec.y));
	}
	else if (keyPressedD)
	{
		dirLeft = false;
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = dirLeft;
		physics->SetVelocity(m_Entity, glm::vec2(m_VecX, vec.y));
	}
	else {
		glm::vec2 vec = physics->GetVelocity(m_Entity);
		physics->SetVelocity(m_Entity,glm::vec2(0,vec.y));
	}
	if (keyPressedK && canJump) {
		canJump = false;
		if (keyPressedS) {
			isFallen = true;
			//physics->SetAcceleration(m_Entity, glm::vec2(0, m_AccY));
		}
		else {
			physics->SetVelocity(m_Entity, glm::vec2(vec.x, m_VecY));
			physics->SetAcceleration(m_Entity, glm::vec2(0, m_AccY));
		}
	}
}

void Player::fire(Timestep ts)
{
	if (CD < 1 / firingRate)CD+=ts;
	else {
		if (InputSystem::IsKeyPressed(Key::J))
		{
			//ÿ�η����ӵ�֮ǰ�����㳯��ĽǶ�
			get_fire_angle();
			if (fireMode == FireMode::ShotGun) {
				//����ǹ
				fireShotGun();
				if (--fireCount == 0)fireMode == FireMode::SemiAutomatic;
			}
			else if (fireMode == FireMode::SemiAutomatic) {
				//���Զ�
				fireBullet();
			}
			else if (fireMode == FireMode::Automatic) {
				//ȫ�Զ��������Զ�����Ч��
				autoCount = 3;
				if (--fireCount == 0)fireMode == FireMode::SemiAutomatic;
			}
			CD -= 1 / firingRate;
		}
	}
	if(autoCount>0)fireAutomatic(ts);
}
void Player::fireAutomatic(Timestep ts)
{
	//������
	//������Է���ŷ���,����һ���ӵ�����
	if (AutoCD < 1 / fireRateAuto)AutoCD += ts;
	else {
		//���Է����ӵ�
		autoCount--;
		fireBullet();
		AutoCD -= 1 / fireRateAuto;
	}
}
//ɢ��
void Player::fireShotGun()
{
	glm::vec2 pos = m_Entity.GetCenter();
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	pos.y -= 30.0f;
	Entity bullet1 = Bullet::Assemble();
	physics->SetPositionWithPixel(bullet1, pos);
	physics->SetVelocity(bullet1, 20.0f, angle);
	Entity bullet2 = Bullet::Assemble();
	physics->SetPositionWithPixel(bullet2, pos);
	physics->SetVelocity(bullet2, 20.0f, angle+15.0f);
	Entity bullet3 = Bullet::Assemble();
	physics->SetPositionWithPixel(bullet3, pos);
	physics->SetVelocity(bullet3, 20.0f, angle-15.0f);
}
void Player::fireBullet()
{		
	Entity bullet = Bullet::Assemble();
	glm::vec2 pos = m_Entity.GetCenter();
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	pos.y -= 30.0f;
	physics->SetPositionWithPixel(bullet, pos);
	physics->SetVelocity(bullet, 20.0f, angle);
}
//���ݽ�ɫ״̬ȷ������Ƕ�
void Player::get_fire_angle()
{
	int cal = 1;
	//�ж����ҳ���
	if (dirLeft) {
		angle = 180.0f;
		cal = -1;
	}
	else {
		angle = 0.0f;
	}
	if (state == "uprun") {
		//��ǹ���
		angle += (cal * 45.0f);
	}
	else if (state == "downrun") {
		//�������
		angle -= (cal * 45.0f);
	}
	else if (state == "up") {
		//�������
		angle = 90.0f;
	}
}

