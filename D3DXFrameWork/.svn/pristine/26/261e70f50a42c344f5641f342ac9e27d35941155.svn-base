
matrix		g_matWorld, g_matView, g_matProj;
bool		g_isColl;

struct VS_IN 
{
	float3	vPosition : POSITION;
};

struct VS_OUT // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;	
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	// mul : 모든 행렬의 연산을 다 수행한다.
	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	return Out;
}

struct PS_IN // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;	
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = vector(g_isColl, 1.f - g_isColl, 0.f, 1.f);

	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{
		fillmode = wireframe;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}	
}