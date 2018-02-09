#include <3DSnowman.fx>

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = input.Pos;
	output.Pos.w = 1;
	output.Pos = mul(output.Pos, mul(mul(model, view), projection)).xyzw;
	output.Norm = mul(float4(input.Norm, 0), model).xyz;
	output.Tex = input.Tex;
	return output;
}