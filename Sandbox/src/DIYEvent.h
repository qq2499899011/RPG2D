#pragma once
#include<RPG2D.h>
using namespace RPG2D;
//�Զ����µ��¼�����
class DIYEvent:public Event
{
	DIYEvent(int value) :eventValue(value) {};
	EVENT_CLASS_TYPE(DIYEvent)
	int eventValue;
};

