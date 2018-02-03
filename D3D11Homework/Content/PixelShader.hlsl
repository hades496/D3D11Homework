// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
};

// (内插)颜色数据的传递函数。
float4 main(PixelShaderInput input) : SV_TARGET
{
	//float3 b = input.color;
	//float3 c = 0.3 * b.x + 0.59 * b.y + 0.11 * b.z;
	return float4(input.color, 1.0f);
}
