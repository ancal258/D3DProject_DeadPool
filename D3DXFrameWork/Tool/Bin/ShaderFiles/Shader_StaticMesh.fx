
matrix		g_matWorld, g_matView, g_matProj;
texture		g_DiffuseTexture;
texture		g_NormalTexture;
texture		g_SpecularTexture;

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
float		g_fPower = 20.f;

vector		g_vCamPosition;
bool		g_isCol;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_IN // ��ȯ�� ��ġ����(�������ۿ� �����) ������ ������ ������� ����ü.
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};


struct VS_OUT // ��ȯ(����, ��, �������)�� ��ģ ������ ����
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
};



VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	// mul : ��� ����� ������ �� �����Ѵ�.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_matWorld);

	Out.vNormal = normalize(vWorldNormal);

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN // �ȼ��� ������ ������� ����ü.
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector	vDiffuse : COLOR0;
	vector	vNormal : COLOR1;
	vector	vDepth : COLOR2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vDiffuse = vDiffuse;
	Out.vNormal = 0;// vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);

	Out.vDiffuse.gb *= 1 - g_isCol;
	return Out;
}



technique Default_Device
{
	pass Default_Rendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}