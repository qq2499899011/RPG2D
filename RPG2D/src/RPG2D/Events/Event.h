#pragma once
#include <string>
namespace RPG2D {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,DIYEvent
	};


	//用于实现event派生子类的函数，三个功能，分别返回事件类型、
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	class Event
	{
	public:
		virtual ~Event() = default;
		bool Handled = false;//事件是否被处理了
		virtual EventType GetEventType() const = 0;//获取事件类型
		virtual const char* GetName() const = 0;//获取事件名称
		//toString 调试用
		virtual std::string ToString() const { return GetName(); }
	};

	//事件处理器
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		//处理事件，在处理事件时，在函数中书写相应的dispatch<Event>(Func),就可以指定事件类型与对应事件处理函数的对应关系。
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

