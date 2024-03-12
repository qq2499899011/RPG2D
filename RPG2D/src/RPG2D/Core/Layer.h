#pragma once

#include "RPG2D/Core/Base.h"
#include "RPG2D/Events/Event.h"
#include "RPG2D/Core/Timestep.h"
namespace RPG2D {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {} //����layerStack������Ⱦʱ
		virtual void OnDetach() {}	//��LayerStack�Ƴ�ʱ
		virtual void OnUpdate(Timestep ts) {}	//app runʱ���ã����ݴ����ʱ�� ������Ⱦ
		virtual void OnImGuiRender() {} //����
		virtual void OnEvent(Event& event) {} //layer����ʱ�䣬��application::on_event����е���

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}