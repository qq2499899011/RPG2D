#pragma once

#include <glm/glm.hpp>

namespace RPG2D {

	/// <summary>
	/// 正交相机 包含三个矩阵，project view 和 二者乘积 以及相机的位置 vec3 以及 相机旋转 float
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

		//相机位置
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		//相机旋转
		float m_Rotation = 0.0f;
	};

}
