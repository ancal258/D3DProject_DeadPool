
matrix		g_matWorld, g_matView, g_matProj;

vector		g_vBrushPosition;
float		g_fRadius;

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

texture		g_BrushTexture;

sampler BrushSampler = sampler_state
{
	texture = g_BrushTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	AddressU = wrap;
	AddressV = wrap;
};


struct VS_IN // 변환을 거치기전(정점버퍼에 선언된) 정점의 정보를 담기위한 구조체.
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT_PHONG
{
	float4	vPosition : POSITION;
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector	vWorldPos : TEXCOORD1;
	vector	vProjPos : TEXCOORD2;

};

VS_OUT_PHONG VS_MAIN_PHONG(VS_IN In)
{
	VS_OUT_PHONG			Out = (VS_OUT_PHONG)0;

	matrix			matWV, matWVP;

	// mul : 모든 행렬의 연산을 다 수행한다.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_PHONG
{
	float4	vPosition : POSITION;
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector	vWorldPos : TEXCOORD1;
	vector	vProjPos : TEXCOORD2;
};


struct PS_OUT
{
	vector	vDiffuse : COLOR0;
	vector	vNormal : COLOR1;
	vector	vDepth : COLOR2;
};

PS_OUT PS_MAIN_PHONG(PS_IN_PHONG In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector	vDiffuse_Sour = tex2D(SourSampler, In.vTexUV * 2.f);
	vector	vDiffuse_Dest1 = tex2D(DestSampler1, In.vTexUV * 300.f);
	vector	vDiffuse_Dest2 = tex2D(DestSampler2, In.vTexUV * 2.f);
	vector	vDiffuse_Dest3 = tex2D(DestSampler3, In.vTexUV * 2.f);
	vector	vFilter = tex2D(FilterSampler, In.vTexUV);
	vector	vBrush = (vector)0;

	if (g_vBrushPosition.x - g_fRadius < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPosition.x + g_fRadius && 
		g_vBrushPosition.z - g_fRadius < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPosition.z + g_fRadius)
	{
		float2	vTexUV;

		vTexUV.x = (In.vWorldPos.x - (g_vBrushPosition.x - g_fRadius)) / (g_fRadius * 2.f);
		vTexUV.y = ((g_vBrushPosition.z + g_fRadius) - In.vWorldPos.z) / (g_fRadius * 2.f);

		vBrush = tex2D(BrushSampler, vTexUV);
	}


	
	vector	vResultColor = vDiffuse_Sour * (vFilter.r) + vDiffuse_Dest1 * (vFilter.g) + vDiffuse_Dest2 * (vFilter.b) + vDiffuse_Dest3 * (vFilter.a);

	Out.vDiffuse = vResultColor;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.f);

	return Out;
}



technique Default_Device
{

	pass Shade_Phong
	{		
		//FillMode = WireFrame;
		VertexShader = compile vs_3_0 VS_MAIN_PHONG();
		PixelShader = compile ps_3_0 PS_MAIN_PHONG();
	}
}