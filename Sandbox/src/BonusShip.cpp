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
	//�����˶���
	movement(ts);
}

bool BonusShip::OnCollisionBegin(Entity other)
{
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	glm::vec2 vec = physics->GetVelocity(m_Entity);
	//��ȡ�Է���Tag��������ӵ���������
	if (other.GetName().compare("Bullet") == 0) {
		//����bonus
		ReleaseAmo();
		//GlobalContext::GetInstance()->m_SceneManager->GetSceneActive()->RemoveEntity(m_Entity);
	}
	else if(other.GetName().compare("Player")==0) {
		//����ҷ�����ײ
		//�����趨ǹе,��ȡ�ű�
		NativeScriptComponent playerComponent = other.GetComponent<NativeScriptComponent>();
		//��ȡ�ű�ʵ��
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

//��Ҫ������������
Entity BonusShip::Assemble(FireMode firemode)
{
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	Entity bonusShip = scene->CreateEntity("BonusShip");
	UID uid = bonusShip.GetUID();
	//�½�sprite���
	SpriteRendererComponent bonusShipSprite;
	bonusShipSprite.Texture = assetManager->GetTexture("bonusShip");
	//����sprite���
	bonusShip.AddComponent<SpriteRendererComponent>(bonusShipSprite);
	//���Ӷ������
	Ref<Animatior> bonusAnimatior = CreateRef<Animatior>();
	bonusAnimatior->AddFrame(0.0f, assetManager->GetTexture("bonus1"));
	bonusAnimatior->AddFrame(0.05f, assetManager->GetTexture("bonus2"));
	bonusAnimatior->AddFrame(0.10f, assetManager->GetTexture("bonus3"));
	bonusAnimatior->AddFrame(0.15f, assetManager->GetTexture("bonus4"));
	Ref<Animatior> bonusShipAnimatior = CreateRef<Animatior>();
	bonusShipAnimatior->AddFrame(0.0f, assetManager->GetTexture("bonusShip"));
	//����Controller
	Ref<AnimatiorController> bonusAC = CreateRef<AnimatiorController>();
	bonusAC->AddAnimation("bonus", bonusAnimatior);
	bonusAC->AddAnimation("bonusShip", bonusShipAnimatior);
	bonusAC->SetState("bonusShip");
	//�����������
	AnimatiorControllerComponent bonusACcomponent;
	bonusACcomponent.animatiorController = bonusAC;
	bonusShip.AddComponent<AnimatiorControllerComponent>(bonusACcomponent);
	//����transform���
	TransformComponent bonusShipTrans;
	bonusShipTrans.Translation = glm::vec3{ 350.0f,0,0.3f };
	bonusShipTrans.Scale = glm::vec3{ 3,3,1 };
	bonusShip.GetComponent<TransformComponent>() = bonusShipTrans;
	//�ű����
	NativeScriptComponent bonusShipScript;
	bonusShipScript.Bind<BonusShip>();
	bonusShip.AddComponent<NativeScriptComponent>(bonusShipScript);
	//��ȡʵ�ʵĽű����
	bonusShipScript = bonusShip.GetComponent<NativeScriptComponent>();
	//��ȡ�ű�ʵ��
	BonusShip* bonus = dynamic_cast<BonusShip*>(bonusShipScript.Instance);
	//����mode
	bonus->SetFireMode(firemode);
	//�������
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	bonusShip.AddComponent<Rigidbody2DComponent>(rgb2d);
	//��ײ��
	BoxCollider2DComponent box2d;
	box2d.isSensor = true;
	physics->CalculateCollisionBox(bonusShipTrans,bonusShipSprite.Texture->GetSize(),box2d);
	bonusShip.AddComponent<BoxCollider2DComponent>(box2d);
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(bonusShip);
	//���ó�ʼ�ٶ�
	//physics->SetVelocity(bonusShip,glm::vec2(3.0f,-3.0f));
	return bonusShip;
}

void BonusShip::ReleaseAmo()
{
	//�ı��ٶ�,��ֱ����
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	m_Velocity = glm::vec2(0, -3);
	physics->SetVelocity(m_Entity, m_Velocity);
	//���ö���Ϊ��˸
	Ref<AnimatiorController> AC = m_Entity.GetComponent<AnimatiorControllerComponent>().animatiorController;
	//���ö���״̬
	AC->SetState("bonus");
}

void BonusShip::SetFireMode(FireMode firemode)
{
	bonus = firemode;
}


void BonusShip::movement(Timestep ts)
{
	//��ȡ����λ��
	glm::vec2 pos = m_Entity.GetPos();
	//RPG2D_TRACE("bonusPos = {0},{1}",pos.x,pos.y);
	//�����ƶ����ۼ�ʱ�䵽�󣬸ı��˶�����
	if (timeCount<changeTime)timeCount += ts;
	else {
		//�޸��ƶ��ٶ�
		Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
		//�޸��ƶ��ٶ�
		m_Velocity.y = m_Velocity.y*(-1);
		//����µ��ٶ�
		RPG2D_INFO("new speed = {0},{1}", m_Velocity.x, m_Velocity.y);
		//�����µ��ƶ��ٶ�
		physics->SetVelocity(m_Entity, m_Velocity);
		timeCount -= changeTime;
	}
}
