#pragma once
#include"RPG2D/Core/Application.h"
#include"RPG2D/Core/LogSystem.h"
//�������
//��Ҫ�����ⲿ�ĺ���
#ifdef RPG2D_PLATFORM_WINDOWS
//�����ⲿʵ��CreatApplication
extern RPG2D::Application* RPG2D::CreateApplication();
int main(int argc,char** argv) {
	//������־,��ʼ�� 
	RPG2D::Log::Init();
	RPG2D_CORE_INFO("Hello");
	RPG2D_WARN("client hello");
	//����app������
	auto app = RPG2D::CreateApplication();
	RPG2D_WARN("www");
	//��ʼ����ִ��
	app->Init();
	app->Run();
	delete app;
	RPG2D_WARN("www1111111222");
}

#endif // RPG2D_PLATFORM_WINDOWS
