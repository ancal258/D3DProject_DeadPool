
matrix		g_matWorld, g_matView, g_matProj;
texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
};


struct VS_IN // 변환을 거치기전(정점버퍼에 선언된) 정점의 정보를 담기위한 구조체.
{
	float3	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_IN_COLOR // 변환을 거치기전(정점버퍼에 선언된) 정점의 정보를 담기위한 구조체.
{
	float3	vPosition : POSITION;
	float4	vColor : COLOR;
};

struct VS_OUT // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT_COLOR // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float4	vColor : COLOR;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	// mul : 모든 행렬의 연산을 다 수행한다.
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

	// mul : 모든 행렬의 연산을 다 수행한다.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vColor = In.vColor;

	return Out;
}

struct PS_IN // 픽셀의 정보를 담기위한 구조체.
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