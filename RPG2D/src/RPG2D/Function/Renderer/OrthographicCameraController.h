#pragma once

#include "RPG2D/Function/Renderer/OrthographicCamera.h"
#include "RPG2D/Core/Timestep.h"
#include "RPG2D/Events/ApplicationEvent.h"
#include "RPG2D/Events/MouseEvent.h"

namespace RPG2D {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float,float,bool);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		//相机
		OrthographicCamera m_Camera;
		//是否可以旋转
		bool m_Rotation;
		//相机位置
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		//相机速度
		float m_CameraTranslationSpeed = 500.0f, m_CameraRotationSpeed = 180.0f;
	};

}
