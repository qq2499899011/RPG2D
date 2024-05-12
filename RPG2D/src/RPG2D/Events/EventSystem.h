#pragma once
#include "Event.h"
namespace RPG2D {
    // 定义事件处理函数的类型
    //using EventHandler = std::function<bool(Event*)>;
	class EventSystem
	{
     /*
      
    public:
		//事件处理系统
        //注册事件处理函数
        void RegisterEventHandler(EventType eventType, EventHandler handler) {
            eventHandlers[eventType].push_back(handler);
        }

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

        // 触发事件，并依次调用事件处理函数
        void TriggerEvent(EventType eventType, Event* event) {
            // 检查事件类型是否已注册
            if (eventHandlers.find(eventType) != eventHandlers.end()) {
                // 调用与该事件类型相关的所有事件处理函数
                for (const auto& handler : eventHandlers[eventType]) {
                    handler(event);
                }
            }
        }

    private:
        // 使用unordered_map存储事件类型与事件处理函数之间的映射关系
        std::unordered_map<EventType, std::vector<EventHandler>> eventHandlers;
      
     */
	};
}

