#include "RPG2Dpch.h"
#include "RPG2D/Function/Renderer/GraphicsContext.h"

#include "RPG2D/Function/Renderer/RendererManager.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace RPG2D {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererManager::GetAPI())
		{
			case RendererAPI::API::None:    RPG2D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		RPG2D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}