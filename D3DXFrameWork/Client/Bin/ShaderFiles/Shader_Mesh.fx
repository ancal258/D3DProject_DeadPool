
matrix		g_matWorld, g_matView, g_matProj;
texture		g_DiffuseTexture;
texture		g_NormalTexture;
texture		g_SpecularTexture;
bool		g_isCol;

vector		g_vCamPosition = vector(1.f,1.f,1.f,1.f);


sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_IN // 변환을 거치기전(정점버퍼에 선언된) 정점의 정보를 담기위한 구조체.
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT_SKY // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	// mul : 모든 행렬의 연산을 다 수행한다.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_matWorld);

	Out.vNormal = normalize(vWorldNormal);

	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT_SKY VS_MAIN_SKY(VS_IN In)
{
	VS_OUT_SKY			Out = (VS_OUT_SKY)0;

	matrix			matWV, matWVP;

	// mul : 모든 행렬의 연산을 다 수행한다.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_matWorld);

	Out.vNormal = normalize(vWorldNormal);

	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);

	return Out;
}

struct PS_IN // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};

struct PS_IN_SKY // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector	vDiffuse : COLOR0;
	vector	vNormal : COLOR1;
};

struct PS_OUT_SKY
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDiffuse.gb = Out.vDiffuse.gb * (1.3 - g_isCol);
	return Out;
}

PS_OUT_SKY PS_MAIN_SKY(PS_IN_SKY In)
{
	PS_OUT_SKY			Out = (PS_OUT_SKY)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = vDiffuse;

	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass SkyBox_Rendering
	{
		VertexShader = compile vs_3_0 VS_MAIN_SKY();
		PixelShader = compile ps_3_0 PS_MAIN_SKY();
	}
}