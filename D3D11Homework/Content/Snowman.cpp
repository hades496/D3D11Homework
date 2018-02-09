#include "pch.h"
#include "Snowman.h"

#include "..\Common\DirectXHelper.h"



using namespace D3D11Homework;

using namespace DirectX;
using namespace Windows::Foundation;


Snowman::Snowman(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	m_LowerSphere = std::unique_ptr<SphereRenderer>(new SphereRenderer(m_deviceResources, false, XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 1.25f, 0.0f)));
	m_UpperSphere = std::unique_ptr<SphereRenderer>(new SphereRenderer(m_deviceResources, false, XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
}

Snowman::Snowman(const std::shared_ptr<DX::DeviceResources>& deviceResources, const bool rotatable, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 transform) :
	m_deviceResources(deviceResources),
	m_rotatable(rotatable)
{
	m_LowerSphere = std::unique_ptr<SphereRenderer>(new SphereRenderer(m_deviceResources, rotatable, XMFLOAT3(0.5f * scale.x, 0.5f * scale.y, 0.5f * scale.z), XMFLOAT3(0.0f + transform.x, 1.25f * scale.y + transform.y, 0.0f + transform.z)));
	m_UpperSphere = std::unique_ptr<SphereRenderer>(new SphereRenderer(m_deviceResources, rotatable, XMFLOAT3(scale.x, scale.y, scale.z), XMFLOAT3( transform.x, transform.y, transform.z)));
}

// 每帧调用一次，更新视图
void Snowman::Update(DX::StepTimer const& timer)
{
	m_LowerSphere->Update(timer);
	m_UpperSphere->Update(timer);
}

void Snowman::ChangeViewMatrix(const DirectX::XMVECTORF32 eye, const DirectX::XMVECTORF32 at, const DirectX::XMVECTORF32 up)
{	
	
	XMStoreFloat4x4(&m_LowerSphere->m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat4x4(&m_UpperSphere->m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

void Snowman::ChangeViewMatrix(const DirectX::XMMATRIX viewMatrix)
{
	XMStoreFloat4x4(&m_LowerSphere->m_constantBufferData.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_UpperSphere->m_constantBufferData.view, XMMatrixTranspose(viewMatrix));
}

void Snowman::CreateWindowSizeDependentResources()
{
	m_LowerSphere->CreateWindowSizeDependentResources();
	m_UpperSphere->CreateWindowSizeDependentResources();
}

// 使用顶点和像素着色器呈现一个帧。
void Snowman::Render()
{
	m_LowerSphere->Render();
	m_UpperSphere->Render();
}


// 通知呈现器，需要释放设备资源。
void Snowman::ReleaseDeviceDependentResources()
{
	m_LowerSphere->ReleaseDeviceDependentResources();
	m_UpperSphere->ReleaseDeviceDependentResources();
}

// 通知呈现器，现在可重新创建设备资源。
void Snowman::CreateDeviceDependentResources()
{
	m_LowerSphere->CreateDeviceDependentResources();
	m_UpperSphere->CreateDeviceDependentResources();
}
