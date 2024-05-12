#include "RPG2Dpch.h"
#include "Animatior.h"
namespace RPG2D {
	Ref<Texture2D> Animatior::GetCurFrame()
	{
		return curTexture;
	}
    // �������
    void Animatior::AddFrame(Timestep ts, Ref<Texture2D> texture){
        //��Ҫ��¼���ʱ�䡣
        if (ts > maxTime-0.1f)maxTime = ts+0.1f;
        frames[ts] = texture;
    }
    // ����ʱ��
    void Animatior::Update(Timestep deltaTime) {
        //����ʱ��
        currentTime = currentTime + deltaTime;
        if (currentTime > maxTime)currentTime = currentTime - maxTime;
        //��������ʱ��ȷ���������
        auto it = frames.upper_bound(currentTime); // ���Ҵ��ڵ�ǰʱ��ĵ�һ��Ԫ�صĵ�����
        if (it != frames.begin())--it; // ���������˻ص�С�ڵ��ڵ�ǰʱ������ֵ
        curTexture = it->second;
    }
    //����ʱ��
    void Animatior::Reset() 
    {
        currentTime = 0;
    }
}
