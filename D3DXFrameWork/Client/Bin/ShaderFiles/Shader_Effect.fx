
matrix		g_matWorld, g_matView, g_matProj;


float		g_fU;
float		g_fV;

float		g_fAlpha;

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_DiffuseTexture;

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
	float2	vTexUV : TEXCOORD0;
};


struct VS_OUT // 변환(월드, 뷰, 투영행렬)을 거친 정점의 정보
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
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
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN // 픽셀의 정보를 담기위한 구조체.
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	float2		vNewUV;

	vNewUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vNewUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	vector		vDepthInfo = tex2D(DepthSampler, vNewUV);

	// 이펙트 픽셀을 그리려고 했던 영역에 이미 그려져 있었든 그 무언가의 깊이.
	float		fDestZ = vDepthInfo.y * 500.0f;

	//vDiffuse.a = vDiffuse.a * saturate(fDestZ - In.vProjPos.w);
	Out.vColor = vDiffuse;
	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_MAIN_MESH(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float2 fUV = float2(In.vTexUV.x + g_fU, In.vTexUV.y + g_fV);
	vector		vDiffuse = tex2D(DiffuseSampler, fUV);

	Out.vColor = vDiffuse;
	Out.vColor.a = saturate(vDiffuse.r + vDiffuse.g + vDiffuse.b);

	return Out;
}

PS_OUT PS_MAIN_AT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);


	Out.vColor = vDiffuse;
	Out.vColor.a = saturate(vDiffuse.r + vDiffuse.g + vDiffuse.b);
	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_MAIN_BLOOD(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);


	Out.vColor = vDiffuse;
	Out.vColor.a = saturate(vDiffuse.r + vDiffuse.g + vDiffuse.b);
	Out.vColor.gb = 0;
	Out.vColor.a *= g_fAlpha;

	return Out;
}
PS_OUT PS_MAIN_EXPLOSION(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);


	Out.vColor = vDiffuse;
	//if(Out.vColor.r == Out.vColor.g && Out.vColor.g == Out.vColor.b)
	//	Out.vColor.a = 0;

	return Out;
}



technique Default_Device
{
	pass Default_Rendering
	{
		cullmode = cw;

		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		/*AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 100;*/


		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Alpha_Testing
	{
		cullmode = cw;

		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;


		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_AT();
	}

	pass MeshEffect_Rendering
	{
		cullmode = none;

		//AlphaBlendEnable = true;
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;

		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 10;


		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_MESH();
	}

	pass BloodEffect_Rendering
	{
		cullmode = none;

		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_BLOOD();
	}
	pass ExplosionEffect_Rendering
	{
		cullmode = none;

		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_EXPLOSION();
	}
}