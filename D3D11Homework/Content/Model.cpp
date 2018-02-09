#include "pch.h"
#include "Model.h"


using namespace D3D11Homework;

Model::Model()
{
}

Model::~Model()
{
}

// 每帧调用一次，更新视图
void Model::Update(DX::StepTimer const& timer)
{

}

void Model::Update(DX::StepTimer const& timer, DirectX::XMFLOAT3)
{

}



void Model::CreateDeviceDependentResources()
{
}

// 使用顶点和像素着色器呈现一个帧。
void Model::Render()
{
}


void Model::ReleaseDeviceDependentResources()
{

}

void Model::CreateWindowSizeDependentResources()
{

}

void Model::ChangeViewMatrix(const DirectX::XMMATRIX viewMatrix)
{
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(viewMatrix));
}
void Model::ChangeModelMatrix(const DirectX::XMMATRIX viewMatrix)
{
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(viewMatrix));
}
