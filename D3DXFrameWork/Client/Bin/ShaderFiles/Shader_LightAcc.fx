
// Shade타겟에 명암을 그리기위한 셰이더.
// 실제 Light클래스안에 만들어 놓은 사각형 버퍼를 렌더링한다.

texture			g_NormalTexture;

sampler	NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

vector			g_vLightDir;


struct PS_IN // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;	
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vShade : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vNormalInfo = tex2D(NormalSampler, In.vTexUV);

	// 0 ~ 1    ->    -1 ~ 1
	vector		vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	Out.vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal)), 0.f);		
	Out.vShade.a = 1.f;

	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{		
		ZWriteEnable = false;
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}	
}