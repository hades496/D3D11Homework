#include <3DSnowman.fx>

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0)
{
	Filter = MIN_MAG_MIP_LINEAR;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColor = 0;
	float4 nor_Light = float4(Normalization(vLightDir.xyz),1);
	float4 nor_Norm = float4(Normalization(input.Norm),1);
	finalColor += saturate(dot(nor_Light, nor_Norm) * vLightColor);
	finalColor.a = 1;

	return txDiffuse.Sample(samLinear, input.Tex) * finalColor;
	//return finalColor;
}
