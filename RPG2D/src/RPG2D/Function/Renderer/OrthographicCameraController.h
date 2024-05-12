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
		//���
		OrthographicCamera m_Camera;
		//�Ƿ������ת
		bool m_Rotation;
		//���λ��
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		//����ٶ�
		float m_CameraTranslationSpeed = 500.0f, m_CameraRotationSpeed = 180.0f;
	};

}
