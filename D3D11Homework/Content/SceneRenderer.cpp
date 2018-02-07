#include "pch.h"
#include "SceneRenderer.h"

#include "..\Common\DirectXHelper.h"
#include <dinput.h>

using namespace D3D11Homework;
using namespace DirectX;
using namespace Windows;
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
	// 初始化视角
	XMVECTORF32 eye = { 5 , 3.7f, 0 , 0.0f };
	XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_modelManager.back()->ChangeViewMatrix(eye, at, up);
}

// 每帧调用一次，更新视图
void SceneRenderer::Update(DX::StepTimer const& timer)
{
	float radiansPerSecond = XMConvertToRadians(30);
	double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
	float radians = static_cast<float>(fmod(totalRotation, XM_2PI));
	XMVECTORF32 eye = { 8 * cos(radians), 2.0f, 8 * sin(radians), 0.0f };
	XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	for (auto &model : m_modelManager)
	{	
		model->ChangeViewMatrix(eye, at, up);
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

