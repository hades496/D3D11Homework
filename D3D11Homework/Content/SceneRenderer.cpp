#include "pch.h"
#include "SceneRenderer.h"

#include "..\Common\DirectXHelper.h"

using namespace D3D11Homework;
using namespace DirectX;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace std;

// 场景初始化
SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)	
{
	// 注册事件监控
	m_moveLookController = ref new MoveLookController;
	m_moveLookController->Initialize(deviceResources->m_window.Get());
	
}

//在场景中添加模型
void SceneRenderer::Add(string type, unique_ptr<Model> model)
{
	// 初始化模型视角
	m_moveLookController->SetPosition(XMFLOAT3(0.0, 3.7f, 8.0f));
	m_moveLookController->SetOrientation(0.0f, XM_PI);

	XMMATRIX mat = m_moveLookController->get_ViewMatrix();
	model->ChangeViewMatrix(mat);

	// 在管理器中添加模型
	m_modelManager.push_back(ModelStruct(move(model),type));
}

// 每帧调用一次，更新视图
void SceneRenderer::Update(DX::StepTimer const& timer)
{
	XMMATRIX viewMatrix = XMMatrixIdentity();
	for (auto &model : m_modelManager) // 计算相机位置
	{
		if (model.type == "box") // 判断相机是否绑定立方体
		{
			if (m_moveLookController->OnBox())
			{
				XMFLOAT4X4 bM = model.model->m_constantBufferData.model;	//获取立方体模型变换矩阵
				XMFLOAT3 bP(bM._14, bM._24 + 3.0f, bM._34);		//计算相机位置
				m_moveLookController->SetPosition(bP);
				break;
			}
		}
	}
	m_moveLookController->Update(m_deviceResources->m_window.Get());
	viewMatrix *= m_moveLookController->get_ViewMatrix();
	for (auto &model : m_modelManager) // 更新模型位置
	{
		if (model.type == "SkyBox")	//天空盒跟随相机移动
		{
			XMFLOAT3 cameraPos = m_moveLookController->get_Position();
			model.model->Update(timer, cameraPos);
		}
		else
		{
			model.model->Update(timer);
		}
	}



	for (auto &model : m_modelManager) //更新相机位置
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

// 使用顶点和像素着色器呈现一个帧。
void SceneRenderer::Render()
{
	for (auto &model : m_modelManager)
	{
		model.model->Render();
	}
}


// 通知呈现器，需要释放设备资源。
void SceneRenderer::ReleaseDeviceDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model.model->ReleaseDeviceDependentResources();
	}
}

// 通知呈现器，现在可重新创建设备资源。
void SceneRenderer::CreateDeviceDependentResources()
{
	for (auto &model : m_modelManager)
	{
		model.model->CreateDeviceDependentResources();
	}
}

