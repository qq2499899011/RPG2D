#include "Enemy.h"
#include "Bullet.h"
void Enemy::OnCreate()
{
}

void Enemy::OnDestroy()
{
}

void Enemy::OnUpdate(Timestep ts)
{
	fire(ts);
}

void Enemy::OnCollisionBegin(Entity other)
{
	//获取对方的Tag，如果是子弹，就死亡
	if (other.GetName().compare("Bullet") == 0) {
		//碰撞时调用
		//RPG2D_INFO("dead");
		//调用移除函数
		//GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(m_Entity);
	}
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
	enemyAnimatior->AddFrame(0.2f, assetManager->GetTexture("enemy4"));
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
	enemyTrans.Translation = glm::vec3{ 350.0f,0,0.3f };
	//大小。scale*长宽 = 实际大小。
	//scale = 实际大小/长宽。
	//enemyTrans.Scale = glm::vec3{ 100.0f / enemySprite.Texture->GetWidth(),50.0f / enemySprite.Texture->GetHeight(),1 };
	enemyTrans.Scale = glm::vec3{1,1,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = enemyTrans;
	//脚本组件
	NativeScriptComponent enemyScript;
	enemyScript.Bind<Enemy>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, enemyScript);
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	//设置size和offst。
	//获取物体实际大小
	float width = enemyTrans.Scale.x * enemySprite.Texture->GetWidth();
	float height = enemyTrans.Scale.y * enemySprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//因为box2d y轴向上，opengl y轴向下，offset.y为负值
	box2d.Offset = glm::vec2(width / 2, height / 2);
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(enemy);
	return enemy;
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

void Enemy::fire(Timestep ts)
{
	if (CD < 1 / firingRate)CD+=ts;
	else {
		Entity bullet = Bullet::Assemble();
		//设置子弹初始位置;
		//获取角色当前位置
		TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
		GlobalContext::GetInstance()->m_PhysicsSystem->SetPositionWithPixel(bullet,glm::vec2(trans.Translation.x,trans.Translation.y+20.0f));
		//设置子弹方向
		Bullet* bulletScript = dynamic_cast<Bullet*>(bullet.GetComponent<NativeScriptComponent>().Instance);
		bulletScript->SetVelocity(glm::vec2(-bulletScript->GetVelocity().x,bulletScript->GetVelocity().y));
		CD -= 1 / firingRate;
	}
}
