﻿#include "pch.h"
#include "D3D11HomeworkMain.h"
#include "Common\DirectXHelper.h"

using namespace D3D11Homework;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace DirectX;
using namespace std;

// 加载应用程序时加载并初始化应用程序资产。
D3D11HomeworkMain::D3D11HomeworkMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// 注册以在设备丢失或重新创建时收到通知
	m_deviceResources->RegisterDeviceNotify(this);
	
	
	m_fpsTextRenderer = unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// 创建场景管理器
	m_sceneRenderer = unique_ptr<SceneRenderer>(new SceneRenderer(m_deviceResources));

	//添加一个正方体盒
	m_sceneRenderer->Add("box", unique_ptr<Model>(new CubeBox(m_deviceResources, true, XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(3.0f, 0.5f, 3.0f))));

	//添加地面
	m_sceneRenderer->Add(
		"plane",
		unique_ptr<Model>(
			new Plane(
				m_deviceResources, 
				false, 
				XMFLOAT3(1000.0f, 1000.0f, 1000.0f), 
				XMFLOAT3(0.0f, 0.0f, 0.0f),
				0
			)
		)
	);
	// 创建天空盒
	m_sceneRenderer->Add(
		"SkyBox",
		unique_ptr<Model>(
			new SkyBox(
				m_deviceResources,
				100.0f
			)
		)
	);

	//添加两个雪人
	m_sceneRenderer->Add("snowman", unique_ptr<Model>(new Snowman(m_deviceResources, true, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(3.0f, 1.5f, 3.0f))));
	m_sceneRenderer->Add("snowman", unique_ptr<Model>(new Snowman(m_deviceResources, false, XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f))));



	// TODO: 如果需要默认的可变时间步长模式之外的其他模式，请更改计时器设置。
	// 例如，对于 60 FPS 固定时间步长更新逻辑，请调用:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

D3D11HomeworkMain::~D3D11HomeworkMain()
{
	// 取消注册设备通知
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// 在窗口大小更改(例如，设备方向更改)时更新应用程序状态
void D3D11HomeworkMain::CreateWindowSizeDependentResources() 
{
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

void D3D11HomeworkMain::Start()
{
	
}

// 每帧更新一次应用程序状态。
void D3D11HomeworkMain::Update() 
{


	// 更新场景对象。
	m_timer.Tick([&]()
	{
		// 更新文本
		m_fpsTextRenderer->Update(m_timer);
		// 更新场景模型
		m_sceneRenderer->Update(m_timer);
		
	});
}

// 根据当前应用程序状态呈现当前帧。
// 如果帧已呈现并且已准备好显示，则返回 true。
bool D3D11HomeworkMain::Render() 
{
	// 在首次更新前，请勿尝试呈现任何内容。
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// 将视区重置为针对整个屏幕。
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// 将呈现目标重置为屏幕。
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// 清除后台缓冲区和深度模具视图。
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// 呈现场景对象。
	
	m_sceneRenderer->Render();
	m_fpsTextRenderer->Render();
	
	return true;
}

// 通知呈现器，需要释放设备资源。
void D3D11HomeworkMain::OnDeviceLost()
{
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
	m_sceneRenderer->ReleaseDeviceDependentResources();
}

// 通知呈现器，现在可重新创建设备资源。
void D3D11HomeworkMain::OnDeviceRestored()
{
	m_fpsTextRenderer->CreateDeviceDependentResources();
	m_sceneRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
