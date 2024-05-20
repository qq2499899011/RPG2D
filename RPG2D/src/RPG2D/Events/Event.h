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

	//事件系统初试
	// 自定义 EventType 的哈希函数
	struct EventTypeHash {
		std::size_t operator()(const EventType& eventType) const {
			// 将 EventType 转换为其底层类型的整数哈希值
			return static_cast<std::size_t>(eventType);
		}
	};

	// 事件处理函数的类型定义
	using EventHandler = std::function<bool(Event*)>;

	// 事件处理系统类
	class EventSystem {
	public:
		// 注册事件处理函数
		void RegisterEventHandler(EventType eventType, EventHandler handler) {
			eventHandlers[eventType].push_back(handler);
		}

		/*
		
		// 移除事件处理函数
		void UnregisterEventHandler(EventType eventType, EventHandler handler) {
			// 检查事件类型是否已注册
			auto it = eventHandlers.find(eventType);
			if (it != eventHandlers.end()) {
				// 查找事件处理函数并移除
				auto& handlers = it->second;
				handlers.erase(std::remove(handlers.begin(), handlers.end(), handler), handlers.end());
			}
		}
		*/

		// 触发事件，并依次调用事件处理函数
		void TriggerEvent(EventType eventType, Event* event) {
			//RPG2D_CORE_INFO("EventSytemDealEvent");
			// 检查事件类型是否已注册
			auto it = eventHandlers.find(eventType);
			if (it != eventHandlers.end()) {
				// 调用与该事件类型相关的所有事件处理函数
				for (const auto& handler : it->second) {
					//直接获取hanlder并调用。
					handler(event);
				}
			}
		}

	private:
		// 使用 unordered_map 存储事件类型与事件处理函数之间的映射关系
		std::map<EventType, std::vector<EventHandler>> eventHandlers;
	};
}

