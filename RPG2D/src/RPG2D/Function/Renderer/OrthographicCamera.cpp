#include "RPG2Dpch.h"
#include "RPG2D/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RPG2D {

	/// <summary>
	/// ͨ�����������ƽ�� ����ƽ�洴�������������н�Զƽ��ֱ�Ϊ-1 �� 1 ��Ȼ��view����Ĭ��Ϊ��λ����
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
	/// ʹ��glm::ortho������������ͬʱ����viewprojection
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
	/// �������pos��rotate����view(��˺�ȡ����� �Լ� ����vp����
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