#include "pch.h"
#include "SkyBox.h"

#include "..\Common\DirectXHelper.h"



using namespace D3D11Homework;

using namespace DirectX;
using namespace Windows::Foundation;


SkyBox::SkyBox(const std::shared_ptr<DX::DeviceResources>& deviceResources, float size) :
	m_deviceResources(deviceResources),
	m_boxSize(size)
{
	m_east = std::unique_ptr<Plane>(new Plane(m_deviceResources, false, XMFLOAT3(m_boxSize, m_boxSize, m_boxSize), XMFLOAT3(m_boxSize / 2.0f, 0.0f, 0.0f), 1));
	m_south = std::unique_ptr<Plane>(new Plane(m_deviceResources, false, XMFLOAT3(m_boxSize, m_boxSize, m_boxSize), XMFLOAT3(0.0f, 0.0f, m_boxSize / 2.0f), 2));
	m_west = std::unique_ptr<Plane>(new Plane(m_deviceResources, false, XMFLOAT3(m_boxSize, m_boxSize, m_boxSize), XMFLOAT3(-m_boxSize / 2.0f, 0.0f, 0.0f), 3));
	m_north = std::unique_ptr<Plane>(new Plane(m_deviceResources, false, XMFLOAT3(m_boxSize, m_boxSize, m_boxSize), XMFLOAT3(0.0f, 0.0f, -m_boxSize / 2.0f), 4));
	m_up = std::unique_ptr<Plane>(new Plane(m_deviceResources, false, XMFLOAT3(m_boxSize, m_boxSize, m_boxSize), XMFLOAT3(0.0f, m_boxSize / 2.0f, 0.0f), 5));
}

// 每帧调用一次，更新视图
void SkyBox::Update(DX::StepTimer const& timer, DirectX::XMFLOAT3 cameraPos)
{
	m_east->Update(timer, cameraPos);
	m_south->Update(timer, cameraPos);
	m_west->Update(timer, cameraPos);
	m_north->Update(timer, cameraPos);
	m_up->Update(timer, cameraPos);
	
}

void SkyBox::ChangeViewMatrix(const DirectX::XMMATRIX viewMatrix)
{
	XMStoreFloat4x4(&m_east->m_constantBufferData.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_south->m_constantBufferData.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_west->m_constantBufferData.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_north->m_constantBufferData.view, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_up->m_constantBufferData.view, XMMatrixTranspose(viewMatrix));
}

void SkyBox::ChangeModelMatrix(const DirectX::XMMATRIX viewMatrix)
{
	XMStoreFloat4x4(&m_east->m_constantBufferData.model, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_south->m_constantBufferData.model, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_west->m_constantBufferData.model, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_north->m_constantBufferData.model, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&m_up->m_constantBufferData.model, XMMatrixTranspose(viewMatrix));
}

void SkyBox::CreateWindowSizeDependentResources()
{
	m_east->CreateWindowSizeDependentResources();
	m_south->CreateWindowSizeDependentResources();
	m_west->CreateWindowSizeDependentResources();
	m_north->CreateWindowSizeDependentResources();
	m_up->CreateWindowSizeDependentResources();
}

// 使用顶点和像素着色器呈现一个帧。
void SkyBox::Render()
{
	m_east->Render();
	m_south->Render();
	m_west->Render();
	m_north->Render();
	m_up->Render();
}


// 通知呈现器，需要释放设备资源。
void SkyBox::ReleaseDeviceDependentResources()
{
	m_east->ReleaseDeviceDependentResources();
	m_south->ReleaseDeviceDependentResources();
	m_west->ReleaseDeviceDependentResources();
	m_north->ReleaseDeviceDependentResources();
	m_up->ReleaseDeviceDependentResources();
}

// 通知呈现器，现在可重新创建设备资源。
void SkyBox::CreateDeviceDependentResources()
{
	m_east->CreateDeviceDependentResources();
	m_south->CreateDeviceDependentResources();
	m_west->CreateDeviceDependentResources();
	m_north->CreateDeviceDependentResources();
	m_up->CreateDeviceDependentResources();
}
