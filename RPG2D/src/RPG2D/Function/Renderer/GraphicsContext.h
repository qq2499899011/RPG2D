#pragma once

namespace RPG2D {

	/// <summary>
	/// 图形上下文，方法：初始化 交换缓冲 创建对应API的context
	/// </summary>
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};

}