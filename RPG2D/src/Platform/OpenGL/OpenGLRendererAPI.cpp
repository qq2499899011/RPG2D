#include "RPG2Dpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace RPG2D {
	/// <summary>
	/// opengl回调函数，根据不同严重等级使用spdlog输出不同严重程度的信息。
	/// </summary>
	/// <param name="source"></param>
	/// <param name="type"></param>
	/// <param name="id"></param>
	/// <param name="severity"></param>
	/// <param name="length"></param>
	/// <param name="message"></param>
	/// <param name="userParam"></param>
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         RPG2D_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       RPG2D_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          RPG2D_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: RPG2D_CORE_TRACE(message); return;
		}
		
		RPG2D_CORE_ASSERT(false, "Unknown severity level!");
	}
	
	/// <summary>
	/// 启用混合 设置混合方式 启用深度测试 启用线段平滑
	/// </summary>
	void OpenGLRendererAPI::Init()
	{
		RPG2D_PROFILE_FUNCTION();

	#ifdef RPG2D_DEBUG
		//开启调试
		glEnable(GL_DEBUG_OUTPUT);
		//设置同步调试，出现问题后立刻调用callback
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//注册回调函数
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		//设置message级别，什么样的message会被输出。
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD); // 设置混合方程
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	/// <summary>
	/// 调整视口大小
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	/// <summary>
	/// drawelement，包含对vao的绑定。
	/// </summary>
	/// <param name="vertexArray"></param>
	/// <param name="indexCount"></param>
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		//TODO:这里暂时搞一下，顶点的数量应该是indices.count/3;
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount()/3;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	/// <summary>
	/// 画线 内部包含vao的绑定
	/// </summary>
	/// <param name="vertexArray"></param>
	/// <param name="vertexCount"></param>
	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	/// <summary>
	/// 设置线段长度
	/// </summary>
	/// <param name="width"></param>
	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

}
