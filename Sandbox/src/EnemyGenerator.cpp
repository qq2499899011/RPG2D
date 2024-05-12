#include "EnemyGenerator.h"

void EnemyGenerator::OnCreate()
{
}

void EnemyGenerator::OnDestroy()
{
}

void EnemyGenerator::OnUpdate(Timestep ts)
{
}

Entity EnemyGenerator::Assemble()
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	Entity EG = scene->CreateEntity("EG");
	UID uid = EG.GetUID();
	assetManager->LoadTextureWithDir("EG.png", true, "EG");
	//sprite组件
	SpriteRendererComponent EGSprite;
	EGSprite.Texture = assetManager->GetTexture("EG");
	scene->AddComponentWithUID<SpriteRendererComponent>(uid, EGSprite);
	//transform组件
	TransformComponent EGTrans;
	EGTrans.Translation = glm::vec3{ 0,0,0.3f };
	//大小。scale*长宽 = 实际大小。
	//scale = 实际大小/长宽。
	//EGTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
	EGTrans.Scale = glm::vec3{ 1,1,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = EGTrans;
	//脚本组件
	NativeScriptComponent EGScript;
	EGScript.Bind<EnemyGenerator>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, EGScript);
	scene->DisableEntity(EG);
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	//设置size和offst。
	//获取物体实际大小
	float width = EGTrans.Scale.x * EGSprite.Texture->GetWidth();
	float height = EGTrans.Scale.y * EGSprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//因为box2d y轴向上，opengl y轴向下，offset.y为负值
	box2d.Offset = glm::vec2(width / 2, height / 2);
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	physics->AddEntity(EG);
	//加入
	//scene->DisableEntity(player);
	return EG;
	return Entity();
}

void EnemyGenerator::GenerateEnemy(Timestep ts)
{
}
