#pragma once
#include <RPG2D.h>
using namespace RPG2D;
//���Զ� �Զ� ����ǹ
enum class FireMode {
	SemiAutomatic,Automatic,ShotGun,
};
class Player:public ScriptableEntity
{
	//��д��������
public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual bool OnCollisionBegin(Entity other)override;
	virtual bool OnCollisionEnd(Entity other)override;
	void SwitchFireMode(FireMode);
	void SetAmmo(Entity entity);
	static Entity Assemble();
private:
	void state_update();
	void movement(Timestep ts);
	void fire(Timestep);
	void fireAutomatic(Timestep);//������ģʽ
	void fireShotGun();//����ǹ����
	void fireBullet();//����һ���ӵ�
	void get_fire_angle();
	//���ģʽ,���
	FireMode fireMode = FireMode::SemiAutomatic;//���ģʽ
	int fireCount = 20;//ʣ�����������
	int autoCount = 0;//����ʣ�����
	float playerSpeed = 500.0f;
	const float firingRate = 3.0f;
	float CD = 0;//�����ȴʱ��
	const float fireRateAuto = 15.0f;//ÿ���������
	float AutoCD = 0;//����������ʱ��
	bool canFire = false;
	bool dirLeft = true;
	float m_VecX = 8.0f;
	float m_VecY = 12.0f;
	float m_AccY = -19.6f;//��ֱ���ٶ�
	bool canJump = true;
	bool isFallen = false;
	float angle = 0.0f;//��ɫ����Ƕ�
	UID groundID;//վ�ڵ����ID��
	Entity ammo;
	//����״̬
	std::string state;
	std::string state_old;//��״̬
};

