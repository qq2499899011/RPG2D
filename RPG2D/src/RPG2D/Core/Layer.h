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

		virtual void OnAttach() {} //加入layerStack进行渲染时
		virtual void OnDetach() {}	//从LayerStack移除时
		virtual void OnUpdate(Timestep ts) {}	//app run时调用，根据传入的时间 更新渲染
		virtual void OnImGuiRender() {} //待定
		virtual void OnEvent(Event& event) {} //layer处理时间，从application::on_event里进行调用

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}