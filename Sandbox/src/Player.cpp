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
	//移动
	movement(ts);
	//开火
	fire(ts);
}

void Player::OnCollisionBegin(Entity other)
{
	//如果和地面发生碰撞，才可以继续条约

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
	//创建animatior
	Ref<Animatior> playerAnimatiorRun = CreateRef<Animatior>();
	playerAnimatiorRun->AddFrame(0.0f, assetManager->GetTexture("player1"));
	playerAnimatiorRun->AddFrame(0.1f, assetManager->GetTexture("player2"));
	playerAnimatiorRun->AddFrame(0.2f, assetManager->GetTexture("player3"));
	Ref<Animatior> playerAnimatiorIdle = CreateRef<Animatior>();
	playerAnimatiorIdle->AddFrame(0.0f, assetManager->GetTexture("player1"));
	//创建Controller
	Ref<AnimatiorController> playerAC = CreateRef<AnimatiorController>();
	playerAC->AddAnimation(AnimationState::Running, playerAnimatiorRun);
	playerAC->AddAnimation(AnimationState::Idle, playerAnimatiorIdle);
	playerAC->SetState(AnimationState::Idle);
	//创建动画组件
	AnimatiorControllerComponent playerACcomponent;
	playerACcomponent.animatiorController = playerAC;
	scene->AddComponentWithName<AnimatiorControllerComponent>("Player", playerACcomponent);
	//新建sprite组件
	SpriteRendererComponent playerSprite;
	playerSprite.Texture = assetManager->GetTexture("player1");
	//增加sprite组件
	scene->AddComponentWithName<SpriteRendererComponent>("Player", playerSprite);
	//设置transform组件
	TransformComponent playerTrans;
	playerTrans.Translation = glm::vec3{ 0,0,0.3f };
	//大小。scale*长宽 = 实际大小。
	//scale = 实际大小/长宽。
	//playerTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
	playerTrans.Scale = glm::vec3{1,1,1};
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
	//设置size和offst。
	//获取物体实际大小
	float width = playerTrans.Scale.x * playerSprite.Texture->GetWidth();
	float height = playerTrans.Scale.y * playerSprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//因为box2d y轴向上，opengl y轴向下，offset.y为负值
	box2d.Offset = glm::vec2(width / 2, height / 2);
	scene->AddComponentWithName<BoxCollider2DComponent>("Player", box2d);
	//将物体加入到物理引擎中。
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(player);
	return player;
}

void Player::movement(Timestep ts)
{
	//获取物理引擎
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	//打印输出角色速度
	glm::vec2 vec = physics->GetVelocity(m_Entity);
	//RPG2D_INFO("playerSpeed:{0},{1}", vec.x, vec.y);
	//获取位置
	glm::vec2 pos =physics->GetPixelPosition(m_Entity);
	//操作玩家
	//左右移动不修改移动速度
	if (InputSystem::IsKeyPressed(Key::A))
	{
		dirLeft = true;
		//设置状态
		m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController->SetState(AnimationState::Running);
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = !dirLeft;
		//给速度而不是修改位置
		//设置水平速度
		physics->SetVelocity(m_Entity,glm::vec2(-m_VecX,vec.y));
	}
	else if (InputSystem::IsKeyPressed(Key::D))
	{
		dirLeft = false;
		m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController->SetState(AnimationState::Running);
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = !dirLeft;
		//设置水平速度
		physics->SetVelocity(m_Entity, glm::vec2(m_VecX, vec.y));
	}
	else {
		//设置速度为0
		m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController->SetState(AnimationState::Idle);
		//获取当前速度
		glm::vec2 vec = physics->GetVelocity(m_Entity);
		physics->SetVelocity(m_Entity,glm::vec2(0,vec.y));
	}

	if (InputSystem::IsKeyPressed(Key::K)&&canJump) {
		canJump = false;
		//按下K可以跳跃
		//给角色一个向上跳跃的速度
		//设置垂直速度
		physics->SetVelocity(m_Entity, glm::vec2(vec.x, m_VecX));
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

