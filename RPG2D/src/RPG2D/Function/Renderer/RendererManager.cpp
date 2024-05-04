#include "RPG2Dpch.h"
#include "RendererManager.h"
#include "RPG2D/Resource/Scene/Scene.h"
#include "RPG2D/Function/Script/Components.h"
namespace RPG2D {
	void RendererManager::Init()
	{
		RPG2D_PROFILE_FUNCTION();
		//��Ⱦ��س�ʼ��
		RenderCommand::Init();
		//������������
		//����SpriteRenderer,������Ҫ�ȴ������������ܳ�ʼ����
		m_SpriteRenderer = CreateRef<SpriteRenderer>(GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite"));
		//����CameraController
		m_CameraController = CreateRef<OrthographicCameraController>(1280.0f,720.0f,false);
		//Renderer2D::Init();
	}

	void RendererManager::Update(Timestep ts)
	{
		//������
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.5f, 1 });
		RenderCommand::Clear();
		//�������������
		m_CameraController->OnUpdate(ts);
		//��ȡ���transform.
		//���������
		BeginScene(m_CameraController->GetCamera());
		//glm::mat4 projection = glm::ortho(0.0f,1280.0f ,0.0f , 720.0f, -1.0f, 1.0f);
		//GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite")->SetMat4("view_projection", projection);
		//����ʵ�壬������Ⱦ.
		//���ƾ���
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
		//Shader�������Uniform
		GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite")->SetMat4("view_projection", m_CameraController->GetCamera().GetViewProjectionMatrix());
	}

	void RendererManager::EndScene()
	{
		//ʲôҲ����

	}

	//ֱ����ͼƬ��С�йء�
	void RendererManager::DrawSprite(TransformComponent& transform, SpriteRendererComponent& sprite)
	{
		//��ȡλ��
		glm::vec2 pos = glm::vec2(transform.Translation.x, transform.Translation.y);
		//��ȡͼƬ�����С��
		//��ȡscale
		glm::vec2 scale = glm::vec2(transform.Scale.x, transform.Scale.y);
		//��ȡsize
		glm::vec2 size = glm::vec2(scale.x * sprite.Texture->GetWidth(), scale.y * sprite.Texture->GetHeight());
		//����Transform��Sprite��������Ƴ���
		m_SpriteRenderer->DrawSprite(sprite.Texture, pos, size, transform.Rotation.z);
	}

}
