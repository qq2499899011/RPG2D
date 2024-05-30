#include "Enemy.h"
#include "Bullet.h"
void Enemy::OnStart() {
}
void Enemy::OnCreate()
{

}

void Enemy::OnDestroy()
{

}

void Enemy::OnUpdate(Timestep ts)
{
	if (isFire) {
		fire(ts,dirLeft);
	}
	if (dirLeft != IsPlayerLeft())//朝向左边说明玩家在敌人左边
	{
		//计时
		countTime += ts;
		if (countTime >= reactTime) {
			//改变方向
			dirLeft = !dirLeft;
			//设置速度
			if (!isFire) {
				if (dirLeft)SetVelocity(glm::vec2(-5.0f, 0));
				else SetVelocity(glm::vec2(5.0f, 0));
			}
			countTime = 0;
		}
	}
	else {
		countTime = 0;
	}
}

bool Enemy::OnCollisionBegin(Entity other)
{
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	glm::vec2 vec = physics->GetVelocity(m_Entity);
	//获取对方的Tag，如果是子弹，就死亡
	if (other.GetName().compare("Bullet") == 0) {
		Bullet* bullet = dynamic_cast<Bullet*>(other.GetComponent<NativeScriptComponent>().Instance);
		//如果是玩家的子弹
		if (bullet->isPlayer()) {
			GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(m_Entity);
			GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(other);
		}
	}
	else if (other.GetName().compare("Ground") == 0) {
		//如果碰到地面就不动了
		physics->SetVelocity(m_Entity, glm::vec2(vec.x, 0));
		physics->SetAcceleration(m_Entity, glm::vec2(0, 0));
	}
	return true;
}

Entity Enemy::Assemble()
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	Entity enemy = scene->CreateEntity("Enemy");
	UID uid = enemy.GetUID();
	//创建animatior
	Ref<Animatior> enemyAnimatior = CreateRef<Animatior>();
	enemyAnimatior->AddFrame(0.0f, assetManager->GetTexture("enemy1"));
	enemyAnimatior->AddFrame(0.1f, assetManager->GetTexture("enemy2"));
	enemyAnimatior->AddFrame(0.2f, assetManager->GetTexture("enemy3"));
	enemyAnimatior->AddFrame(0.3f, assetManager->GetTexture("enemy4"));
	enemyAnimatior->AddFrame(0.4f, assetManager->GetTexture("enemy5"));
	//创建Controller
	Ref<AnimatiorController> enemyAC = CreateRef<AnimatiorController>();
	enemyAC->AddAnimation("idle", enemyAnimatior);
	enemyAC->SetState("idle");
	//创建动画组件
	AnimatiorControllerComponent enemyACcomponent;
	enemyACcomponent.animatiorController = enemyAC;
	scene->AddComponentWithUID<AnimatiorControllerComponent>(uid, enemyACcomponent);
	//新建sprite组件
	SpriteRendererComponent enemySprite;
	enemySprite.Texture = assetManager->GetTexture("enemy2");
	//增加sprite组件
	scene->AddComponentWithUID<SpriteRendererComponent>(uid, enemySprite);
	//设置transform组件
	TransformComponent enemyTrans;
	enemyTrans.Translation = glm::vec3{ 900.0f,0,0.3f };
	enemyTrans.Scale = glm::vec3{3,3,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = enemyTrans;
	//脚本组件
	NativeScriptComponent enemyScript;
	enemyScript.Bind<Enemy>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, enemyScript);
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rgb2d.Acceleration.y = -19.6f;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	//设置size和offst。
	//获取物体实际大小
	box2d.isSensor = true;
	float width = enemyTrans.Scale.x * enemySprite.Texture->GetWidth();
	float height = enemyTrans.Scale.y * enemySprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//因为box2d y轴向上，opengl y轴向下，offset.y为负值
	box2d.Offset = glm::vec2(width / 2, height / 2);
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(enemy);
	//获取物理引擎	
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	//设置初始速度
	physics->SetVelocity(enemy, glm::vec2(-5.0f,0.0f));
	return enemy;
}

Entity Enemy::AssembleFire()
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	Entity enemy = scene->CreateEntity("Enemy");
	UID uid = enemy.GetUID();
	//新建sprite组件
	SpriteRendererComponent enemySprite;
	enemySprite.Texture = assetManager->GetTexture("enemy02");
	//增加sprite组件
	scene->AddComponentWithUID<SpriteRendererComponent>(uid, enemySprite);
	//设置transform组件
	TransformComponent enemyTrans;
	enemyTrans.Translation = glm::vec3{ 1300.0f,0,0.3f };
	enemyTrans.Scale = glm::vec3{ 3,3,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = enemyTrans;
	//脚本组件
	NativeScriptComponent enemyScript;
	enemyScript.Bind<Enemy>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, enemyScript);
	//获取实例并setFire
	//获取实际的脚本组件
	enemyScript = enemy.GetComponent<NativeScriptComponent>();
	//获取脚本实例
	Enemy* enemyIns = dynamic_cast<Enemy*>(enemyScript.Instance);
	//设置isFIre
	enemyIns->SetIsFire(true);
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rgb2d.Acceleration.y = -19.6f;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	box2d.isSensor = true;
	physics->CalculateCollisionBox(enemyTrans, enemySprite.Texture->GetSize(), box2d);
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(enemy);
	return enemy;
}

bool Enemy::IsPlayerLeft()
{
	//获取玩家
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	float playerPosx = scene->GetComponentWithName<TransformComponent>("Player").Translation.x;
	float posx = m_Entity.GetComponent<TransformComponent>().Translation.x;
	//如果小于说明在左，否则在右
	if (posx < playerPosx)return false;
	else return true;
}

void Enemy::SetVelocity(glm::vec2 vec)
{
	m_Velocity = vec;
	//根据速度大小，设置朝向
	if (vec.x > 0) {
		//向右
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = true;
	}
	else {
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = false;
	}
	//获取物理引擎	
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	//设置初始速度
	physics->SetVelocity(m_Entity,m_Velocity);
}
//朝左开枪，设置
void Enemy::fire(Timestep ts,bool isLeft)
{
	if (CD < 1 / firingRate)CD+=ts;
	else {
		Entity bullet = Bullet::Assemble(false);
		//设置子弹初始位置;
		//获取角色当前位置
		TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
		GlobalContext::GetInstance()->m_PhysicsSystem->SetPositionWithPixel(bullet,glm::vec2(trans.Translation.x,trans.Translation.y+20.0f));
		//设置子弹方向
		Bullet* bulletScript = dynamic_cast<Bullet*>(bullet.GetComponent<NativeScriptComponent>().Instance);
		//bulletScript->SetVelocity(glm::vec2(-bulletScript->GetVelocity().x,bulletScript->GetVelocity().y));

		if (isLeft) {
			bulletScript->SetVelocity(glm::vec2(-5.0f, 0.0f));
			m_Entity.GetComponent<SpriteRendererComponent>().mirror = false;
		}
		else {
			bulletScript->SetVelocity(glm::vec2(5.0f, 0.0f));
			m_Entity.GetComponent<SpriteRendererComponent>().mirror = true;
		}
		CD -= 1 / firingRate;
	}
}
