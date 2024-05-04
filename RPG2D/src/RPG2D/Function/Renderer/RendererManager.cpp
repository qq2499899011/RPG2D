#include "RPG2Dpch.h"
#include "RendererManager.h"
#include "RPG2D/Resource/Scene/Scene.h"
#include "RPG2D/Function/Script/Components.h"
namespace RPG2D {
	void RendererManager::Init()
	{
		RPG2D_PROFILE_FUNCTION();
		//渲染相关初始化
		RenderCommand::Init();
		//创建场景数据
		//生成SpriteRenderer,这里需要先创建场景，才能初始化。
		m_SpriteRenderer = CreateRef<SpriteRenderer>(GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite"));
		//生成CameraController
		m_CameraController = CreateRef<OrthographicCameraController>(1280.0f,720.0f,false);
		//Renderer2D::Init();
	}

	void RendererManager::Update(Timestep ts)
	{
		//清理场景
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.5f, 1 });
		RenderCommand::Clear();
		//调用相机控制器
		m_CameraController->OnUpdate(ts);
		//获取相机transform.
		//设置主相机
		BeginScene(m_CameraController->GetCamera());
		//glm::mat4 projection = glm::ortho(0.0f,1280.0f ,0.0f , 720.0f, -1.0f, 1.0f);
		//GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite")->SetMat4("view_projection", projection);
		//遍历实体，进行渲染.
		//绘制精灵
		// Draw sprites
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		{
			auto group = m_Registry->group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				DrawSprite(transform, sprite);
			}
		}
		EndScene();
	}

	void RendererManager::Shutdown()
	{
		//Renderer2D::Shutdown();
	}

	void RendererManager::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void RendererManager::BeginScene(OrthographicCamera& camera)
	{
		//Shader设置相机Uniform
		GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite")->SetMat4("view_projection", m_CameraController->GetCamera().GetViewProjectionMatrix());
	}

	void RendererManager::EndScene()
	{
		//什么也不做

	}

	//直接与图片大小有关。
	void RendererManager::DrawSprite(TransformComponent& transform, SpriteRendererComponent& sprite)
	{
		//获取位置
		glm::vec2 pos = glm::vec2(transform.Translation.x, transform.Translation.y);
		//获取图片本身大小。
		//获取scale
		glm::vec2 scale = glm::vec2(transform.Scale.x, transform.Scale.y);
		//获取size
		glm::vec2 size = glm::vec2(scale.x * sprite.Texture->GetWidth(), scale.y * sprite.Texture->GetHeight());
		//根据Transform和Sprite将精灵绘制出来
		m_SpriteRenderer->DrawSprite(sprite.Texture, pos, size, transform.Rotation.z);
	}

}
