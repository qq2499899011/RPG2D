#include "RPG2Dpch.h"
#include "RendererManager.h"
#include "RPG2D/Resource/Scene/Scene.h"
#include "RPG2D/Resource/ResType/Components.h"
#include "RPG2D/Resource/ResType/Entity.h"
namespace RPG2D {
	RendererManager::RendererManager()
	{
		//����CameraController
		m_CameraController = CreateRef<OrthographicCameraController>(1280.0f,720.0f,false);
		//m_CameraController = CreateRef<OrthographicCameraController>(1600.0f,900.0f,false);
	}
	void RendererManager::Init()
	{
		
		//��Ⱦ��س�ʼ��
		RenderCommand::Init();
		//������������
		//����SpriteRenderer,������Ҫ�ȴ������������ܳ�ʼ����
		m_SpriteRenderer = CreateRef<SpriteRenderer>(GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite"));
		m_SquareRenderer = CreateRef<SquareRenderer>(GlobalContext::GetInstance()->m_AssetManager->GetShader("square"));
	}

	void RendererManager::Update(Timestep ts)
	{
		//������
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
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
		//��Ⱦ����
		{
			auto group = m_Registry->view<TransformComponent, ParticleEmitterComponent>();
			for (auto entity : group)
			{
				auto [trans, emitter] = group.get<TransformComponent, ParticleEmitterComponent>(entity);
				//��Ⱦ����
				DrawParticle(trans, emitter);
			}
		}
		//��Ⱦ������
		{
			auto group = m_Registry->view<TransformComponent, SquareRendererComponent>();
			for (auto entity : group)
			{
				auto [trans, square] = group.get<TransformComponent, SquareRendererComponent>(entity);
				//��Ⱦ����
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
		//Shader�������Uniform
		GlobalContext::GetInstance()->m_AssetManager->GetShader("sprite")->SetMat4("view_projection", m_CameraController->GetCamera().GetViewProjectionMatrix());
		//������������Ⱦ������Ⱦ
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
		//ʲôҲ����

	}

	//ֱ����ͼƬ��С�йء�
	void RendererManager::DrawSprite(TransformComponent& transform, SpriteRendererComponent& sprite)
	{
		//��ȡλ��
		glm::vec3 pos = glm::vec3(transform.Translation);
		//��ȡͼƬ�����С��
		//��ȡscale
		glm::vec2 scale = glm::vec2(transform.Scale.x, transform.Scale.y);
		//��ȡsize
		glm::vec2 size = glm::vec2(scale.x * sprite.Texture->GetWidth(), scale.y * sprite.Texture->GetHeight());
		//��ȡʵ�ʴ�С�󣬼���ʵ��λ��
		pos.x -= size.x * transform.anchor.x;
		pos.y -= size.y * transform.anchor.y;
		m_SpriteRenderer->DrawSprite(sprite.Texture, pos, size, transform.Rotation.z,sprite.mirror,sprite.Color,sprite.Index);
	}
	//ֱ����ͼƬ��С�йء�
	void RendererManager::DrawSquare(TransformComponent& transform, SquareRendererComponent& square)
	{
		//��ȡλ��
		glm::vec3 pos = glm::vec3(transform.Translation);
		//��ȡͼƬ�����С��
		//��ȡscale
		glm::vec2 scale = glm::vec2(transform.Scale.x, transform.Scale.y);
		//��ȡsize
		glm::vec2 size = glm::vec2(scale.x * square.Size.x, scale.y * square.Size.y);
		//��ȡʵ�ʴ�С�󣬼���ʵ��λ��
		pos.x -= size.x * transform.anchor.x;
		pos.y -= size.y * transform.anchor.y;
		m_SquareRenderer->DrawSquare(pos, size, transform.Rotation.z, square.Color);
	}

	//ֱ����ͼƬ��С�йء�
	void RendererManager::DrawParticle(TransformComponent& transform, ParticleEmitterComponent& emitter)
	{
		//��ȡλ��
		glm::vec3 pos = glm::vec3(transform.Translation);
		//��ȡͼƬ�����С��
		//��ȡscale
		glm::vec2 scale = glm::vec2(transform.Scale.x, transform.Scale.y);
		//����Transform��Sprite��������Ƴ���
		m_SpriteRenderer->DrawParticle(emitter.particleEmitter,pos,scale);
	}

}
