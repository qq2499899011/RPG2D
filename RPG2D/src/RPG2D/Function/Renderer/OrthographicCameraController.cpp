#include "RPG2Dpch.h"
#include "RPG2D/Function/Renderer/OrthographicCameraController.h"
#include "RPG2D/Function/Input/InputSystem.h"
#include "RPG2D/Core/KeyCodes.h"

namespace RPG2D {

	OrthographicCameraController::OrthographicCameraController(float width,float height,bool rotation)
		: m_Camera(width,height), m_Rotation(rotation),m_ScreenWidth(width),m_ScreenHeight(height)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (!lockX) {
			if (InputSystem::IsKeyPressed(Key::Left))
			{
				m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
				m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			}
			else if (InputSystem::IsKeyPressed(Key::Right))
			{
				m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
				m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			}
		}
		else {
			LookAtX();
		}
		if (!lockY) {
			if (InputSystem::IsKeyPressed(Key::Up))
			{
				m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
				m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			}
			else if (InputSystem::IsKeyPressed(Key::Down))
			{
				m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
				m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			}
		}
		else {
			LookAtY();
		}
		if (m_Rotation)
		{
			if (InputSystem::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (InputSystem::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}
		m_CameraPosition.x = std::max(m_CameraPosition.x, boardLeft);
		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RPG2D_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RPG2D_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::LookAtX()
	{
		//获取Entity的中心位置横坐标
		float entityX = m_LookAt.GetComponent<TransformComponent>().Translation.x;
		//计算相机X
		m_CameraPosition.x = entityX - m_ScreenWidth / 2;
	}

	void OrthographicCameraController::LookAtY()
	{
		//获取Entity的中心位置横坐标
		float entityY = m_LookAt.GetComponent<TransformComponent>().Translation.y;
		//计算相机X
		m_CameraPosition.y = entityY - m_ScreenHeight / 2;
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_Camera.SetProjection(width,height);
		m_ScreenWidth = width;
		m_ScreenHeight = height;
	}

	void OrthographicCameraController::SetLookAtX(Entity entity)
	{
		lockX = true;
		m_LookAt = entity;
	}

	void OrthographicCameraController::SetLookAtY(Entity entity)
	{
		lockY = true;
		m_LookAt = entity;
	}

	//鼠标滑轮滚动时，修改本身的zoomLevel
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		/*
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		*/
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}
