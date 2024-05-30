#include <RPG2D.h>
#include "AssetPrepare.h"
void AssetPrepare::LoadTexture()
{
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	//加载纹理
	assetManager->LoadTextureWithDir("awesomeface.png", true, "face");
	assetManager->LoadTextureWithDir("stage1.png", true, "back");
	assetManager->LoadTextureWithDir("bullet.png",true,"bullet");
	//角色移动
	assetManager->LoadTextureWithDir("player/player_run01.png",true,"player_run1");
	assetManager->LoadTextureWithDir("player/player_run02.png",true,"player_run2");
	assetManager->LoadTextureWithDir("player/player_run03.png",true,"player_run3");
	//角色死亡
	assetManager->LoadTextureWithDir("player/player_dead01.png",true,"player_dead1");
	assetManager->LoadTextureWithDir("player/player_dead02.png",true,"player_dead2");
	assetManager->LoadTextureWithDir("player/player_dead03.png",true,"player_dead3");
	//角色向上持枪+移动
	assetManager->LoadTextureWithDir("player/player_uprun01.png",true,"player_uprun1");
	assetManager->LoadTextureWithDir("player/player_uprun02.png",true,"player_uprun2");
	assetManager->LoadTextureWithDir("player/player_uprun03.png",true,"player_uprun3");
	//角色向下持枪+移动
	assetManager->LoadTextureWithDir("player/player_downrun01.png",true,"player_downrun1");
	assetManager->LoadTextureWithDir("player/player_downrun02.png",true,"player_downrun2");
	assetManager->LoadTextureWithDir("player/player_downrun03.png",true,"player_downrun3");
	//角色跳跃
	assetManager->LoadTextureWithDir("player/player_jump01.png",true,"player_jump1");
	assetManager->LoadTextureWithDir("player/player_jump02.png",true,"player_jump2");
	//角色向上持枪
	assetManager->LoadTextureWithDir("player/player_up01.png",true,"player_up1");
	//角色趴下
	assetManager->LoadTextureWithDir("player/player_down01.png",true,"player_down1");
	//敌人纹理
	assetManager->LoadTextureWithDir("enemy/enemy1.png",true,"enemy1");
	assetManager->LoadTextureWithDir("enemy/enemy2.png",true,"enemy2");
	assetManager->LoadTextureWithDir("enemy/enemy3.png",true,"enemy3");
	assetManager->LoadTextureWithDir("enemy/enemy4.png",true,"enemy4");
	assetManager->LoadTextureWithDir("enemy/enemy5.png",true,"enemy5");
	//第二种敌人
	assetManager->LoadTextureWithDir("enemy2/enemy.png",true,"enemy02");
	//目录
	assetManager->LoadTextureWithDir("menu.png",true,"menu");
	//Bonus
	assetManager->LoadTextureWithDir("bonus/bonusShip.png",true,"bonusShip");
	assetManager->LoadTextureWithDir("bonus/bonus1.png",true,"bonus1");
	assetManager->LoadTextureWithDir("bonus/bonus2.png",true,"bonus2");
	assetManager->LoadTextureWithDir("bonus/bonus3.png",true,"bonus3");
	assetManager->LoadTextureWithDir("bonus/bonus4.png",true,"bonus4");
	//雪花
	assetManager->LoadTextureWithDir("snowflake.png",true,"snow");
}

void AssetPrepare::LoadShader()
{	
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	//加载着色器
	assetManager->LoadShaderWithDir("sprite.vs","sprite.fs", "", "sprite");
	assetManager->GetShader("sprite")->Bind();
	assetManager->GetShader("sprite")->SetInt("sprite", 0);
	//加载纯色渲染着色器
	assetManager->LoadShaderWithDir("square.vs","square.fs", "", "square");
}

void AssetPrepare::LoadAudio()
{
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	//加载音效
	assetManager->LoadAudioWithDir("Title.mp3","TitleBgm");
	assetManager->LoadAudioWithDir("MainBgm.mp3","MainBgm");

}

void AssetPrepare::SetDir()
{
	Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
	//设置资源路径
	/*
	assetManager->SetShaderDir("C:/Work/GameEngine/RPG2D/Sandbox/asset/shader/");
	assetManager->SetTextureDir("C:/Work/GameEngine/RPG2D/Sandbox/asset/texture/");
	assetManager->SetAudioDir("C:/Work/GameEngine/RPG2D/Sandbox/asset/audio/");
	*/
	assetManager->SetShaderDir("asset/shader/");
	assetManager->SetTextureDir("asset/texture/");
	assetManager->SetAudioDir("asset/audio/");
}
