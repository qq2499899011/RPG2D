#include <RPG2D.h>
#include "RPG2D/Core/EntryPoint.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Ground.h"
#include "UI.h"
#include "ParticleExample.h"
#include "AssetPrepare.h"
#include "BonusShip.h"
using namespace RPG2D;
//继承init，设置相应场景与初始化
class Sandbox :public Application {
public:
	Sandbox() {
	}
	~Sandbox() {

	}
	//定义场景。
	void Init()override {
		Application::Init();
		//加载资源
		AssetPrepare::SetDir();
		AssetPrepare::LoadShader();
		AssetPrepare::LoadTexture();
		AssetPrepare::LoadAudio();
		//定义场景，组装实体		
		//创建场景
		Ref<SceneManager> sceneManager = GlobalContext::GetInstance()->m_SceneManager;
		Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
		//加入场景
		Ref<Scene> scene = sceneManager->CreateScene("Start");
		sceneManager->SetSceneActive("Start");
		//背景
		{
			scene->CreateEntity("background");
			SpriteRendererComponent spriteComponent;
			spriteComponent.Texture = assetManager->GetTexture("back");
			scene->AddComponentWithName<SpriteRendererComponent>("background", spriteComponent);
			//设置transform组件
			TransformComponent trans;
			trans.Translation = glm::vec3{0,0,0};
			trans.Scale = glm::vec3{3,3,1};
			scene->GetComponentWithName<TransformComponent>("background") = trans;
			//增加背景音效
			AudioComponent audio;
			audio.audio = assetManager->GetAudio("MainBgm");
			audio.play = true;
			scene->AddComponentWithName<AudioComponent>("background",audio);
		}
		{
			
		}
		Ground::Assemble();
		Player::Assemble();
		Enemy::Assemble();
		Enemy::AssembleFire();
		BonusShip::Assemble(FireMode::Automatic);
		//UI::Assemble();
		ParticleExample::Assemble();
		//开始场景
		Ref<Scene> menu = sceneManager->CreateScene("Menu");
		sceneManager->SetSceneActive("Menu");
		{
			Entity menuBack = menu->CreateEntity("MenuBack");
			SpriteRendererComponent spriteComponent;
			spriteComponent.Texture = assetManager->GetTexture("menu");
			menuBack.AddComponent<SpriteRendererComponent>(spriteComponent);
			//设置transform组件
			TransformComponent trans;
			trans.Translation = glm::vec3{ 266,0,0 };
			trans.Scale = glm::vec3{ 3,3,1 };
			menuBack.GetComponent<TransformComponent>() = trans;
			//增加背景音效
			AudioComponent audio;
			audio.audio = assetManager->GetAudio("TitleBgm");
			audio.play = true;
			menuBack.AddComponent<AudioComponent>(audio);
		}
		UI::MenuUIAssemble();
		//UI开始游戏按钮
		//UI按钮，开始游戏
		sceneManager->SetSceneActive("Menu");
	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}