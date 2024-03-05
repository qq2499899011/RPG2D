#pragma once

#include "RPG2D/Core/KeyCodes.h"
#include "RPG2D/Core/MouseCodes.h"

#include <glm/glm.hpp>

/// <summary>
/// 检查input各种信息，检查某个按键是否被按下，鼠标的位置信息等。
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
