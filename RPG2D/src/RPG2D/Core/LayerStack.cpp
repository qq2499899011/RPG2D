#include "RPG2Dpch.h"
#include "RPG2D/Core/LayerStack.h"

namespace RPG2D {

	LayerStack::~LayerStack()
	{
		//��ʼ��layers
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}
	/// <summary>
	/// ���뵽layer��
	/// </summary>
	/// <param name="layer"></param>
	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}
	/// <summary>
	/// ���ȼ�����
	/// </summary>
	/// <param name="overlay"></param>
	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}

}
