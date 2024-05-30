#pragma once

#include "RPG2D/Function/Renderer/OrthographicCamera.h"
#include "RPG2D/Core/Timestep.h"
#include "RPG2D/Events/ApplicationEvent.h"
#include "RPG2D/Events/MouseEvent.h"
#include "RPG2D/Resource/ResType/Entity.h"
namespace RPG2D {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float,float,bool);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void OnResize(float width, float height);
		void SetLookAtX(Entity);
		void SetLookAtY(Entity);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void LookAtX();//设置X轴中心跟随物体
		void LookAtY();//设置Y轴中心跟随物体
	private:
		//相机
		OrthographicCamera m_Camera;
		//是否可以旋转
		bool m_Rotation;
		bool lockX = false;
		bool lockY = false;
		Entity m_LookAt;
		//相机位置
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		//相机速度
		float m_CameraTranslationSpeed = 500.0f, m_CameraRotationSpeed = 180.0f;
		//记录当前屏幕的长宽
		float m_ScreenWidth = 1600, m_ScreenHeight = 900;
		float boardLeft = 0.0f;//x不小于这个值
	};

}
