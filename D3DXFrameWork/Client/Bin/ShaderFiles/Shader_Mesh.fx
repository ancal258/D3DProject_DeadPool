
matrix		g_matWorld, g_matView, g_matProj;
texture		g_DiffuseTexture;
texture		g_NormalTexture;
texture		g_SpecularTexture;
bool		g_isCol;

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
float		g_fPower = 20.f;

vector		g_vCamPosition = vector(1.f,1.f,1.f,1.f);


sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct VS_IN // 변환을 거치기전(정점버퍼에 선언된) 정점의 정보를 담기위한 구조체.
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};


struct VS_OUT // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
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

	Out.vNormal = normalize(vWorldNormal);

	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);

	return Out;
}

VS_OUT VS_MAIN_SKY(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

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

struct PS_IN // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);

	vector		vLook = normalize(In.vWorldPos - g_vCamPosition);
	vector		vReflect = normalize(reflect(g_vLightDir, In.vNormal));

	vector		vSpecular = pow(max(dot(vLook * -1.f, vReflect), 0.f), g_fPower);

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = (g_vLightDiffuse * (vDiffuse * g_vMtrlDiffuse)) * saturate(vShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ (g_vLightSpecular * g_vMtrlSpecular) * vSpecular;

	Out.vColor.gb = Out.vColor.gb * (1.3 - g_isCol);
	return Out;
}

PS_OUT PS_MAIN_SKY(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);

	vector		vLook = normalize(In.vWorldPos - g_vCamPosition);
	vector		vReflect = normalize(reflect(g_vLightDir, In.vNormal));

	vector		vSpecular = pow(max(dot(vLook * -1.f, vReflect), 0.f), g_fPower);

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = (g_vLightDiffuse * (vDiffuse * g_vMtrlDiffuse));

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
}