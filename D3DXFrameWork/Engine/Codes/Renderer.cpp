#include "..\Headers\Renderer.h"

#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Shader.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
{
	m_pTarget_Manager->AddRef();
}


HRESULT CRenderer::Add_Render_Group(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (RENDER_END <= eGroup)
		return E_FAIL;

	if (nullptr == pGameObject)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	pGameObject->AddRef();

	return NOERROR;
}

HRESULT CRenderer::Ready_Renderer()
{
	LPDIRECT3DDEVICE9	pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return E_FAIL;
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3DVIEWPORT9		ViewPort;
	pGraphic_Device->GetViewport(&ViewPort);

	// 디퍼드 렌더링을 위한 렌더타겟 정보를 생성한다.


	// For. Target_Diffuse : 디퍼드로 그리는 객체들의 픽셀 색상을 저장.
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Diffuse", ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For. Target_Normal : 디퍼드로 그리는 객체들의 픽셀 노말을 저장.
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Depth : 디퍼드로 그리는 객체들의 깊이를 저장.
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For. Target_Shade : 디퍼드로 그리는 객체들의 픽셀 명암을 저장.
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	// For.Target_Specular : 디퍼드로 그리는 객체들의 픽셀 정반사광을 저장.
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Diffuse", 0.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Normal", 0.f, 200.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Depth", 0.f, 400.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Shade", ViewPort.Width - 200.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Specular", ViewPort.Width - 200.f, 200.f, 200.f, 200.f)))
		return E_FAIL;
#endif


	// For. MRT_Deferred
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;
	// For. MRT_LightAcc
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;
	// For.Shader_LightAcc
	m_pShader_LightAcc = CShader::Create(pGraphic_Device, L"../Bin/ShaderFiles/Shader_LightAcc.fx");
	if (nullptr == m_pShader_LightAcc)
		return E_FAIL;

	// For.Shader_Blend
	m_pShader_Blend = CShader::Create(pGraphic_Device, L"../Bin/ShaderFiles/Shader_Blend.fx");
	if (nullptr == m_pShader_Blend)
		return E_FAIL;

	// For.Buffer
	if (FAILED(pGraphic_Device->CreateVertexBuffer(sizeof(VTXVIEWPORT) * 4, 0, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXVIEWPORT*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec4(0.0f - 0.5f, 0.0f - 0.5f, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPosition = _vec4(ViewPort.Width - 0.5f, 0.f - 0.5f, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPosition = _vec4(ViewPort.Width - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPosition = _vec4(0.0f - 0.5f, ViewPort.Height - 0.5f, 0.f, 1.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	if (FAILED(pGraphic_Device->CreateIndexBuffer(sizeof(POLYGON16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	POLYGON16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();



	return NOERROR;
}

void CRenderer::Render_Renderer()
{
	Render_Priority();
	Render_Deferred();
	Render_Alpha();
	Render_UI();

#ifdef _DEBUG
	//m_pTarget_Manager->Render_DebugBuffer(L"MRT_Deferred");
	//m_pTarget_Manager->Render_DebugBuffer(L"MRT_LightAcc");
#endif
}

void CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_PRIORITY].clear();
}

void CRenderer::Render_NoneAlpha()
{
	for (auto& pGameObject : m_RenderList[RENDER_NONEALPHA])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_NONEALPHA].clear();
}

void CRenderer::Render_BackUI()
{
	for (auto& pGameObject : m_RenderList[RENDER_BACKUI])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_BACKUI].clear();
}

void CRenderer::Render_Player()
{
	for (auto& pGameObject : m_RenderList[RENDER_PLAYER])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_PLAYER].clear();
}

_bool Sort(CGameObject * pSour, CGameObject * pDest)
{
	return pSour->Get_ViewDepth() > pDest->Get_ViewDepth();
}

void CRenderer::Render_Alpha()
{
	m_RenderList[RENDER_ALPHA].sort(Sort);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_ALPHA].clear();
}

void CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_UI].clear();
}

void CRenderer::Render_Deferred()
{
	if (nullptr == m_pTarget_Manager)
		return;

	m_pTarget_Manager->Begin_MRT(L"MRT_Deferred");

	Render_NoneAlpha();
	Render_BackUI();
	Render_Player();

	m_pTarget_Manager->End_MRT(L"MRT_Deferred");

	Render_LightAcc();

	Render_Blend();
}

void CRenderer::Render_LightAcc()
{

	if (nullptr == m_pTarget_Manager)
		return;

	LPD3DXEFFECT pEffect = m_pShader_LightAcc->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	// 장치에 Shade타겟을 셋팅한다.
	m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc");
	if (true == m_isTrigger)
	{
		m_pTarget_Manager->SetUp_OnShader(pEffect, "g_NormalTexture", L"Target_Normal");
		m_pTarget_Manager->SetUp_OnShader(pEffect, "g_DepthTexture", L"Target_Depth");
	}
	pEffect->Begin(nullptr, 0);

	CLight_Manager::GetInstance()->Render_Light(pEffect);

	pEffect->End();

	m_pTarget_Manager->End_MRT(L"MRT_LightAcc");
}

void CRenderer::Render_Blend()
{
	LPDIRECT3DDEVICE9	pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return;

	pGraphic_Device->AddRef();

	LPD3DXEFFECT pEffect = m_pShader_Blend->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	m_pTarget_Manager->SetUp_OnShader(pEffect, "g_DiffuseTexture", L"Target_Diffuse");

	m_pTarget_Manager->SetUp_OnShader(pEffect, "g_ShadeTexture", L"Target_Shade");
	m_pTarget_Manager->SetUp_OnShader(pEffect, "g_SpecularTexture", L"Target_Specular");

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXVIEWPORT));
	pGraphic_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	pGraphic_Device->SetIndices(m_pIB);
	pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pGraphic_Device);
	Safe_Release(pEffect);



}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*		pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		_MSG_BOX("CRenderer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone_Component()
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
		{
			Safe_Release(pGameObject);
		}
		m_RenderList[i].clear();
	}
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pShader_Blend);
	Safe_Release(m_pShader_LightAcc);
	Safe_Release(m_pTarget_Manager);

	CComponent::Free();
}
