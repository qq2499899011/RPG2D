#pragma once

#include "RPG2D/Function/Renderer/Buffer.h"

#include <memory>

namespace RPG2D {

	/// <summary>
	/// VAO抽象API，包括：绑定 解绑 添加VBO 设定VEO 根据API类型创建对应的VAO
	/// </summary>
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

}
