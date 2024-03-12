#include "RPG2Dpch.h"
#include "RPG2D/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RPG2D {

	/// <summary>
	/// 通过传入的左右平面 上下平面创造正交矩阵，其中进远平面分别为-1 和 1 ，然后view矩阵默认为单位矩阵。
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="top"></param>
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		RPG2D_PROFILE_FUNCTION();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/// <summary>
	/// 使用glm::ortho创建正交矩阵，同时更新viewprojection
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="top"></param>
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		RPG2D_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/// <summary>
	/// 根据相机pos和rotate计算view(相乘后取逆矩阵） 以及 更新vp矩阵。
	/// </summary>
	void OrthographicCamera::RecalculateViewMatrix()
	{
		RPG2D_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}