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

	//用于实现event派生子类的函数，根据类型生成而不需要 不断重写这些函数。
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

//同上面，根据输入的类型生成相应的函数
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;//事件是否被处理了

		virtual EventType GetEventType() const = 0;//事件类型
		virtual const char* GetName() const = 0;//时间名称-类型的字符串形式
		virtual int GetCategoryFlags() const = 0;//事件类别

		//toString 调试用
		virtual std::string ToString() const { return GetName(); }

		//判断时间是否属于这个类型。快速对时间进行分类
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
		//T是事件类型，F是处理的相关函数。//dispatch 其实就是判断事件是否符合指定的事件类型，如果满足事件类型，就对事件进行处理。处理方式由F决定。
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));//m_Event被定义为Event类型的引用，这里将其转化为对实际类型T的引用
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	//重载运算符。事件信息
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}

