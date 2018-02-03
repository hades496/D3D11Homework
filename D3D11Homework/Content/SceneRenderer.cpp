#include "pch.h"
#include "SceneRenderer.h"

#include "..\Common\DirectXHelper.h"

using namespace D3D11Homework;

using namespace DirectX;
using namespace Windows::Foundation;
using namespace std;

// 场景初始化
SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{

}

SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, void* start()) :
	m_deviceResources(deviceResources)
{
	start();
}

//在场景中添加模型
void SceneRenderer::Add(std::unique_ptr<Model> model)
{
	m_modelManager.push_back(std::move(model));
}

// 每帧调用一次，更新视图
void SceneRenderer::Update(DX::StepTimer const& timer)
{
	for (auto &model : m_modelManager)
	{
		model->Update(timer);
	}
}

// 
void SceneRenderer::CreateWindowSizeDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model->CreateWindowSizeDependentResources();
	}
}

// 使用顶点和像素着色器呈现一个帧。
void SceneRenderer::Render()
{
	for (auto &model : m_modelManager)
	{
		model->Render();
	}
}


// 通知呈现器，需要释放设备资源。
void SceneRenderer::ReleaseDeviceDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model->ReleaseDeviceDependentResources();
	}
}

// 通知呈现器，现在可重新创建设备资源。
void SceneRenderer::CreateDeviceDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model->CreateDeviceDependentResources();
	}
}

