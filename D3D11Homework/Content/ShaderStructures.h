﻿#pragma once

namespace D3D11Homework
{
	// 用于向顶点着色器发送 MVP 矩阵的常量缓冲区。
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	// 用于向顶点着色器发送每个顶点的数据Position,Color,Normal,Texure。
	struct VertexPCNT
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texure;
	};
}