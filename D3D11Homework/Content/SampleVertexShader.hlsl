#include "..\FX\LightHelper.fx"

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// ת��������ռ�
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// ת������μ��ÿռ�
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	return vout;
}