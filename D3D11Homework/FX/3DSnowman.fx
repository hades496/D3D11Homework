//--------------------------------------------------------------------------------------
// File: 3Dsnowman.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : Normal;
	float2 Tex : TEXCOORD1;
};

float3 Normalization(float3 norm)
{
	float3 out_norm = norm;
	float d = length(norm);
	out_norm /= d;
	return out_norm;
}



//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------


cbuffer ConstantBuffer: register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
	float4 vLightDir;
	float4 vLightColor;
}

