#pragma once

#include "RPG2D/Function/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace RPG2D {
	/// <summary>
	/// ����glfw�������ģ�������Ⱦ��
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