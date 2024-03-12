#include "RPG2Dpch.h"
#include "RPG2D/Function/InputSystem.h"

#include "RPG2D/Core/Application.h"
#include <GLFW/glfw3.h>

namespace RPG2D {

	bool InputSystem::IsKeyPressed(const KeyCode key)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool InputSystem::IsMouseButtonPressed(const MouseCode button)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 InputSystem::GetMousePosition()
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float InputSystem::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float InputSystem::GetMouseY()
	{
		return GetMousePosition().y;
	}

}