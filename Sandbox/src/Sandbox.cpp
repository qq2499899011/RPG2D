#include <RPG2D.h>
#include "RPG2D/Core/EntryPoint.h"
#include "Player.h"
#include "Bullet.h"
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
		//RPG2D_TRACE("{0}", event);//��仰������
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
		//��������
		assetManager->LoadTextureWithDir("player1.png",true,"player1");
		assetManager->LoadTextureWithDir("player2.png",true,"player2");
		assetManager->LoadTextureWithDir("player3.png",true,"player3");
		//��������
		Ref<SceneManager> sceneManager = GlobalContext::GetInstance()->m_SceneManager;
		//��������
		Ref<Scene> scene = CreateRef<Scene>("Start");
		//���볡��
		sceneManager->AddScene(scene);
		//����
		{
			//����ʵ��
			scene->CreateEntity("background");
			//�½�sprite���
			SpriteRendererComponent spriteComponent;
			spriteComponent.Texture = assetManager->GetTexture("face");
			//����sprite���
			scene->AddComponentWithName<SpriteRendererComponent>("background", spriteComponent);
			//����transform���
			TransformComponent trans;
			trans.Translation = glm::vec3{0,0,0};
			//��С��scale*���� = ʵ�ʴ�С��
			//scale = ʵ�ʴ�С/����
			trans.Scale = glm::vec3{ 1280.0f / spriteComponent.Texture->GetWidth(),720.0f / spriteComponent.Texture->GetHeight(),1 };
			scene->GetComponentByName<TransformComponent>("background") = trans;
		}
		//��ɫ
		{
			Entity player = scene->CreateEntity("Player");
			//����animatior
			Ref<Animatior> playerAnimatior = CreateRef<Animatior>();
			playerAnimatior->AddFrame(0.0f, assetManager->GetTexture("player1"));
			playerAnimatior->AddFrame(0.1f, assetManager->GetTexture("player2"));
			playerAnimatior->AddFrame(0.2f, assetManager->GetTexture("player3"));
			//����Controller
			Ref<AnimatiorController> playerAC = CreateRef<AnimatiorController>();
			playerAC->AddAnimation(AnimationState::Idle, playerAnimatior);
			playerAC->SetState(AnimationState::Idle);
			//�����������
			AnimatiorControllerComponent playerACcomponent;
			playerACcomponent.animatiorController = playerAC;
			scene->AddComponentWithName<AnimatiorControllerComponent>("Player", playerACcomponent);
			//�½�sprite���
			SpriteRendererComponent playerSprite;
			playerSprite.Texture = assetManager->GetTexture("player1");
			//����sprite���
			scene->AddComponentWithName<SpriteRendererComponent>("Player", playerSprite);
			//����transform���
			TransformComponent playerTrans;
			playerTrans.Translation = glm::vec3{ 0,0,0 };
			//��С��scale*���� = ʵ�ʴ�С��
			//scale = ʵ�ʴ�С/����
			//playerTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
			playerTrans.Scale = glm::vec3{1,1,1};
			scene->GetComponentByName<TransformComponent>("Player") = playerTrans;
			//�ű����
			NativeScriptComponent playerScript;
			playerScript.Bind<Player>();
			scene->AddComponentWithName<NativeScriptComponent>("Player", playerScript);
			//�������
			//scene->DisableEntity(player);
		}
		//�ӵ�
		{
			Entity player = scene->CreateEntity("Bullet");
			assetManager->LoadTextureWithDir("bullet.png",true,"bullet");
			//�½�sprite���
			SpriteRendererComponent bulletSprite;
			bulletSprite.Texture = assetManager->GetTexture("bullet");
			//����sprite���
			scene->AddComponentWithName<SpriteRendererComponent>("Bullet", bulletSprite);
			//����transform���
			TransformComponent bulletTrans;
			bulletTrans.Translation = glm::vec3{ 0,0,0 };
			//��С��scale*���� = ʵ�ʴ�С��
			//scale = ʵ�ʴ�С/����
			//bulletTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
			bulletTrans.Scale = glm::vec3{ 0.2,0.2,1 };
			scene->GetComponentByName<TransformComponent>("Bullet") = bulletTrans;
			//�ű����
			NativeScriptComponent bulletScript;
			bulletScript.Bind<Bullet>();
			scene->AddComponentWithName<NativeScriptComponent>("Bullet", bulletScript);
			//�������
			//scene->DisableEntity(player);

		}
		//���ó���
		sceneManager->SetSceneActive("Start");
		
	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}