#include "RPG2Dpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace RPG2D {
	/// opengl回调函数，根据不同严重等级使用spdlog输出不同严重程度的信息。
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
	
	/// 启用混合 设置混合方式 启用深度测试 启用线段平滑
	void OpenGLRendererAPI::Init()
	{
		

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

	/// drawelement，包含对vao的绑定。
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		//TODO:这里暂时搞一下，顶点的数量应该是indices.count/3;
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount()/3;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	/// 画线 内部包含vao的绑定
	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	/// 设置线段长度
	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

}
