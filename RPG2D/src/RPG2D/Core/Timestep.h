#pragma once
//计时
namespace RPG2D {
	class Timestep 
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		//重载为float对象。
		operator float() const { return m_Time; }
		bool operator<(const Timestep& other) const {
			return m_Time < other.GetSeconds();
		}
		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}
