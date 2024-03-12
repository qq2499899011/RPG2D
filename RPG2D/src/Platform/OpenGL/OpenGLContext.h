#pragma once

#include "RPG2D/Function/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace RPG2D {
	/// <summary>
	/// 创建glfw的上下文，用于渲染。
	/// </summary>
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}