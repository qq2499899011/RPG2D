#include "RPG2Dpch.h"
#include "GlobalContext.h"
namespace RPG2D {
	Ref<GlobalContext> GlobalContext::Instance = nullptr;
	GlobalContext::GlobalContext()
	{
	}

	Ref<GlobalContext> GlobalContext::GetInstace()
	{
		if (!Instance)Create();
		return Instance;
	}

	void GlobalContext::Create()
	{
		//����Instance
		Instance = CreateRef<GlobalContext>();
	}

	void RPG2D::GlobalContext::Init()
	{
		//��ʼ��
	}
}
