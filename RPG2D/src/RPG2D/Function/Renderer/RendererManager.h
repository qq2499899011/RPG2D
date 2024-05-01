#pragma once
#include "RPG2D/Function/Renderer/RenderCommand.h"
#include "RPG2D/Function/Renderer/OrthographicCamera.h"
#include "RPG2D/Function/Renderer/Shader.h"
#include "RPG2D/Core/Timestep.h"
#include "RPG2D/Resource/Scene/Scene.h"
#include "RPG2D/Function/Renderer/SpriteRenderer.h"
#include "RPG2D/Function/Renderer/OrthographicCameraController.h"
namespace RPG2D {

	/// <summary>
	/// �����������������������Ȼ��������Ⱦ����Ļ�ϵ��ܹ�������
	/// ֻ��ȾSprite����
	/// </summary>
	class RendererManager
	{
	public:
		void Init();
		void Update(Ref<Scene> scene,Timestep ts);
		void Shutdown();
		void OnWindowResize(uint32_t width, uint32_t height);
		void BeginScene(OrthographicCamera& camera);
		void EndScene();
		void DrawSprite(TransformComponent& transform,SpriteRendererComponent& sprite);
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		Ref<SpriteRenderer> m_SpriteRenderer;
		Ref<OrthographicCameraController> m_CameraController;
	};
}
