#pragma once
#include <string>
#include <functional>
#include <map>
#include "RPG2D/Core/LogSystem.h"
namespace RPG2D {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,DIYEvent
	};


	//����ʵ��event��������ĺ������������ܣ��ֱ𷵻��¼����͡�
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	class Event
	{
	public:
		virtual ~Event() = default;
		bool Handled = false;//�¼��Ƿ񱻴�����
		virtual EventType GetEventType() const = 0;//��ȡ�¼�����
		virtual const char* GetName() const = 0;//��ȡ�¼�����
		//toString ������
		virtual std::string ToString() const { return GetName(); }
	};

	//�¼�������
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		//�����¼����ڴ����¼�ʱ���ں�������д��Ӧ��dispatch<Event>(Func),�Ϳ���ָ���¼��������Ӧ�¼��������Ķ�Ӧ��ϵ��
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));//m_Event������ΪEvent���͵����ã����ｫ��ת��Ϊ��ʵ������T������
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	//������������¼���Ϣ
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	//�¼�ϵͳ����
	// �Զ��� EventType �Ĺ�ϣ����
	struct EventTypeHash {
		std::size_t operator()(const EventType& eventType) const {
			// �� EventType ת��Ϊ��ײ����͵�������ϣֵ
			return static_cast<std::size_t>(eventType);
		}
	};

	// �¼������������Ͷ���
	using EventHandler = std::function<bool(Event*)>;

	// �¼�����ϵͳ��
	class EventSystem {
	public:
		// ע���¼�������
		void RegisterEventHandler(EventType eventType, EventHandler handler) {
			eventHandlers[eventType].push_back(handler);
		}

		/*
		
		// �Ƴ��¼�������
		void UnregisterEventHandler(EventType eventType, EventHandler handler) {
			// ����¼������Ƿ���ע��
			auto it = eventHandlers.find(eventType);
			if (it != eventHandlers.end()) {
				// �����¼����������Ƴ�
				auto& handlers = it->second;
				handlers.erase(std::remove(handlers.begin(), handlers.end(), handler), handlers.end());
			}
		}
		*/

		// �����¼��������ε����¼�������
		void TriggerEvent(EventType eventType, Event* event) {
			//RPG2D_CORE_INFO("EventSytemDealEvent");
			// ����¼������Ƿ���ע��
			auto it = eventHandlers.find(eventType);
			if (it != eventHandlers.end()) {
				// ��������¼�������ص������¼�������
				for (const auto& handler : it->second) {
					//ֱ�ӻ�ȡhanlder�����á�
					handler(event);
				}
			}
		}

	private:
		// ʹ�� unordered_map �洢�¼��������¼�������֮���ӳ���ϵ
		std::map<EventType, std::vector<EventHandler>> eventHandlers;
	};
}

