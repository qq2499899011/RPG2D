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
		//新增事件类型，可以通过这种方式对事件进行处理
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
		assetManager->LoadTextureWithDir("background.png", true, "back");
		assetManager->LoadTextureWithDir("bullet.png",true,"bullet");
		//动画纹理
		assetManager->LoadTextureWithDir("player1.png",true,"player1");
		assetManager->LoadTextureWithDir("player2.png",true,"player2");
		assetManager->LoadTextureWithDir("player3.png",true,"player3");
		//敌人纹理
		assetManager->LoadTextureWithDir("enemy1.png",true,"enemy1");
		assetManager->LoadTextureWithDir("enemy2.png",true,"enemy2");
		assetManager->LoadTextureWithDir("enemy3.png",true,"enemy3");
		assetManager->LoadTextureWithDir("enemy4.png",true,"enemy4");
		//创建场景
		Ref<SceneManager> sceneManager = GlobalContext::GetInstance()->m_SceneManager;
		Ref<Scene> scene = CreateRef<Scene>("Start");
		//加入场景
		sceneManager->AddScene(scene);
		sceneManager->SetSceneActive("Start");
		//背景
		{
			//创建实体
			scene->CreateEntity("background");
			//新建sprite组件
			SpriteRendererComponent spriteComponent;
			//spriteComponent.Texture = assetManager->GetTexture("face");
			Ref<Texture2D> texture = assetManager->GetTexture("back");
			//texture->SetSubWH(100,100);
			spriteComponent.Texture = assetManager->GetTexture("back");
			//spriteComponent.Index = glm::ivec2(3, 1);
			//增加sprite组件
			scene->AddComponentWithName<SpriteRendererComponent>("background", spriteComponent);
			//设置transform组件
			TransformComponent trans;
			trans.Translation = glm::vec3{0,0,0};
			//大小。scale*长宽 = 实际大小。
			//scale = 实际大小/长宽。
			//trans.Scale = glm::vec3{ 1280.0f / spriteComponent.Texture->GetWidth(),720.0f / spriteComponent.Texture->GetHeight(),1 };
			//背景的大小被放大了1.2倍,最终大小为 2945*768 ,实际大小2454*640;
			trans.Scale = glm::vec3{1.2,1.2,1};
			scene->GetComponentWithName<TransformComponent>("background") = trans;
		}
		//地面
		{
			//左上角0,666.大小2945*102碰撞体
			Ground::Assemble();
		}
		//角色
		{
			Player::Assemble();
		}
		//敌人
		{
			Enemy::Assemble();
			//再创建一个敌人
			Entity enemy2 = Enemy::Assemble();
			Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
			glm::vec2 pos = physics->GetPixelPosition(enemy2);
			pos.x += 500.0f;
			//操作玩家
			physics->SetPositionWithPixel(enemy2, pos);
		}
		//UI
		{
			UI::Assemble();
		}
		//粒子效果
		{
			ParticleExample::Assemble();
		}
	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}