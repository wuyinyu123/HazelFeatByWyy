#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel
{
	LayerStack::LayerStack()
	{
		mLayerInsert = mLayers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers)
		{
			delete layer;
		}
	}

	//mLayerInsert迭代器始终指向最新插入的元素
	void LayerStack::PushLayer(Layer* layer)
	{
		mLayerInsert = mLayers.emplace(mLayerInsert, layer);
	}

	//添加
	void LayerStack::PushOverlay(Layer* overlay)
	{
		//在末端插入元素。原地构造，避免拷贝
		mLayers.emplace_back(overlay);
	}

	//移除并改变mLayerInsert迭代器位置
	void LayerStack::PopLayer(Layer* layer)
	{
		//遍历查找layer，若找到则返回迭代器指针
		auto it = std::find(mLayers.begin(), mLayers.end(), layer);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
			mLayerInsert--; 
		}
	}

	//移除
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
		}
	}
}