#include "pch.h"
#include "SceneRenderer.h"

#include "..\Common\DirectXHelper.h"

using namespace D3D11Homework;

using namespace DirectX;
using namespace Windows::Foundation;
using namespace std;

// ������ʼ��
SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{

}

SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, void* start()) :
	m_deviceResources(deviceResources)
{
	start();
}

//�ڳ��������ģ��
void SceneRenderer::Add(std::unique_ptr<Model> model)
{
	m_modelManager.push_back(std::move(model));
}

// ÿ֡����һ�Σ�������ͼ
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

// ʹ�ö����������ɫ������һ��֡��
void SceneRenderer::Render()
{
	for (auto &model : m_modelManager)
	{
		model->Render();
	}
}


// ֪ͨ����������Ҫ�ͷ��豸��Դ��
void SceneRenderer::ReleaseDeviceDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model->ReleaseDeviceDependentResources();
	}
}

// ֪ͨ�����������ڿ����´����豸��Դ��
void SceneRenderer::CreateDeviceDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model->CreateDeviceDependentResources();
	}
}

