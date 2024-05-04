#pragma once

#include "RPG2D/Core/Base.h"
#include "RPG2D/Events/Event.h"
#include <string>
#include <functional>

namespace RPG2D {

	struct WindowProps //数据结构，记录窗口的一些信息
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "RPG2D Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// 接口，定义一些windows要用到的函数
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>; //返回void.接受Event的事件处理函数类型

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;//处理窗口的刷新和更新

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;//设置事件的回调函数，当事件发生时回调callback
		virtual void SetVSync(bool enabled) = 0;//设置垂直同步
		virtual bool IsVSync() const = 0;//是否开启了垂直同步
		virtual float GetTime() const = 0;//返回时间
		virtual void* GetNativeWindow() const = 0;//获取窗口

		static Scope<Window> Create(const WindowProps& props = WindowProps());//创建窗口，创建一个指向window类的unique指针
	};

}
