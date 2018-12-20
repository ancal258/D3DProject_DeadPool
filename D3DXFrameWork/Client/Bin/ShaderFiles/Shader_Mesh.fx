
matrix		g_matWorld, g_matView, g_matProj;
texture		g_DiffuseTexture;
texture		g_NormalTexture;
texture		g_SpecularTexture;
bool		g_isCol;
bool		g_isCartoon;

vector		g_vCamPosition = vector(1.f,1.f,1.f,1.f);


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

struct VS_OUT_SKY // ��ȯ(����, ��, �������)�� ��ģ ������ ����
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct VS_OUT_PLAYER // ��ȯ(����, ��, �������)�� ��ģ ������ ����
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	float4	vCamDir : TEXCOORD2;
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

VS_OUT_SKY VS_MAIN_SKY(VS_IN In)
{
	VS_OUT_SKY			Out = (VS_OUT_SKY)0;

	matrix			matWV, matWVP;

	// mul : ��� ����� ������ �� �����Ѵ�.
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

	// mul : ��� ����� ������ �� �����Ѵ�.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);


	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_matWorld);
	vector		vCamDir = g_vCamPosition - mul(vector(In.vPosition, 1.f), g_matWorld);

	Out.vNormal = normalize(vWorldNormal);

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

	Out.vCamDir = normalize(vCamDir);

	return Out;
}

struct PS_IN // �ȼ��� ������ ������� ����ü.
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;

};

struct PS_IN_SKY // �ȼ��� ������ ������� ����ü.
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct PS_IN_PLAYER // ��ȯ(����, ��, �������)�� ��ģ ������ ����
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	float4	vCamDir : TEXCOORD2;
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

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

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

	Out.vDiffuse =vDiffuse;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	//Out.vDiffuse.gb = Out.vDiffuse.gb * (1.3 - g_isCol);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);

	if (true == g_isCartoon)
	{
		float fOffset = dot(In.vCamDir, In.vNormal);

		Out.vDiffuse = ceil(Out.vDiffuse * 5.0f) / 5.f;

		if (fOffset < 0.34f && fOffset > -0.34f)
			Out.vDiffuse.rgb = 0.f;

		Out.vNormal = ceil(Out.vNormal * 5.0f) / 5.f;
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