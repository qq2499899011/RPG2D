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
	playerTrans.Translation = glm::vec3{ 0,0,0.3f };
	playerTrans.Scale = glm::vec3{2,2,1};
	playerTrans.anchor = glm::vec2{0,1};
	scene->GetComponentWithName<TransformComponent>("Player") = playerTrans;
	//脚本组件
	NativeScriptComponent playerScript;
	playerScript.Bind<Player>();
	scene->AddComponentWithName<NativeScriptComponent>("Player", playerScript);
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	scene->AddComponentWithName<Rigidbody2DComponent>("Player", rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	physics->CalculateCollisionBox(playerTrans,glm::vec2(playerSprite.Texture->GetWidth(),playerSprite.Texture->GetHeight()),box2d);
	scene->AddComponentWithName<BoxCollider2DComponent>("Player", box2d);
	//将物体加入到物理引擎中。
	physics->AddEntity(player);
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

void Player::OnCollisionBegin(Entity other)
{
	//如果和地面发生碰撞，需要判断是地面
	canJump = true;	
	if (other.GetName().compare("Bullet") == 0) {
		//碰撞时调用
		RPG2D_INFO("player being hit");
	}

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
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	glm::vec2 vec = physics->GetVelocity(m_Entity);
	glm::vec2 pos =physics->GetPixelPosition(m_Entity);
	if (InputSystem::IsKeyPressed(Key::A))
	{
		dirLeft = true;
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = dirLeft;
		physics->SetVelocity(m_Entity,glm::vec2(-m_VecX,vec.y));
	}
	else if (InputSystem::IsKeyPressed(Key::D))
	{
		dirLeft = false;
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = dirLeft;
		physics->SetVelocity(m_Entity, glm::vec2(m_VecX, vec.y));
	}
	else {
		glm::vec2 vec = physics->GetVelocity(m_Entity);
		physics->SetVelocity(m_Entity,glm::vec2(0,vec.y));
	}
	if (InputSystem::IsKeyPressed(Key::K)&&canJump) {
		canJump = false;
		physics->SetVelocity(m_Entity, glm::vec2(vec.x, m_VecY));
	}
}

void Player::fire(Timestep ts)
{
	if (CD < 1 / firingRate)CD+=ts;
	else {
		if (InputSystem::IsKeyPressed(Key::J))
		{
			//复制子弹
			Entity bullet = Bullet::Assemble();
			//设置子弹初始位置;
			//获取角色当前位置
			TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
			GlobalContext::GetInstance()->m_PhysicsSystem->SetPositionWithPixel(bullet,glm::vec2(trans.Translation.x,trans.Translation.y));
			//设置子弹方向
			Bullet* bulletScript = dynamic_cast<Bullet*>(bullet.GetComponent<NativeScriptComponent>().Instance);
			if (dirLeft) {
				bulletScript->SetVelocity(glm::vec2(-bulletScript->GetVelocity().x,bulletScript->GetVelocity().y));
			}
			else {
				bulletScript->SetVelocity(glm::vec2(bulletScript->GetVelocity().x,bulletScript->GetVelocity().y));
			}
			//设置子弹朝向
			CD -= 1 / firingRate;
		}
	}
}

