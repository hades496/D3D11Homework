#include "..\FX\LightHelper.fx"

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// 转换到世界空间
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// 转换到齐次剪裁空间
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	return vout;
}