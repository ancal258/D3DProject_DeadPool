
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

struct VS_OUT // ��ȯ(����, ��, �������)�� ��ģ ������ ����
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
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

struct PS_IN // �ȼ��� ������ ������� ����ü.
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	float2 vTexUV = In.vTexUV;

	Out.vColor = tex2D(BaseSampler, vTexUV);
	Out.vColor.b *= 0.1f;
	Out.vColor.g *= 0.1f;
	Out.vColor.a = 1 - (Out.vColor.a + 0.2f);
	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{
		CullMode = none;

		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}