#pragma once
#include "RPG2D/Core/Layer.h"
#include "RPG2D/Events/Event.h"
#include "RPG2D/Events/ApplicationEvent.h"
#include "RPG2D/Events/KeyEvent.h"
#include "RPG2D/Events/MouseEvent.h"
namespace RPG2D {
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;
		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
	};
}