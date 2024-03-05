#pragma once

#include "RPG2D/Core/KeyCodes.h"
#include "RPG2D/Core/MouseCodes.h"

#include <glm/glm.hpp>

/// <summary>
/// ���input������Ϣ�����ĳ�������Ƿ񱻰��£�����λ����Ϣ�ȡ�
/// </summary>
namespace RPG2D {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
