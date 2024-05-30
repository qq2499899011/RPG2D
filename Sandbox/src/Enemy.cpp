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
	if (dirLeft != IsPlayerLeft())//�������˵������ڵ������
	{
		//��ʱ
		countTime += ts;
		if (countTime >= reactTime) {
			//�ı䷽��
			dirLeft = !dirLeft;
			//�����ٶ�
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
	//��ȡ�Է���Tag��������ӵ���������
	if (other.GetName().compare("Bullet") == 0) {
		Bullet* bullet = dynamic_cast<Bullet*>(other.GetComponent<NativeScriptComponent>().Instance);
		//�������ҵ��ӵ�
		if (bullet->isPlayer()) {
			GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(m_Entity);
			GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(other);
		}
	}
	else if (other.GetName().compare("Ground") == 0) {
		//�����������Ͳ�����
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
	//����animatior
	Ref<Animatior> enemyAnimatior = CreateRef<Animatior>();
	enemyAnimatior->AddFrame(0.0f, assetManager->GetTexture("enemy1"));
	enemyAnimatior->AddFrame(0.1f, assetManager->GetTexture("enemy2"));
	enemyAnimatior->AddFrame(0.2f, assetManager->GetTexture("enemy3"));
	enemyAnimatior->AddFrame(0.3f, assetManager->GetTexture("enemy4"));
	enemyAnimatior->AddFrame(0.4f, assetManager->GetTexture("enemy5"));
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
	enemyTrans.Translation = glm::vec3{ 900.0f,0,0.3f };
	enemyTrans.Scale = glm::vec3{3,3,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = enemyTrans;
	//�ű����
	NativeScriptComponent enemyScript;
	enemyScript.Bind<Enemy>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, enemyScript);
	//�������
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rgb2d.Acceleration.y = -19.6f;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//��ײ��
	BoxCollider2DComponent box2d;
	//����size��offst��
	//��ȡ����ʵ�ʴ�С
	box2d.isSensor = true;
	float width = enemyTrans.Scale.x * enemySprite.Texture->GetWidth();
	float height = enemyTrans.Scale.y * enemySprite.Texture->GetHeight();
	box2d.Size = glm::vec2(width, height);
	//��Ϊbox2d y�����ϣ�opengl y�����£�offset.yΪ��ֵ
	box2d.Offset = glm::vec2(width / 2, height / 2);
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(enemy);
	//��ȡ��������	
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	//���ó�ʼ�ٶ�
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
	//�½�sprite���
	SpriteRendererComponent enemySprite;
	enemySprite.Texture = assetManager->GetTexture("enemy02");
	//����sprite���
	scene->AddComponentWithUID<SpriteRendererComponent>(uid, enemySprite);
	//����transform���
	TransformComponent enemyTrans;
	enemyTrans.Translation = glm::vec3{ 1300.0f,0,0.3f };
	enemyTrans.Scale = glm::vec3{ 3,3,1 };
	scene->GetComponentWithUID<TransformComponent>(uid) = enemyTrans;
	//�ű����
	NativeScriptComponent enemyScript;
	enemyScript.Bind<Enemy>();
	scene->AddComponentWithUID<NativeScriptComponent>(uid, enemyScript);
	//��ȡʵ����setFire
	//��ȡʵ�ʵĽű����
	enemyScript = enemy.GetComponent<NativeScriptComponent>();
	//��ȡ�ű�ʵ��
	Enemy* enemyIns = dynamic_cast<Enemy*>(enemyScript.Instance);
	//����isFIre
	enemyIns->SetIsFire(true);
	//�������
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rgb2d.Acceleration.y = -19.6f;
	scene->AddComponentWithUID<Rigidbody2DComponent>(uid, rgb2d);
	//��ײ��
	BoxCollider2DComponent box2d;
	box2d.isSensor = true;
	physics->CalculateCollisionBox(enemyTrans, enemySprite.Texture->GetSize(), box2d);
	scene->AddComponentWithUID<BoxCollider2DComponent>(uid, box2d);
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(enemy);
	return enemy;
}

bool Enemy::IsPlayerLeft()
{
	//��ȡ���
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	float playerPosx = scene->GetComponentWithName<TransformComponent>("Player").Translation.x;
	float posx = m_Entity.GetComponent<TransformComponent>().Translation.x;
	//���С��˵�����󣬷�������
	if (posx < playerPosx)return false;
	else return true;
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
//����ǹ������
void Enemy::fire(Timestep ts,bool isLeft)
{
	if (CD < 1 / firingRate)CD+=ts;
	else {
		Entity bullet = Bullet::Assemble(false);
		//�����ӵ���ʼλ��;
		//��ȡ��ɫ��ǰλ��
		TransformComponent& trans = m_Entity.GetComponent<TransformComponent>();
		GlobalContext::GetInstance()->m_PhysicsSystem->SetPositionWithPixel(bullet,glm::vec2(trans.Translation.x,trans.Translation.y+20.0f));
		//�����ӵ�����
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
