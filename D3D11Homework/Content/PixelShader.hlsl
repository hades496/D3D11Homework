// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
};

// (�ڲ�)��ɫ���ݵĴ��ݺ�����
float4 main(PixelShaderInput input) : SV_TARGET
{
	//float3 b = input.color;
	//float3 c = 0.3 * b.x + 0.59 * b.y + 0.11 * b.z;
	return float4(input.color, 1.0f);
}
