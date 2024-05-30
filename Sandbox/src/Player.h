#pragma once
#include <RPG2D.h>
using namespace RPG2D;
//半自动 自动 霰弹枪
enum class FireMode {
	SemiAutomatic,Automatic,ShotGun,
};
class Player:public ScriptableEntity
{
	//重写各个函数
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
	void fireAutomatic(Timestep);//三连发模式
	void fireShotGun();//霰弹枪发射
	void fireBullet();//发射一发子弹
	void get_fire_angle();
	//射击模式,射击
	FireMode fireMode = FireMode::SemiAutomatic;//射击模式
	int fireCount = 20;//剩余射击次数。
	int autoCount = 0;//连射剩余次数
	float playerSpeed = 500.0f;
	const float firingRate = 3.0f;
	float CD = 0;//射击冷却时间
	const float fireRateAuto = 15.0f;//每秒射击次数
	float AutoCD = 0;//三连发计算时间
	bool canFire = false;
	bool dirLeft = true;
	float m_VecX = 8.0f;
	float m_VecY = 12.0f;
	float m_AccY = -19.6f;//竖直加速度
	bool canJump = true;
	bool isFallen = false;
	float angle = 0.0f;//角色射击角度
	UID groundID;//站在地面的ID号
	Entity ammo;
	//各个状态
	std::string state;
	std::string state_old;//旧状态
};

