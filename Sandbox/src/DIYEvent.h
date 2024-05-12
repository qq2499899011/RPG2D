#pragma once
#include<RPG2D.h>
using namespace RPG2D;
//自定义新的事件类型
class DIYEvent:public Event
{
	DIYEvent(int value) :eventValue(value) {};
	EVENT_CLASS_TYPE(DIYEvent)
	int eventValue;
};

