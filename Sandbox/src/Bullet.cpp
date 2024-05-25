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
	//�½�sprite���
	SpriteRendererComponent bulletSprite;
	bulletSprite.Texture = assetManager->GetTexture("bullet");
	//����sprite���
	scene->AddComponentWithUID<SpriteRendererComponent>(uid, bulletSprite);
	//����transform���
	TransformComponent bulletTrans;
	bulletTrans.Translation = glm::vec3{ 0,0,0.3f};
	//��С��scale*���� = ʵ�ʴ�С��
	//scale = ʵ�ʴ�С/����
	//bulletTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
	bulletTrans.Scale = glm::vec3{ 0.2f,0.2f,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = bulletTrans;
	//�ű����
	NativeScriptComponent bulletScript;
	bulletScript.Bind<Bullet>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, bulletScript);
	scene->DisableEntity(bullet);
	//�������
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Kinematic;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//��ײ��
	BoxCollider2DComponent box2d;
	//����size��offst��
	//��ȡ����ʵ�ʴ�С
	float width =bulletTrans.Scale.x * bulletSprite.Texture->GetWidth();
	float height = bulletTrans.Scale.y * bulletSprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//��Ϊbox2d y�����ϣ�opengl y�����£�offset.yΪ��ֵ
	box2d.Offset = glm::vec2(width / 2, height / 2);
	//���ô�����
	box2d.isSensor = true;
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	physics->AddEntity(bullet);
	//����
	//scene->DisableEntity(player);
	return bullet;

}

void Bullet::SetVelocity(glm::vec2 vec)
{
	m_Velocity = vec;
	//��ȡ��������	
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	//���ó�ʼ�ٶ�
	physics->SetVelocity(m_Entity,m_Velocity);
}

