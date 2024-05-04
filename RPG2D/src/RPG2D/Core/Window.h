#pragma once

#include "RPG2D/Core/Base.h"
#include "RPG2D/Events/Event.h"
#include <string>
#include <functional>

namespace RPG2D {

	struct WindowProps //���ݽṹ����¼���ڵ�һЩ��Ϣ
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

	// �ӿڣ�����һЩwindowsҪ�õ��ĺ���
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>; //����void.����Event���¼�����������

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;//�����ڵ�ˢ�º͸���

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;//�����¼��Ļص����������¼�����ʱ�ص�callback
		virtual void SetVSync(bool enabled) = 0;//���ô�ֱͬ��
		virtual bool IsVSync() const = 0;//�Ƿ����˴�ֱͬ��
		virtual float GetTime() const = 0;//����ʱ��
		virtual void* GetNativeWindow() const = 0;//��ȡ����

		static Scope<Window> Create(const WindowProps& props = WindowProps());//�������ڣ�����һ��ָ��window���uniqueָ��
	};

}
