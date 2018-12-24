
matrix		g_matWorld, g_matView, g_matProj;
texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
};


struct VS_IN // ��ȯ�� ��ġ����(�������ۿ� �����) ������ ������ ������� ����ü.
{
	float3	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_IN_COLOR // ��ȯ�� ��ġ����(�������ۿ� �����) ������ ������ ������� ����ü.
{
	float3	vPosition : POSITION;
	float4	vColor : COLOR;
};

struct VS_OUT // ��ȯ(����, ��, �������)�� ��ģ ������ ����
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT_COLOR // ��ȯ(����, ��, �������)�� ��ģ ������ ����
{
	float4	vPosition : POSITION;
	float4	vColor : COLOR;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	// mul : ��� ����� ������ �� �����Ѵ�.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;	 
}

VS_OUT_COLOR VS_MAIN_COLOR(VS_IN_COLOR In)
{
	VS_OUT_COLOR			Out = (VS_OUT_COLOR)0;

	matrix			matWV, matWVP;

	// mul : ��� ����� ������ �� �����Ѵ�.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vColor = In.vColor;

	return Out;
}

struct PS_IN // �ȼ��� ������ ������� ����ü.
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_IN_COLOR
{
	float4	vPosition : POSITION;
	float4	vColor : COLOR;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	float2 vTexUV = In.vTexUV;
	vTexUV.x = 1 - vTexUV.x;
	Out.vColor = tex2D(BaseSampler, vTexUV);

	return Out;
}

PS_OUT PS_MAIN_COLOR(PS_IN_COLOR In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = In.vColor;

	return Out;

}



technique Default_Device
{
	pass Default_Rendering
	{
		CullMode = none;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Color_Rendering
	{
		CullMode = none;
		VertexShader = compile vs_3_0 VS_MAIN_COLOR();
		PixelShader = compile ps_3_0 PS_MAIN_COLOR();
	}
}