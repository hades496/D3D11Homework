#include "pch.h"
#include "SceneRenderer.h"

#include "..\Common\DirectXHelper.h"

using namespace D3D11Homework;
using namespace DirectX;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace std;

// ������ʼ��
SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)	
{
	// ע���¼����
	m_moveLookController = ref new MoveLookController;
	m_moveLookController->Initialize(deviceResources->m_window.Get());
	
}

//�ڳ��������ģ��
void SceneRenderer::Add(string type, unique_ptr<Model> model)
{
	// ��ʼ��ģ���ӽ�
	m_moveLookController->SetPosition(XMFLOAT3(0.0, 3.7f, 8.0f));
	m_moveLookController->SetOrientation(0.0f, XM_PI);

	XMMATRIX mat = m_moveLookController->get_ViewMatrix();
	model->ChangeViewMatrix(mat);

	// �ڹ����������ģ��
	m_modelManager.push_back(ModelStruct(move(model),type));
}

// ÿ֡����һ�Σ�������ͼ
void SceneRenderer::Update(DX::StepTimer const& timer)
{
	XMMATRIX viewMatrix = XMMatrixIdentity();
	for (auto &model : m_modelManager) // �������λ��
	{
		if (model.type == "box") // �ж�����Ƿ��������
		{
			if (m_moveLookController->OnBox())
			{
				XMFLOAT4X4 bM = model.model->m_constantBufferData.model;	//��ȡ������ģ�ͱ任����
				XMFLOAT3 bP(bM._14, bM._24 + 3.0f, bM._34);		//�������λ��
				m_moveLookController->SetPosition(bP);
				break;
			}
		}
	}
	m_moveLookController->Update(m_deviceResources->m_window.Get());
	viewMatrix *= m_moveLookController->get_ViewMatrix();
	for (auto &model : m_modelManager) // ����ģ��λ��
	{
		if (model.type == "SkyBox")	//��պи�������ƶ�
		{
			XMFLOAT3 cameraPos = m_moveLookController->get_Position();
			model.model->Update(timer, cameraPos);
		}
		else
		{
			model.model->Update(timer);
		}
	}



	for (auto &model : m_modelManager) //�������λ��
	{
		model.model->ChangeViewMatrix(viewMatrix);
	}
}

// 
void SceneRenderer::CreateWindowSizeDependentResources()
{
	
	for (auto &model : m_modelManager)
	{
		
		model.model->CreateWindowSizeDependentResources();
	}
}

// ʹ�ö����������ɫ������һ��֡��
void SceneRenderer::Render()
{
	for (auto &model : m_modelManager)
	{
		model.model->Render();
	}
}


// ֪ͨ����������Ҫ�ͷ��豸��Դ��
void SceneRenderer::ReleaseDeviceDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model.model->ReleaseDeviceDependentResources();
	}
}

// ֪ͨ�����������ڿ����´����豸��Դ��
void SceneRenderer::CreateDeviceDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model.model->CreateDeviceDependentResources();
	}
}

