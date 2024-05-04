#pragma once

#include <glm/glm.hpp>

namespace RPG2D {

	/// <summary>
	/// ������� ������������project view �� ���߳˻� �Լ������λ�� vec3 �Լ� �����ת float
	/// </summary>
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float,float);

		void SetProjection(float,float);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		//���λ��
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		//�����ת
		float m_Rotation = 0.0f;
	};

}
