#include "RPG2Dpch.h"
#include "RendererManager.h"
#include "RPG2D/Function/Renderer/Renderer2D.h"

namespace RPG2D {

	Scope<RendererManager::SceneData> RendererManager::s_SceneData = CreateScope<RendererManager::SceneData>();

	void RendererManager::Init()
	{
		RPG2D_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void RendererManager::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void RendererManager::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void RendererManager::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void RendererManager::EndScene()
	{
	}

	/// <summary>
	/// 传入shader 物体数据 transform，以此画出游戏对象。
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="vertexArray"></param>
	/// <param name="transform"></param>
	void RendererManager::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
