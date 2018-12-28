
matrix		g_matWorld, g_matView, g_matProj;
texture		g_DiffuseTexture;
texture		g_NormalTexture;
texture		g_SpecularTexture;
bool		g_isCol;
bool		g_isCartoon;
float		g_fCartoonOffset = 1.f;
float		g_fOutlineOffset = 1.f;

vector		g_vCamPosition = vector(1.f,1.f,1.f,1.f);


sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};


struct VS_IN // 변환을 거치기전(정점버퍼에 선언된) 정점의 정보를 담기위한 구조체.
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float3	vTangent : TANGENT;
};

struct VS_OUT // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	float3	vRight : TEXCOORD2;
	float3	vUp	: TEXCOORD3;
	float3	vLook : TEXCOORD4;
};

struct VS_OUT_SKY // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct VS_OUT_PLAYER // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	float4	vCamDir : TEXCOORD2;
	float3	vRight : TEXCOORD3;
	float3	vUp	: TEXCOORD4;
	float3	vLook : TEXCOORD5;
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
	float3		vWorldNormal2 = mul(vector(In.vNormal, 0.f), g_matWorld);
	float3		vWorldTangent = mul(vector(In.vTangent, 0.f), g_matWorld);
	float3		vWorldBinormal = cross(vWorldNormal, vWorldTangent);
	Out.vNormal = normalize(vWorldNormal);
	Out.vLook = normalize(vWorldNormal2);
	Out.vRight = normalize(vWorldTangent);
	Out.vUp = normalize(vWorldBinormal);

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

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

VS_OUT_PLAYER VS_MAIN_PLAYER(VS_IN In)
{
	VS_OUT_PLAYER			Out = (VS_OUT_PLAYER)0;

	matrix			matWV, matWVP;

	// mul : 모든 행렬의 연산을 다 수행한다.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);


	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_matWorld);
	vector		vCamDir = g_vCamPosition - mul(vector(In.vPosition, 1.f), g_matWorld);

	float3		vWorldNormal2 = mul(vector(In.vNormal, 0.f), g_matWorld);
	float3		vWorldTangent = mul(vector(In.vTangent, 0.f), g_matWorld);
	float3		vWorldBinormal = cross(vWorldNormal, vWorldTangent);
	Out.vNormal = normalize(vWorldNormal);
	Out.vLook = normalize(vWorldNormal2);
	Out.vRight = normalize(vWorldTangent);
	Out.vUp = normalize(vWorldBinormal);

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

	Out.vCamDir = normalize(vCamDir);

	return Out;
}

struct PS_IN // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	float3	vRight : TEXCOORD2;
	float3	vUp	: TEXCOORD3;
	float3	vLook : TEXCOORD4;
};

struct PS_IN_SKY // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct PS_IN_PLAYER // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	float4	vCamDir : TEXCOORD2;
	float3	vRight : TEXCOORD3;
	float3	vUp	: TEXCOORD4;
	float3	vLook : TEXCOORD5;
};

struct PS_OUT
{
	vector	vDiffuse : COLOR0;
	vector	vNormal : COLOR1;
	vector	vDepth : COLOR2;
};

struct PS_OUT_SKY
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	float3		vNormalMap;// = (vNormalTex.xyz - 0.5f) * 2.f;

	vNormalMap.x = vDiffuse.a;
	vNormalMap.y = vNormalTex.a;
	vNormalMap.z = 0;//1 - vNormalTex.y;
	vNormalMap = (vNormalMap.xyz - 0.5f) * 2.f;

	float3x3 matWorld;
	matWorld[0] = In.vRight;
	matWorld[1] = In.vUp;
	matWorld[2] = In.vLook;
	float3 vRealNormal = mul(vNormalMap, matWorld);

	Out.vDiffuse = vDiffuse;
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vNormal = vector((vRealNormal.xyz * 0.5f) + 0.5f, 0);
	//Out.vDiffuse.gb = Out.vDiffuse.gb * (1.3 - g_isCol);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);


	return Out;
}

PS_OUT_SKY PS_MAIN_SKY(PS_IN_SKY In)
{
	PS_OUT_SKY			Out = (PS_OUT_SKY)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = vDiffuse;

	return Out;
}

PS_OUT PS_MAIN_PLAYER(PS_IN_PLAYER In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	float3		vNormalTex = tex2D(NormalSampler, In.vTexUV);
	vNormalTex = float3((vNormalTex.xyz - 0.5) * 2.f);
	float3x3 matWorld;
	matWorld[0] = In.vRight;
	matWorld[1] = In.vUp;
	matWorld[2] = In.vLook;
	float3 vRealNormal = mul(vNormalTex, matWorld);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	//Out.vNormal = vector((vRealNormal.xyz + 1) * 0.5f, 0);


	Out.vDiffuse =vDiffuse;

	//Out.vDiffuse.gb = Out.vDiffuse.gb * (1.3 - g_isCol);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);

	if (true == g_isCartoon)
	{
		float fOffset = dot(In.vCamDir, In.vNormal);

		Out.vDiffuse = ceil(Out.vDiffuse * g_fCartoonOffset) / g_fCartoonOffset;

		if (fOffset < 0.43f && fOffset > -0.43f)
			Out.vDiffuse.rgb = g_fOutlineOffset;

		Out.vNormal = ceil(Out.vNormal * g_fCartoonOffset) / g_fCartoonOffset;
		//Out.vDepth = ceil(Out.vDepth * 5.0f) / 5.f;
	}


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
	
	pass Player_Rendering
	{
		VertexShader = compile vs_3_0 VS_MAIN_PLAYER();
		PixelShader = compile ps_3_0 PS_MAIN_PLAYER();
	}
}