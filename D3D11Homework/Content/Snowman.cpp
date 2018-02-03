#include "pch.h"
#include "Snowman.h"

#include "..\Common\DirectXHelper.h"



using namespace D3D11Homework;

using namespace DirectX;
using namespace Windows::Foundation;

// ���ļ��м��ض����������ɫ����Ȼ��ʵ���������弸��ͼ�Ρ�
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

// ÿ֡����һ�Σ�������ͼ
void Snowman::Update(DX::StepTimer const& timer)
{
	m_LowerSphere->Update(timer);
	m_UpperSphere->Update(timer);
}

void Snowman::CreateWindowSizeDependentResources()
{
	m_LowerSphere->CreateWindowSizeDependentResources();
	m_UpperSphere->CreateWindowSizeDependentResources();
}

// ʹ�ö����������ɫ������һ��֡��
void Snowman::Render()
{
	m_LowerSphere->Render();
	m_UpperSphere->Render();
}


// ֪ͨ����������Ҫ�ͷ��豸��Դ��
void Snowman::ReleaseDeviceDependentResources()
{
	m_LowerSphere->ReleaseDeviceDependentResources();
	m_UpperSphere->ReleaseDeviceDependentResources();
}

// ֪ͨ�����������ڿ����´����豸��Դ��
void Snowman::CreateDeviceDependentResources()
{
	m_LowerSphere->CreateDeviceDependentResources();
	m_UpperSphere->CreateDeviceDependentResources();
}
