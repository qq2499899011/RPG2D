#pragma once
#include "RPG2D/Function/Renderer/RenderCommand.h"
#include "RPG2D/Function/Renderer/Shader.h"
#include "RPG2D/Core/Timestep.h"
#include "RPG2D/Function/Renderer/SpriteRenderer.h"
#include "RPG2D/Function/Renderer/OrthographicCameraController.h"
#include "RPG2D/Function/Renderer/SquareRenderer.h"
#include "RPG2D/Resource/ResType/Components.h"
namespace RPG2D {

	/// <summary>
	/// 真正遍历场景中所有组件，然后将内容渲染在屏幕上的总管理器。
	/// 只渲染Sprite就行
	/// </summary>
	class RendererManager
	{
	public:
		RendererManager();
		void Init();
		void Update(Timestep ts);
		void Shutdown();
		void OnWindowResize(uint32_t width, uint32_t height);
		void BeginScene(OrthographicCamera& camera);
		void LookAtX(Entity);
		void LookAtY(Entity);
		void EndScene();
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		Ref<SpriteRenderer> m_SpriteRenderer;
		Ref<SquareRenderer> m_SquareRenderer;
		Ref<OrthographicCameraController> m_CameraController;
		void DrawSprite(TransformComponent& transform,SpriteRendererComponent& sprite);
		void DrawSquare(TransformComponent& transform,SquareRendererComponent& sprite);
		void DrawParticle(TransformComponent& transform,ParticleEmitterComponent&);
	};
}
