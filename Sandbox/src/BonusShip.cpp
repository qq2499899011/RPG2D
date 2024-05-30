#include "BonusShip.h"
#include "Bullet.h"
#include "Player.h"
void BonusShip::OnCreate()
{
}

void BonusShip::OnDestroy()
{
}

void BonusShip::OnUpdate(Timestep ts)
{
	//来回运动。
	movement(ts);
}

bool BonusShip::OnCollisionBegin(Entity other)
{
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	glm::vec2 vec = physics->GetVelocity(m_Entity);
	//获取对方的Tag，如果是子弹，就死亡
	if (other.GetName().compare("Bullet") == 0) {
		//化身bonus
		ReleaseAmo();
		//GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(m_Entity);
	}
	else if(other.GetName().compare("Player")==0) {
		//与玩家发生碰撞
		//调用设定枪械,获取脚本
		NativeScriptComponent playerComponent = other.GetComponent<NativeScriptComponent>();
		//获取脚本实例
		Player* player = dynamic_cast<Player*>(playerComponent.Instance);
		player->SwitchFireMode(bonus);
		destory = true;
	}
	return true;
}

bool BonusShip::OnCollisionEnd(Entity other)
{
	if (destory) {
		GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(m_Entity);
	}
	return true;
}

//需要填入武器类型
Entity BonusShip::Assemble(FireMode firemode)
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	Entity bonusShip = scene->CreateEntity("BonusShip");
	UID uid = bonusShip.GetUID();
	//新建sprite组件
	SpriteRendererComponent bonusShipSprite;
	bonusShipSprite.Texture = assetManager->GetTexture("bonusShip");
	//增加sprite组件
	bonusShip.AddComponent<SpriteRendererComponent>(bonusShipSprite);
	//增加动画组件
	Ref<Animatior> bonusAnimatior = CreateRef<Animatior>();
	bonusAnimatior->AddFrame(0.0f, assetManager->GetTexture("bonus1"));
	bonusAnimatior->AddFrame(0.05f, assetManager->GetTexture("bonus2"));
	bonusAnimatior->AddFrame(0.10f, assetManager->GetTexture("bonus3"));
	bonusAnimatior->AddFrame(0.15f, assetManager->GetTexture("bonus4"));
	Ref<Animatior> bonusShipAnimatior = CreateRef<Animatior>();
	bonusShipAnimatior->AddFrame(0.0f, assetManager->GetTexture("bonusShip"));
	//创建Controller
	Ref<AnimatiorController> bonusAC = CreateRef<AnimatiorController>();
	bonusAC->AddAnimation("bonus", bonusAnimatior);
	bonusAC->AddAnimation("bonusShip", bonusShipAnimatior);
	bonusAC->SetState("bonusShip");
	//创建动画组件
	AnimatiorControllerComponent bonusACcomponent;
	bonusACcomponent.animatiorController = bonusAC;
	bonusShip.AddComponent<AnimatiorControllerComponent>(bonusACcomponent);
	//设置transform组件
	TransformComponent bonusShipTrans;
	bonusShipTrans.Translation = glm::vec3{ 350.0f,0,0.3f };
	bonusShipTrans.Scale = glm::vec3{ 3,3,1 };
	bonusShip.GetComponent<TransformComponent>() = bonusShipTrans;
	//脚本组件
	NativeScriptComponent bonusShipScript;
	bonusShipScript.Bind<BonusShip>();
	bonusShip.AddComponent<NativeScriptComponent>(bonusShipScript);
	//获取实际的脚本组件
	bonusShipScript = bonusShip.GetComponent<NativeScriptComponent>();
	//获取脚本实例
	BonusShip* bonus = dynamic_cast<BonusShip*>(bonusShipScript.Instance);
	//设置mode
	bonus->SetFireMode(firemode);
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	bonusShip.AddComponent<Rigidbody2DComponent>(rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	box2d.isSensor = true;
	physics->CalculateCollisionBox(bonusShipTrans,bonusShipSprite.Texture->GetSize(),box2d);
	bonusShip.AddComponent<BoxCollider2DComponent>(box2d);
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(bonusShip);
	//设置初始速度
	//physics->SetVelocity(bonusShip,glm::vec2(3.0f,-3.0f));
	return bonusShip;
}

void BonusShip::ReleaseAmo()
{
	//改变速度,竖直下落
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	m_Velocity = glm::vec2(0, -3);
	physics->SetVelocity(m_Entity, m_Velocity);
	//设置动画为闪烁
	Ref<AnimatiorController> AC = m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController;
	//设置动画状态
	AC->SetState("bonus");
}

void BonusShip::SetFireMode(FireMode firemode)
{
	bonus = firemode;
}


void BonusShip::movement(Timestep ts)
{
	//获取物体位置
	glm::vec2 pos = m_Entity.GetPos();
	//RPG2D_TRACE("bonusPos = {0},{1}",pos.x,pos.y);
	//向右移动，累计时间到后，改变运动方向
	if (timeCount<changeTime)timeCount += ts;
	else {
		//修改移动速度
		Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
		//修改移动速度
		m_Velocity.y = m_Velocity.y*(-1);
		//输出新的速度
		RPG2D_INFO("new speed = {0},{1}", m_Velocity.x, m_Velocity.y);
		//设置新的移动速度
		physics->SetVelocity(m_Entity, m_Velocity);
		timeCount -= changeTime;
	}
}
