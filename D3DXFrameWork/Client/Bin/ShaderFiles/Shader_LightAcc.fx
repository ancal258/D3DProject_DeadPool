//
//
//// ShadeŸ�ٿ� ����� �׸������� ���̴�.
//// ���� LightŬ�����ȿ� ����� ���� �簢�� ���۸� �������Ѵ�.
//
//texture		g_NormalTexture;
//
//
//sampler NormalSampler = sampler_state
//{
//	texture		g_NormalTexture;
//};
//struct PS_IN // �ȼ��� ������ ������� ����ü.
//{
//	float4	vPosition : POSITION;
//	float2	vTexUV : TEXCOORD0;
//};
//
//struct PS_OUT
//{
//	vector	vShade : COLOR0;
//};
//
//PS_OUT PS_MAIN(PS_IN In)
//{
//	PS_OUT			Out = (PS_OUT)0;
//
//	vector vNormalInfo = tex2D(NormalSampler, In.vTexUV);
//
//	vector vNormal = vector(vNoramlInfo.xyz * 2 - 1.f,0);
//
//	Out.Shade = max(dot(normalize(g_vLightDir)* -1.f, normalize(vNormal)),0.f);
//
//	return Out;
//}
//
//
//technique Default_Device
//{
//	pass Default_Rendering
//	{
//		VertexShader = compile NULL;
//		PixelShader = compile ps_3_0 PS_MAIN();
//	}	
//} 