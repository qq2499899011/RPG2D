#include "RPG2Dpch.h"
#include "RPG2D/Function/Renderer/Buffer.h"

#include "RPG2D/Function/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace RPG2D {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererManager::GetAPI())
		{
			case RendererAPI::API::None:    RPG2D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
		}

		RPG2D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererManager::GetAPI())
		{
			case RendererAPI::API::None:    RPG2D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		RPG2D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RendererManager::GetAPI())
		{
			case RendererAPI::API::None:    RPG2D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		RPG2D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}