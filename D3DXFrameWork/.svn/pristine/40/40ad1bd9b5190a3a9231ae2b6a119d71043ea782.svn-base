
matrix		g_matWorld, g_matView, g_matProj;
vector		g_vMouse;
vector		g_vPickingPoint = (0,0,0,0);
float		g_fRadius;
texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
};

texture		g_FilterTexture;
sampler FilterSampler = sampler_state
{
	texture = g_FilterTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
texture		g_SourTexture;
sampler SourSampler = sampler_state
{
	texture = g_SourTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	AddressU = wrap;
	AddressV = wrap;
};
texture		g_DestTexture1;
sampler DestSampler1 = sampler_state
{
	texture = g_DestTexture1;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	AddressU = wrap;
	AddressV = wrap;
};

texture		g_DestTexture2;
sampler DestSampler2 = sampler_state
{
	texture = g_DestTexture2;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	AddressU = wrap;
	AddressV = wrap;
};


texture		g_DestTexture3;
sampler DestSampler3 = sampler_state
{
	texture = g_DestTexture3;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	AddressU = wrap;
	AddressV = wrap;
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
	float4  vColor : COLOR;
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

	vector vMousePos = mul(g_vMouse, matWVP);
	vector vPickingPos = mul(g_vPickingPoint, matWVP);
	vector vDir = Out.vPosition - vMousePos;
	vector vPickingDir = Out.vPosition - vPickingPos;
	if (length(vDir) <= g_fRadius)
		Out.vColor = vector(1.5f, 0.6f, 0.6f, 0.3f);
	else if (length(vPickingDir) <= 1.f)
		Out.vColor = vector(0.6f, 1.5f, 0.6f, 0.3f);
	else
		Out.vColor = vector(1,1,1,1);

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
	float4  vColor : COLOR;
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

	vector	vDiffuse_Sour = tex2D(SourSampler, In.vTexUV * 2.f);
	vector	vDiffuse_Dest1 = tex2D(DestSampler1, In.vTexUV * 50.f);
	vector	vDiffuse_Dest2 = tex2D(DestSampler2, In.vTexUV * 50.f);
	vector	vDiffuse_Dest3 = tex2D(DestSampler3, In.vTexUV * 50.f);


	vector	vFilter = tex2D(FilterSampler, In.vTexUV);

	vector	vResultColor = vDiffuse_Sour * (vFilter.r) + vDiffuse_Dest1 * (vFilter.g) + vDiffuse_Dest2 * (vFilter.b) + vDiffuse_Dest3 * (vFilter.a);

	Out.vColor = vResultColor; // tex2D(BaseSampler, In.vTexUV);
	Out.vColor.rgb *= In.vColor.rgb;
	Out.vColor.a = In.vColor.a;
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
		CullMode = ccw;

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