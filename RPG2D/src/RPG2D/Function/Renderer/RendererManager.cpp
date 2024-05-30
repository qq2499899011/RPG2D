#include "RPG2Dpch.h"
#include "RendererManager.h"
#include "RPG2D/Resource/Scene/Scene.h"
#include "RPG2D/Resource/ResType/Components.h"
#include "RPG2D/Resource/ResType/Entity.h"
namespace RPG2D {
	RendererManager::RendererManager()
	{
		//生成CameraController
		m_CameraController = CreateRef<OrthographicCameraController>(1280.0f,720.0f,false);
		//m_CameraController = CreateRef<OrthographicCameraController>(1600.0f,900.0f,false);
	}
	void RendererManager::Init()
	{
		
		//渲染相关初始化
		RenderCommand::Init();
		//创建场景数据
		//生成SpriteRenderer,这里需要先创建场景，才能初始化。
		m_SpriteRenderer = CreateRef<SpriteRenderer>(GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite"));
		m_SquareRenderer = CreateRef<SquareRenderer>(GlobalContext::GetInstance()->m_AssetManager->GetShader("square"));
	}

	void RendererManager::Update(Timestep ts)
	{
		//清理场景
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
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
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		{
			auto group = m_Registry->group<TransformComponent>(entt::get<SpriteRendererComponent>);
			//auto group = m_Registry->group<TransformComponent,SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				DrawSprite(transform, sprite);
			}
		}
		//渲染粒子
		{
			auto group = m_Registry->view<TransformComponent, ParticleEmitterComponent>();
			for (auto entity : group)
			{
				auto [trans, emitter] = group.get<TransformComponent, ParticleEmitterComponent>(entity);
				//渲染粒子
				DrawParticle(trans, emitter);
			}
		}
		//渲染正方形
		{
			auto group = m_Registry->view<TransformComponent, SquareRendererComponent>();
			for (auto entity : group)
			{
				auto [trans, square] = group.get<TransformComponent, SquareRendererComponent>(entity);
				//渲染粒子
				DrawSquare(trans, square);
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
		//设置正方形渲染器的渲染
		GlobalContext::GetInstance()->m_AssetManager->GetShader("square")->SetMat4("view_projection", m_CameraController->GetCamera().GetViewProjectionMatrix());
	}

	void RendererManager::LookAtX(Entity entity)
	{
		m_CameraController->SetLookAtX(entity);
	}

	void RendererManager::LookAtY(Entity entity)
	{
		m_CameraController->SetLookAtY(entity);
	}

	void RendererManager::EndScene()
	{
		//什么也不做

	}

	//直接与图片大小有关。
	void RendererManager::DrawSprite(TransformComponent& transform, SpriteRendererComponent& sprite)
	{
		//获取位置
		glm::vec3 pos = glm::vec3(transform.Translation);
		//获取图片本身大小。
		//获取scale
		glm::vec2 scale = glm::vec2(transform.Scale.x, transform.Scale.y);
		//获取size
		glm::vec2 size = glm::vec2(scale.x * sprite.Texture->GetWidth(), scale.y * sprite.Texture->GetHeight());
		//获取实际大小后，计算实际位置
		pos.x -= size.x * transform.anchor.x;
		pos.y -= size.y * transform.anchor.y;
		m_SpriteRenderer->DrawSprite(sprite.Texture, pos, size, transform.Rotation.z,sprite.mirror,sprite.Color,sprite.Index);
	}
	//直接与图片大小有关。
	void RendererManager::DrawSquare(TransformComponent& transform, SquareRendererComponent& square)
	{
		//获取位置
		glm::vec3 pos = glm::vec3(transform.Translation);
		//获取图片本身大小。
		//获取scale
		glm::vec2 scale = glm::vec2(transform.Scale.x, transform.Scale.y);
		//获取size
		glm::vec2 size = glm::vec2(scale.x * square.Size.x, scale.y * square.Size.y);
		//获取实际大小后，计算实际位置
		pos.x -= size.x * transform.anchor.x;
		pos.y -= size.y * transform.anchor.y;
		m_SquareRenderer->DrawSquare(pos, size, transform.Rotation.z, square.Color);
	}

	//直接与图片大小有关。
	void RendererManager::DrawParticle(TransformComponent& transform, ParticleEmitterComponent& emitter)
	{
		//获取位置
		glm::vec3 pos = glm::vec3(transform.Translation);
		//获取图片本身大小。
		//获取scale
		glm::vec2 scale = glm::vec2(transform.Scale.x, transform.Scale.y);
		//根据Transform和Sprite将精灵绘制出来
		m_SpriteRenderer->DrawParticle(emitter.particleEmitter,pos,scale);
	}

}
