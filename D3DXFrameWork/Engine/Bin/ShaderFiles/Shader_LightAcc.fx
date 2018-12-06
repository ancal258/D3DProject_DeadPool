
// Shade타겟에 명암을 그리기위한 셰이더.
// 실제 Light클래스안에 만들어 놓은 사각형 버퍼를 렌더링한다.

texture			g_NormalTexture;
texture			g_DepthTexture;

vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

vector			g_vMtrlDiffuse = (vector)1.f;
vector			g_vMtrlAmbient = (vector)1.f;
vector			g_vMtrlSpecular = (vector)1.f;
float			g_fPower = 30.f;

matrix			g_matViewInv;
matrix			g_matProjInv;

vector			g_vCamPosition;


sampler	NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

vector			g_vLightDir;
vector			g_vLightPos;
float			g_fRange;

struct PS_IN // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;	
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vShade : COLOR0;
	vector	vSpecular : COLOR1;
};

PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector		vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepthInfo.y * 500.f;
	// 0 ~ 1    ->    -1 ~ 1
	vector		vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);
	
	vector		vShade = max(dot(normalize(g_vLightDir)* -1.f, normalize(vNormal)), 0.f);
	Out.vShade = (g_vLightDiffuse * g_vMtrlDiffuse) * saturate((vShade+ (g_vLightAmbient * g_vMtrlAmbient)));
	Out.vShade.a = 1.f;

	vector		vReflect = reflect(g_vLightDir, vNormal);
	vector		vProjPos;

	// 투영행렬까지 곱해놓은 결과벡터 // UV는 0~1로 표현 되기 때문에 -1~1로 표현해줌.
	vProjPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vProjPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vProjPos.z = (vDepthInfo.x) * fViewZ;
	vProjPos.w = 1.f * fViewZ;

	vector		vViewPos;
	vViewPos = mul(vProjPos, g_matProjInv);

	vector		vWorldPos;
	vWorldPos = mul(vViewPos, g_matViewInv);

	vector		vLook = vWorldPos - g_vCamPosition;

	vector		vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * vSpecular;
	Out.vSpecular.a = 0.f;

	return Out;
}


PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector		vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepthInfo.y * 500.f;
	vector		vProjPos;

	// 투영행렬까지 곱해놓은 결과벡터 // UV는 0~1로 표현 되기 때문에 -1~1로 표현해줌.
	vProjPos.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vProjPos.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vProjPos.z = (vDepthInfo.x) * fViewZ;
	vProjPos.w = 1.f * fViewZ;

	vector		vViewPos;
	vViewPos = mul(vProjPos, g_matProjInv);

	vector		vWorldPos;
	vWorldPos = mul(vViewPos, g_matViewInv);

	// 점광원의 위치에서부터 내 픽셀을 바라보는 방향벡터.
	vector		vLightDir = vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);
				//어텐션
	float		fAtt = max((g_fRange - fDistance), 0) / g_fRange;


	// 0 ~ 1    ->    -1 ~ 1
	vector		vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);


	vector		vShade = max(dot(normalize(vLightDir)* -1.f, normalize(vNormal)), 0.f);
	
	Out.vShade = (g_vLightDiffuse * g_vMtrlDiffuse) * saturate((vShade + (g_vLightAmbient * g_vMtrlAmbient))) * fAtt;
	
	Out.vShade.a = 1.f;

	vector		vReflect = reflect(g_vLightDir, vNormal);

	vector		vLook = vWorldPos - g_vCamPosition;

	vector		vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * vSpecular * fAtt;
	Out.vSpecular.a = 0.f;

	return Out;
}


technique Default_Device
{
	pass Directional_Rendering
	{
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}

	pass Point_Rendering
	{
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}
}