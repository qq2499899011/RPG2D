#include "Bullet.h"

void Bullet::OnCreate()
{
}

void Bullet::OnDestroy()
{
}

void Bullet::OnUpdate(Timestep ts)
{
	//movement(ts);
	
}

Entity Bullet::Assemble()
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	Entity bullet = scene->CreateEntity("Bullet");
	UID uid = bullet.GetUID();
	//新建sprite组件
	SpriteRendererComponent bulletSprite;
	bulletSprite.Texture = assetManager->GetTexture("bullet");
	//增加sprite组件
	scene->AddComponentWithUID<SpriteRendererComponent>(uid, bulletSprite);
	//设置transform组件
	TransformComponent bulletTrans;
	bulletTrans.Translation = glm::vec3{ 0,0,0.3f};
	//大小。scale*长宽 = 实际大小。
	//scale = 实际大小/长宽。
	//bulletTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
	bulletTrans.Scale = glm::vec3{ 0.2f,0.2f,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = bulletTrans;
	//脚本组件
	NativeScriptComponent bulletScript;
	bulletScript.Bind<Bullet>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, bulletScript);
	scene->DisableEntity(bullet);
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Kinematic;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	//设置size和offst。
	//获取物体实际大小
	float width =bulletTrans.Scale.x * bulletSprite.Texture->GetWidth();
	float height = bulletTrans.Scale.y * bulletSprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//因为box2d y轴向上，opengl y轴向下，offset.y为负值
	box2d.Offset = glm::vec2(width / 2, height / 2);
	//设置传感器
	box2d.isSensor = true;
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	physics->AddEntity(bullet);
	//加入
	//scene->DisableEntity(player);
	return bullet;

}

void Bullet::SetVelocity(glm::vec2 vec)
{
	m_Velocity = vec;
	//获取物理引擎	
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	//设置初始速度
	physics->SetVelocity(m_Entity,m_Velocity);
}

