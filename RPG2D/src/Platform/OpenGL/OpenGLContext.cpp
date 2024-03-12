#include "RPG2Dpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace RPG2D {

	/// <summary>
	/// ͨ������glfw������г�ʼ��
	/// </summary>
	/// <param name="windowHandle"></param>
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RPG2D_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	/// <summary>
	/// ���������ģ�����opengl�������������ǰ�Կ���opengl��Ϣ��
	/// </summary>
	void OpenGLContext::Init()
	{
		RPG2D_PROFILE_FUNCTION();
		//ָ�������ģ�����ǰ���ڹ�����opengl�������ģ�������������������Ⱦ������Ⱦ����ǰ������
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RPG2D_CORE_ASSERT(status, "Failed to initialize Glad!");

		RPG2D_CORE_INFO("OpenGL Info:");
		RPG2D_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		RPG2D_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		RPG2D_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		RPG2D_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "RPG2D requires at least OpenGL version 4.5!");
	}
	/// <summary>
	/// �������壬�򵥵Ķ�glfwSwapBuffer���з�װ��
	/// </summary>
	void OpenGLContext::SwapBuffers()
	{
		RPG2D_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}
