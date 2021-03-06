#include <3DSnowman.fx>

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, model);
	output.Pos = mul(output.Pos, view);
	output.Pos = mul(output.Pos, projection);
	output.Norm = mul(float4(input.Norm, 0), model).xyz;
	output.Tex = input.Tex;
	return output;
}

