#pragma once
#include "Event.h"
namespace RPG2D {
    // �����¼�������������
    //using EventHandler = std::function<bool(Event*)>;
	class EventSystem
	{
     /*
      
    public:
		//�¼�����ϵͳ
        //ע���¼�������
        void RegisterEventHandler(EventType eventType, EventHandler handler) {
            eventHandlers[eventType].push_back(handler);
        }

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

        // �����¼��������ε����¼�������
        void TriggerEvent(EventType eventType, Event* event) {
            // ����¼������Ƿ���ע��
            if (eventHandlers.find(eventType) != eventHandlers.end()) {
                // ��������¼�������ص������¼�������
                for (const auto& handler : eventHandlers[eventType]) {
                    handler(event);
                }
            }
        }

    private:
        // ʹ��unordered_map�洢�¼��������¼�������֮���ӳ���ϵ
        std::unordered_map<EventType, std::vector<EventHandler>> eventHandlers;
      
     */
	};
}

