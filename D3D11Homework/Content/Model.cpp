#include "pch.h"
#include "Model.h"


using namespace D3D11Homework;

Model::Model()
{
}

Model::~Model()
{
}

// ÿ֡����һ�Σ�������ͼ
void Model::Update(DX::StepTimer const& timer)
{

}

void Model::CreateDeviceDependentResources()
{
}

// ʹ�ö����������ɫ������һ��֡��
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
