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
//�̳�init��������Ӧ�������ʼ��
class Sandbox :public Application {
public:
	Sandbox() {
	}
	~Sandbox() {

	}
	//���峡����
	void Init()override {
		Application::Init();
		//������Դ
		AssetPrepare::SetDir();
		AssetPrepare::LoadShader();
		AssetPrepare::LoadTexture();
		AssetPrepare::LoadAudio();
		//���峡������װʵ��		
		//��������
		Ref<SceneManager> sceneManager = GlobalContext::GetInstance()->m_SceneManager;
		Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
		//���볡��
		Ref<Scene> scene = sceneManager->CreateScene("Start");
		sceneManager->SetSceneActive("Start");
		//����
		{
			scene->CreateEntity("background");
			SpriteRendererComponent spriteComponent;
			spriteComponent.Texture = assetManager->GetTexture("back");
			scene->AddComponentWithName<SpriteRendererComponent>("background", spriteComponent);
			//����transform���
			TransformComponent trans;
			trans.Translation = glm::vec3{0,0,0};
			trans.Scale = glm::vec3{3,3,1};
			scene->GetComponentWithName<TransformComponent>("background") = trans;
			//���ӱ�����Ч
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
		//��ʼ����
		Ref<Scene> menu = sceneManager->CreateScene("Menu");
		sceneManager->SetSceneActive("Menu");
		{
			Entity menuBack = menu->CreateEntity("MenuBack");
			SpriteRendererComponent spriteComponent;
			spriteComponent.Texture = assetManager->GetTexture("menu");
			menuBack.AddComponent<SpriteRendererComponent>(spriteComponent);
			//����transform���
			TransformComponent trans;
			trans.Translation = glm::vec3{ 266,0,0 };
			trans.Scale = glm::vec3{ 3,3,1 };
			menuBack.GetComponent<TransformComponent>() = trans;
			//���ӱ�����Ч
			AudioComponent audio;
			audio.audio = assetManager->GetAudio("TitleBgm");
			audio.play = true;
			menuBack.AddComponent<AudioComponent>(audio);
		}
		UI::MenuUIAssemble();
		//UI��ʼ��Ϸ��ť
		//UI��ť����ʼ��Ϸ
		sceneManager->SetSceneActive("Menu");
	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}