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
	//创建animatior
	//跑步
	Ref<Animatior> playerAnimatiorRun = CreateRef<Animatior>();
	playerAnimatiorRun->AddFrame(0.0f, assetManager->GetTexture("player_run1"));
	playerAnimatiorRun->AddFrame(0.1f, assetManager->GetTexture("player_run2"));
	playerAnimatiorRun->AddFrame(0.2f, assetManager->GetTexture("player_run3"));
	//静止站立
	Ref<Animatior> playerAnimatiorIdle = CreateRef<Animatior>();
	playerAnimatiorIdle->AddFrame(0.0f, assetManager->GetTexture("player_run3"));
	//死亡
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
	//持枪
	//创建Controller
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
	//创建动画组件
	AnimatiorControllerComponent playerACcomponent;
	playerACcomponent.animatiorController = playerAC;
	scene->AddComponentWithName<AnimatiorControllerComponent>("Player", playerACcomponent);
	//新建sprite组件
	SpriteRendererComponent playerSprite;
	playerSprite.Texture = assetManager->GetTexture("player_run3");
	//增加sprite组件
	scene->AddComponentWithName<SpriteRendererComponent>("Player", playerSprite);
	//设置transform组件
	TransformComponent playerTrans;
	playerTrans.Translation = glm::vec3{ 40,0,0.3f };
	playerTrans.Scale = glm::vec3{3,3,1};
	playerTrans.anchor = glm::vec2{0,1};
	scene->GetComponentWithName<TransformComponent>("Player") = playerTrans;
	//脚本组件
	NativeScriptComponent playerScript;
	playerScript.Bind<Player>();
	scene->AddComponentWithName<NativeScriptComponent>("Player", playerScript);
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rgb2d.Acceleration.y = -19.6f;//设置加速度
	scene->AddComponentWithName<Rigidbody2DComponent>("Player", rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	physics->CalculateCollisionBox(playerTrans,glm::vec2(playerSprite.Texture->GetWidth(),playerSprite.Texture->GetHeight()),box2d);
	scene->AddComponentWithName<BoxCollider2DComponent>("Player", box2d);
	//将物体加入到物理引擎中。
	physics->AddEntity(player);
	//设置相机跟随玩家
	renderer->LookAtX(player);
	return player;
}

void Player::OnDestroy()
{

}

void Player::OnUpdate(Timestep ts)
{
	//根据输入获取状态
	state_update();
	//移动
	movement(ts);
	//开火
	fire(ts);
}

bool Player::OnCollisionBegin(Entity other)
{
	if (other.GetName().compare("Ground") == 0) {
		//如果正在坠落，并且加速度等于0
		Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
		glm::vec2 vec = physics->GetVelocity(m_Entity);
		glm::vec2 acc = physics->GetAcceleration(m_Entity);
		if (isFallen && (acc.y == 0)) {
			//刚好下落，设置加速度
			physics->SetAcceleration(m_Entity, glm::vec2(0, m_AccY));
			RPG2D_INFO("fallen");
		}
		else {
			//碰撞时调用
			float playerY = m_Entity.GetComponent<TransformComponent>().Translation.y;
			float groundY = other.GetComponent<TransformComponent>().Translation.y;
			//获取二者的y值
			RPG2D_INFO("player.y={0},ground.y={1}",playerY,groundY);
			if (playerY < groundY+10.0f) {
				groundID = other.GetUID();
				//相当于站在了地面上，所以站立地面++,而且不能是idle
				isFallen = false;
				canJump = true;	
				//说明在上方。此时设置加速度为0，并且竖直方向速度为0
				physics->SetVelocity(m_Entity, glm::vec2(vec.x, 0));
				physics->SetAcceleration(m_Entity, glm::vec2(0, 0));
			}
		}
	}
	return true;
}

bool Player::OnCollisionEnd(Entity other)
{
	//这里会出现问题
	if (other.GetName().compare("Ground") == 0) {
		//离开了地面，施加正常加速度
		Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
		//如果当前碰到的和之前记录的是一个平台，会触发掉落.
		if (groundID == other.GetUID()) {
			if (state == "run" || state_old == "downrun" || state == "uprun") {
				//以这种姿态离开时，需要设置加速度
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

//更新玩家状态并设置动画
void Player::state_update()
{
	bool keyPressedA = InputSystem::IsKeyPressed(Key::A);
	bool keyPressedD = InputSystem::IsKeyPressed(Key::D);
	bool keyPressedW = InputSystem::IsKeyPressed(Key::W);
	bool keyPressedS = InputSystem::IsKeyPressed(Key::S);
	bool keyPressedK = InputSystem::IsKeyPressed(Key::K);
	state_old = state;
	state = "idle";
	//判断左右
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
		//斜上方
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
	//如果在地面，然后同时按下了S和K，之恶
	//设置动画
	m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController->SetState(state);
	if (state != state_old) {
		if (state == "jump" ||state_old== "jump" || state== "down" ||state_old == "down" ) {
			//如果切换到jump或者down 或者从jump或者down切换回去，那么重新设置box
			//获取当前材质，重新计算box2d
			Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
			Ref<AnimatiorController> ac = m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController;
			glm::vec2 size = ac->GetCurFrame()->GetSize();
			physics->CalculateCollisionBox(m_Entity.GetComponent<TransformComponent>(),size,m_Entity.GetComponent<BoxCollider2DComponent>());
			physics->ResetBoxCollider(m_Entity);
			//输出变换情况
			RPG2D_INFO("change state{0} to {1}",state_old,state);
		}
	}
}

void Player::movement(Timestep ts)
{
	//计算物体在竖直方向的速度。
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	glm::vec2 vec = physics->GetVelocity(m_Entity);
	glm::vec2 pos =physics->GetPixelPosition(m_Entity);
	bool keyPressedA = InputSystem::IsKeyPressed(Key::A);
	bool keyPressedD = InputSystem::IsKeyPressed(Key::D);
	bool keyPressedW = InputSystem::IsKeyPressed(Key::W);
	bool keyPressedS = InputSystem::IsKeyPressed(Key::S);
	bool keyPressedK = InputSystem::IsKeyPressed(Key::K);
	//获取速度和位置
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
			//每次发射子弹之前都计算朝向的角度
			get_fire_angle();
			if (fireMode == FireMode::ShotGun) {
				//霰弹枪
				fireShotGun();
				if (--fireCount == 0)fireMode == FireMode::SemiAutomatic;
			}
			else if (fireMode == FireMode::SemiAutomatic) {
				//半自动
				fireBullet();
			}
			else if (fireMode == FireMode::Automatic) {
				//全自动，设置自动开火效率
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
	//三连发
	//如果可以发射才发射,计算一个子弹数量
	if (AutoCD < 1 / fireRateAuto)AutoCD += ts;
	else {
		//可以发射子弹
		autoCount--;
		fireBullet();
		AutoCD -= 1 / fireRateAuto;
	}
}
//散射
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
//根据角色状态确定射击角度
void Player::get_fire_angle()
{
	int cal = 1;
	//判断左右朝向
	if (dirLeft) {
		angle = 180.0f;
		cal = -1;
	}
	else {
		angle = 0.0f;
	}
	if (state == "uprun") {
		//举枪射击
		angle += (cal * 45.0f);
	}
	else if (state == "downrun") {
		//放下射击
		angle -= (cal * 45.0f);
	}
	else if (state == "up") {
		//向上射击
		angle = 90.0f;
	}
}

