#include <RPG2D.h>
#include "RPG2D/Core/EntryPoint.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Ground.h"
#include "UI.h"
#include "ParticleExample.h"
using namespace RPG2D;
class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}
	void OnUpdate(Timestep ts) override
	{
		//RPG2D_INFO("ExampleLayer::Update");
	}
	void OnEvent(Event& event) override
	{
		//�����¼����ͣ�����ͨ�����ַ�ʽ���¼����д���
	}
};

//�̳�init��������Ӧ�������ʼ��
class Sandbox :public Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
	//���峡����
	void Init()override {
		Application::Init();
		//���峡������װʵ��		
		//��������load shaders
		Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
		//������Դ·��
		assetManager->SetShaderDir("C:/Work/GameEngine/RPG2D/Sandbox/asset/shader/");
		assetManager->SetTextureDir("C:/Work/GameEngine/RPG2D/Sandbox/asset/texture/");
		assetManager->SetAudioDir("C:/Work/GameEngine/RPG2D/Sandbox/asset/audio/");
		//��ȡ����
		float width =static_cast<float>(m_Window->GetWidth());
		float height = static_cast<float>(m_Window->GetHeight());
		//������ɫ��
		assetManager->LoadShaderWithDir("sprite.vs","sprite.fs", "", "sprite");
		assetManager->GetShader("sprite")->Bind();
		assetManager->GetShader("sprite")->SetInt("sprite", 0);
		//��������
		assetManager->LoadTextureWithDir("awesomeface.png", true, "face");
		assetManager->LoadTextureWithDir("background.png", true, "back");
		assetManager->LoadTextureWithDir("bullet.png",true,"bullet");
		//��������
		//��ɫ�ƶ�
		assetManager->LoadTextureWithDir("player/player_run01.png",true,"player_run1");
		assetManager->LoadTextureWithDir("player/player_run02.png",true,"player_run2");
		assetManager->LoadTextureWithDir("player/player_run03.png",true,"player_run3");
		//��ɫ����
		assetManager->LoadTextureWithDir("player/player_dead01.png",true,"player_dead1");
		assetManager->LoadTextureWithDir("player/player_dead02.png",true,"player_dead2");
		assetManager->LoadTextureWithDir("player/player_dead03.png",true,"player_dead3");
		//��ɫ���ϳ�ǹ+�ƶ�
		assetManager->LoadTextureWithDir("player/player_uprun01.png",true,"player_uprun1");
		assetManager->LoadTextureWithDir("player/player_uprun02.png",true,"player_uprun2");
		assetManager->LoadTextureWithDir("player/player_uprun03.png",true,"player_uprun3");
		//��ɫ���³�ǹ+�ƶ�
		assetManager->LoadTextureWithDir("player/player_downrun01.png",true,"player_downrun1");
		assetManager->LoadTextureWithDir("player/player_downrun02.png",true,"player_downrun2");
		assetManager->LoadTextureWithDir("player/player_downrun03.png",true,"player_downrun3");
		//��ɫ��Ծ
		assetManager->LoadTextureWithDir("player/player_jump01.png",true,"player_jump1");
		assetManager->LoadTextureWithDir("player/player_jump02.png",true,"player_jump2");
		//��ɫ���ϳ�ǹ
		assetManager->LoadTextureWithDir("player/player_up01.png",true,"player_up1");
		//��ɫſ��
		assetManager->LoadTextureWithDir("player/player_down01.png",true,"player_down1");
		//��������
		assetManager->LoadTextureWithDir("enemy1.png",true,"enemy1");
		assetManager->LoadTextureWithDir("enemy2.png",true,"enemy2");
		assetManager->LoadTextureWithDir("enemy3.png",true,"enemy3");
		assetManager->LoadTextureWithDir("enemy4.png",true,"enemy4");
		//������Ч
		assetManager->LoadAudioWithDir("test.mp3", "bgm");
		//��������
		Ref<SceneManager> sceneManager = GlobalContext::GetInstance()->m_SceneManager;
		//���볡��
		Ref<Scene> scene = sceneManager->CreateScene("Start");
		sceneManager->SetSceneActive("Start");
		//����
		{
			//����ʵ��
			scene->CreateEntity("background");
			//�½�sprite���
			SpriteRendererComponent spriteComponent;
			//spriteComponent.Texture = assetManager->GetTexture("face");
			Ref<Texture2D> texture = assetManager->GetTexture("back");
			//texture->SetSubWH(100,100);
			spriteComponent.Texture = assetManager->GetTexture("back");
			//spriteComponent.Index = glm::ivec2(3, 1);
			//����sprite���
			scene->AddComponentWithName<SpriteRendererComponent>("background", spriteComponent);
			//����transform���
			TransformComponent trans;
			trans.Translation = glm::vec3{0,0,0};
			//��С��scale*���� = ʵ�ʴ�С��
			//scale = ʵ�ʴ�С/����
			//trans.Scale = glm::vec3{ 1280.0f / spriteComponent.Texture->GetWidth(),720.0f / spriteComponent.Texture->GetHeight(),1 };
			//�����Ĵ�С���Ŵ���1.2��,���մ�СΪ 2945*768 ,ʵ�ʴ�С2454*640;
			trans.Scale = glm::vec3{1.2,1.2,1};
			scene->GetComponentWithName<TransformComponent>("background") = trans;
			//���ӱ�����Ч
			AudioComponent audio;
			audio.audio = assetManager->GetAudio("bgm");
			audio.play = true;
			scene->AddComponentWithName<AudioComponent>("background",audio);
		}
		//����
		{
			//���Ͻ�0,666.��С2945*102��ײ��
			Ground::Assemble();
		}
		//��ɫ
		{
			Player::Assemble();
		}
		//����
		{
			Enemy::Assemble();
			//�ٴ���һ������
			Entity enemy2 = Enemy::Assemble();
			Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
			glm::vec2 pos = physics->GetPixelPosition(enemy2);
			pos.x += 500.0f;
			//�������
			physics->SetPositionWithPixel(enemy2, pos);
		}
		//UI
		{
			UI::Assemble();
		}
		//����Ч��
		{
			ParticleExample::Assemble();
		}
	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}