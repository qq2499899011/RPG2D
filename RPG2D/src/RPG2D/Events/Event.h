#pragma once
#include <string>
namespace RPG2D {

	// Events in RPG2D are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0), //1
		EventCategoryInput = BIT(1), //2
		EventCategoryKeyboard = BIT(2), //4
		EventCategoryMouse = BIT(3), //8
		EventCategoryMouseButton = BIT(4)  //16
	};

	//����ʵ��event��������ĺ����������������ɶ�����Ҫ ������д��Щ������
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

//ͬ���棬�������������������Ӧ�ĺ���
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;//�¼��Ƿ񱻴�����

		virtual EventType GetEventType() const = 0;//�¼�����
		virtual const char* GetName() const = 0;//ʱ������-���͵��ַ�����ʽ
		virtual int GetCategoryFlags() const = 0;//�¼����

		//toString ������
		virtual std::string ToString() const { return GetName(); }

		//�ж�ʱ���Ƿ�����������͡����ٶ�ʱ����з���
		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// F will be deduced by the compiler
		//T���¼����ͣ�F�Ǵ������غ�����//dispatch ��ʵ�����ж��¼��Ƿ����ָ�����¼����ͣ���������¼����ͣ��Ͷ��¼����д�������ʽ��F������
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

}

