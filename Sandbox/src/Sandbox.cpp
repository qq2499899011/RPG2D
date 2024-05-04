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
		//RPG2D_TRACE("{0}", event);//这句话有问题
	}
};

//继承init，设置相应场景与初始化
class Sandbox :public Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
	//定义场景。
	void Init()override {
		Application::Init();
		//定义场景，组装实体		
		//加载内容load shaders
		Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;
		//设置资源路径
		assetManager->SetShaderDir("C:/Work/GameEngine/RPG2D/Sandbox/asset/shader/");
		assetManager->SetTextureDir("C:/Work/GameEngine/RPG2D/Sandbox/asset/texture/");
		//获取长宽
		float width =static_cast<float>(m_Window->GetWidth());
		float height = static_cast<float>(m_Window->GetHeight());
		//加载并设定Shader
		assetManager->LoadShaderWithDir("sprite.vs","sprite.fs", "", "sprite");
		assetManager->GetShader("sprite")->Bind();
		assetManager->GetShader("sprite")->SetInt("sprite", 0);
		//加载纹理
		assetManager->LoadTextureWithDir("awesomeface.png", true, "face");
		//动画纹理
		assetManager->LoadTextureWithDir("player1.png",true,"player1");
		assetManager->LoadTextureWithDir("player2.png",true,"player2");
		assetManager->LoadTextureWithDir("player3.png",true,"player3");
		//创建场景
		Ref<SceneManager> sceneManager = GlobalContext::GetInstance()->m_SceneManager;
		//创建场景
		Ref<Scene> scene = CreateRef<Scene>("Start");
		//加入场景
		sceneManager->AddScene(scene);
		//背景
		{
			//创建实体
			scene->CreateEntity("background");
			//新建sprite组件
			SpriteRendererComponent spriteComponent;
			spriteComponent.Texture = assetManager->GetTexture("face");
			//增加sprite组件
			scene->AddComponentWithName<SpriteRendererComponent>("background", spriteComponent);
			//设置transform组件
			TransformComponent trans;
			trans.Translation = glm::vec3{0,0,0};
			//大小。scale*长宽 = 实际大小。
			//scale = 实际大小/长宽。
			trans.Scale = glm::vec3{ 1280.0f / spriteComponent.Texture->GetWidth(),720.0f / spriteComponent.Texture->GetHeight(),1 };
			scene->GetComponentByName<TransformComponent>("background") = trans;
		}
		//角色
		{
			Entity player = scene->CreateEntity("Player");
			//创建animatior
			Ref<Animatior> playerAnimatior = CreateRef<Animatior>();
			playerAnimatior->AddFrame(0.0f, assetManager->GetTexture("player1"));
			playerAnimatior->AddFrame(0.1f, assetManager->GetTexture("player2"));
			playerAnimatior->AddFrame(0.2f, assetManager->GetTexture("player3"));
			//创建Controller
			Ref<AnimatiorController> playerAC = CreateRef<AnimatiorController>();
			playerAC->AddAnimation(AnimationState::Idle, playerAnimatior);
			playerAC->SetState(AnimationState::Idle);
			//创建动画组件
			AnimatiorControllerComponent playerACcomponent;
			playerACcomponent.animatiorController = playerAC;
			scene->AddComponentWithName<AnimatiorControllerComponent>("Player", playerACcomponent);
			//新建sprite组件
			SpriteRendererComponent playerSprite;
			playerSprite.Texture = assetManager->GetTexture("player1");
			//增加sprite组件
			scene->AddComponentWithName<SpriteRendererComponent>("Player", playerSprite);
			//设置transform组件
			TransformComponent playerTrans;
			playerTrans.Translation = glm::vec3{ 0,0,0 };
			//大小。scale*长宽 = 实际大小。
			//scale = 实际大小/长宽。
			//playerTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
			playerTrans.Scale = glm::vec3{1,1,1};
			scene->GetComponentByName<TransformComponent>("Player") = playerTrans;
			//脚本组件
			NativeScriptComponent playerScript;
			playerScript.Bind<Player>();
			scene->AddComponentWithName<NativeScriptComponent>("Player", playerScript);
			//物理组件
			//scene->DisableEntity(player);
		}
		//子弹
		{
			Entity player = scene->CreateEntity("Bullet");
			assetManager->LoadTextureWithDir("bullet.png",true,"bullet");
			//新建sprite组件
			SpriteRendererComponent bulletSprite;
			bulletSprite.Texture = assetManager->GetTexture("bullet");
			//增加sprite组件
			scene->AddComponentWithName<SpriteRendererComponent>("Bullet", bulletSprite);
			//设置transform组件
			TransformComponent bulletTrans;
			bulletTrans.Translation = glm::vec3{ 0,0,0 };
			//大小。scale*长宽 = 实际大小。
			//scale = 实际大小/长宽。
			//bulletTrans.Scale = glm::vec3{ 100.0f / playerSprite.Texture->GetWidth(),50.0f / playerSprite.Texture->GetHeight(),1 };
			bulletTrans.Scale = glm::vec3{ 0.2,0.2,1 };
			scene->GetComponentByName<TransformComponent>("Bullet") = bulletTrans;
			//脚本组件
			NativeScriptComponent bulletScript;
			bulletScript.Bind<Bullet>();
			scene->AddComponentWithName<NativeScriptComponent>("Bullet", bulletScript);
			//物理组件
			//scene->DisableEntity(player);

		}
		//启用场景
		sceneManager->SetSceneActive("Start");
		
	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}