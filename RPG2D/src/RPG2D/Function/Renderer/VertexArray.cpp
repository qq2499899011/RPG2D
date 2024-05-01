#include "RPG2Dpch.h"
#include "RPG2D/Function/Renderer/VertexArray.h"

#include "RPG2D/Function/Renderer/RendererManager.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace RPG2D {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererManager::GetAPI())
		{
			case RendererAPI::API::None:    RPG2D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		RPG2D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}