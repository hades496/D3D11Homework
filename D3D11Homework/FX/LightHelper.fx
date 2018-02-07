/* 
**********               Lighting.fx		  **********
********** �������ֹ�Դ�ṹ��Ķ���ͼ��㺯�� **********
*/

// ���Դ
struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

// �۹�ƹ�Դ
struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};


// ƽ�й�Դ
struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

// ����
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;  //  w����Ϊ�߹�ǿ��
	float4 Reflect;
};

// ����ƽ�й�Դ
void ComputeDirectionalLight(Material mat, DirectionalLight L, float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// ��ʼ������ı���
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ����ʸ������ߵĴ��������෴
	float3 lightVec = -L.Direction;

	// ��ӻ�����
	ambient = mat.Ambient * L.Ambient;

	// ���������;����
	float diffuseFactor = dot(lightVec, normal);

	// Flatten���⶯̬��֧
	[flatten]
	// ���㱳���Դ���ټ���
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}

// ������Դ
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// ��ʼ���������
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ����ָ���Դ��ʸ��
	float3 lightVec = L.Position - pos;

	// �������Դ�ľ���
	float d = length(lightVec);

	// Range test.
	if (d > L.Range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	// ˥��
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}

// ����۹��
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// ��ʼ���������.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// �ӱ���ָ���Դ�Ĺ���ʸ��
	float3 lightVec = L.Position - pos;

	// �����뿪��Դ�ľ���
	float d = length(lightVec);

	// Range test.
	if (d > L.Range)
		return;

	// �淶������ʸ��
	lightVec /= d;

	// ���㻷����
	ambient = mat.Ambient * L.Ambient;

	// ����������;���⣬provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten���⶯̬��֧
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}


cbuffer cbPerFrame
{
	DirectionalLight gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	Material gMaterial;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
};

