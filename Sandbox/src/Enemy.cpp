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
	//��ȡ�Է���Tag��������ӵ���������
	if (other.GetName().compare("Bullet") == 0) {
		//��ײʱ����
		//RPG2D_INFO("dead");
		//�����Ƴ�����
		//GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(m_Entity);
	}
}

Entity Enemy::Assemble()
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	Entity enemy = scene->CreateEntity("Enemy");
	UID uid = enemy.GetUID();
	//����animatior
	Ref<Animatior> enemyAnimatior = CreateRef<Animatior>();
	enemyAnimatior->AddFrame(0.0f, assetManager->GetTexture("enemy1"));
	enemyAnimatior->AddFrame(0.1f, assetManager->GetTexture("enemy2"));
	enemyAnimatior->AddFrame(0.2f, assetManager->GetTexture("enemy3"));
	enemyAnimatior->AddFrame(0.2f, assetManager->GetTexture("enemy4"));
	//����Controller
	Ref<AnimatiorController> enemyAC = CreateRef<AnimatiorController>();
	enemyAC->AddAnimation("idle", enemyAnimatior);
	enemyAC->SetState("idle");
	//�����������
	AnimatiorControllerComponent enemyACcomponent;
	enemyACcomponent.animatiorController = enemyAC;
	scene->AddComponentWithUID<AnimatiorControllerComponent>(uid, enemyACcomponent);
	//�½�sprite���
	SpriteRendererComponent enemySprite;
	enemySprite.Texture = assetManager->GetTexture("enemy2");
	//����sprite���
	scene->AddComponentWithUID<SpriteRendererComponent>(uid, enemySprite);
	//����transform���
	TransformComponent enemyTrans;
	enemyTrans.Translation = glm::vec3{ 350.0f,0,0.3f };
	//��С��scale*���� = ʵ�ʴ�С��
	//scale = ʵ�ʴ�С/����
	//enemyTrans.Scale = glm::vec3{ 100.0f / enemySprite.Texture->GetWidth(),50.0f / enemySprite.Texture->GetHeight(),1 };
	enemyTrans.Scale = glm::vec3{1,1,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = enemyTrans;
	//�ű����
	NativeScriptComponent enemyScript;
	enemyScript.Bind<Enemy>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, enemyScript);
	//�������
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//��ײ��
	BoxCollider2DComponent box2d;
	//����size��offst��
	//��ȡ����ʵ�ʴ�С
	float width = enemyTrans.Scale.x * enemySprite.Texture->GetWidth();
	float height = enemyTrans.Scale.y * enemySprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//��Ϊbox2d y�����ϣ�opengl y�����£�offset.yΪ��ֵ
	box2d.Offset = glm::vec2(width / 2, height / 2);
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(enemy);
	return enemy;
}

void Enemy::SetVelocity(glm::vec2 vec)
{
	m_Velocity = vec;
	//�����ٶȴ�С�����ó���
	if (vec.x > 0) {
		//����
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = true;
	}
	else {
		m_Entity.GetComponent<SpriteRendererComponent>().mirror = false;
	}
	//��ȡ��������	
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	//���ó�ʼ�ٶ�
	physics->SetVelocity(m_Entity,m_Velocity);
}

void Enemy::fire(Timestep ts)
{
	if (CD < 1 / firingRate)CD+=ts;
	else {
		Entity bullet = Bullet::Assemble();
		//�����ӵ���ʼλ��;
		//��ȡ��ɫ��ǰλ��
		TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
		GlobalContext::GetInstance()->m_PhysicsSystem->SetPositionWithPixel(bullet,glm::vec2(trans.Translation.x,trans.Translation.y+20.0f));
		//�����ӵ�����
		Bullet* bulletScript = dynamic_cast<Bullet*>(bullet.GetComponent<NativeScriptComponent>().Instance);
		bulletScript->SetVelocity(glm::vec2(-bulletScript->GetVelocity().x,bulletScript->GetVelocity().y));
		CD -= 1 / firingRate;
	}
}
