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

void Model::ChangeViewMatrix(const DirectX::XMVECTORF32 eye, const DirectX::XMVECTORF32 at, const DirectX::XMVECTORF32 up)
{
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}
