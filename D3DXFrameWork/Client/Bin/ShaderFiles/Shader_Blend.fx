
// 백버퍼에에 디퓨즈장면과 셰이드장면을 조합한 결과를 그리기위한 셰이더.


texture			g_DiffuseTexture;

sampler	DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_ShadeTexture;

sampler	ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_BloomTexture;

sampler BloomSampler = sampler_state
{
	texture = g_BloomTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture			g_ColorTexture;

sampler ColorSampler = sampler_state
{
	texture = g_ColorTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

vector			g_vLightDir;


float2 rcpres = { 0.001280, 0.000768};
float g_rcPresMul = 1.f;
//float2 rcpres = { 0.002580, 0.003368};
float2 PixelKernelH[13] =
{
	{ -6, 0 },
	{ -5, 0 },
	{ -4, 0 },
	{ -3, 0 },
	{ -2, 0 },
	{ -1, 0 },
	{ 0, 0 },
	{ 1, 0 },
	{ 2, 0 },
	{ 3, 0 },
	{ 4, 0 },
	{ 5, 0 },
	{ 6, 0 },
};

float2 PixelKernelV[13] =
{
	{ 0, -6 },
	{ 0, -5 },
	{ 0, -4 },
	{ 0, -3 },
	{ 0, -2 },
	{ 0, -1 },
	{ 0,  0 },
	{ 0,  1 },
	{ 0,  2 },
	{ 0,  3 },
	{ 0,  4 },
	{ 0,  5 },
	{ 0,  6 },
};

float BlurWeights[13] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};


struct PS_IN // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;	
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
	vector	vColor2 : COLOR1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	Out.vColor = vDiffuse * saturate(vShade) + vSpecular;
	Out.vColor2 = (Out.vColor * 1.2f) + 0.65f;
	Out.vColor2.a = 1.f;
	return Out;
}

float4 PS_MAIN_LAST(float2 vTexUV : TEXCOORD) : COLOR
{
	float4			Out = (float4)0;

	vector		vColor1 = tex2D(BloomSampler, vTexUV);
	vColor1 = pow(vColor1, 32);
	float4	vColor2 = -0.84;

	for (int iIndex = 0; iIndex < 13; ++iIndex)
	{
		vColor2 += tex2D(BloomSampler, vTexUV + (PixelKernelH[iIndex] * rcpres)) * BlurWeights[iIndex];
		vColor2 += tex2D(BloomSampler, vTexUV + (PixelKernelV[iIndex] * rcpres)) * BlurWeights[iIndex];
	}
	//vColor2 *= 0.48f;

	vector		vColor3 = tex2D(ColorSampler, vTexUV);

	Out = (vColor1 + vColor2 + vColor3) * g_rcPresMul;
	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{		
		ZWriteEnable = false;
		AlphaTestEnable = true;
		AlphaRef = 0;
		AlphaFunc = Greater;
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}	

	pass Last_Rendering
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_LAST();
	}
}