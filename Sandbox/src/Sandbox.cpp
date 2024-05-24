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
		//��ȡ����
		float width =static_cast<float>(m_Window->GetWidth());
		float height = static_cast<float>(m_Window->GetHeight());
		//���ز��趨Shader
		assetManager->LoadShaderWithDir("sprite.vs","sprite.fs", "", "sprite");
		assetManager->GetShader("sprite")->Bind();
		assetManager->GetShader("sprite")->SetInt("sprite", 0);
		//��������
		assetManager->LoadTextureWithDir("awesomeface.png", true, "face");
		assetManager->LoadTextureWithDir("background.png", true, "back");
		assetManager->LoadTextureWithDir("bullet.png",true,"bullet");
		//��������
		assetManager->LoadTextureWithDir("player1.png",true,"player1");
		assetManager->LoadTextureWithDir("player2.png",true,"player2");
		assetManager->LoadTextureWithDir("player3.png",true,"player3");
		//��������
		assetManager->LoadTextureWithDir("enemy1.png",true,"enemy1");
		assetManager->LoadTextureWithDir("enemy2.png",true,"enemy2");
		assetManager->LoadTextureWithDir("enemy3.png",true,"enemy3");
		assetManager->LoadTextureWithDir("enemy4.png",true,"enemy4");
		//��������
		Ref<SceneManager> sceneManager = GlobalContext::GetInstance()->m_SceneManager;
		Ref<Scene> scene = CreateRef<Scene>("Start");
		//���볡��
		sceneManager->AddScene(scene);
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