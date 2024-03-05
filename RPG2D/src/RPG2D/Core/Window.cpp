#include "RPG2Dpch.h"
#include "Window.h"

#ifdef RPG2D_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif
namespace RPG2D
{
	Scope<Window> Window::Create(const WindowProps& props)//����ָ��window������ָ�롣
	{
		//�����ǰ��windowsƽ̨������
#ifdef RPG2D_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		RPG2D_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}
