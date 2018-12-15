#include "stdafx.h"
#include "..\Headers\Effect_RectParticle.h"
#include "Component_Manager.h"
#include "Target_Manager.h"

_USING(Client)

CEffect_RectParticle::CEffect_RectParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CEffect_RectParticle::CEffect_RectParticle(const CEffect_RectParticle & rhs)
	: CGameObject(rhs)
{

}

HRESULT CEffect_RectParticle::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEffect_RectParticle::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	_float fX = (rand() % 20) * 0.1f + 10; // 10.x
	_float fZ = (rand() % 20) * 0.1f + 3; // 10.x
	m_fFrame = rand() % 16;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(fX, 0.f, fZ));
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(rand() % 10 + 10.0f, 0.f, rand() % 10));

	return NOERROR;
}

_int CEffect_RectParticle::Update_GameObject(const _float & fTimeDelta)
{
	//m_fFrame += 16 * fTimeDelta;
	//m_fAlpha -= 1 * fTimeDelta;
	//if (16.0f <= m_fFrame)
	//{
	//	m_fAlpha = 1.f;
	//	m_fFrame = 0.f;
	//}

	m_fAlpha -= 1 * fTimeDelta;
	if (0 >= m_fAlpha)
	{
		m_fAlpha = 1.f;
	}


	Compute_ViewDepth(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));

	_matrix		matView;
	Get_Transform(D3DTS_VIEW, &matView);

	// 카메라 회전상태의 역변환 상태로 갖고 있는다.
	_matrix		matBill;
	D3DXMatrixIdentity(&matBill);

	memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vec3));
	memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vec3));
	memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vec3));

	D3DXMatrixInverse(&matBill, nullptr, &matBill);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, (_vec3*)&matBill.m[0][0]);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, (_vec3*)&matBill.m[1][0]);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, (_vec3*)&matBill.m[2][0]);

	return _int();
}

_int CEffect_RectParticle::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}

void CEffect_RectParticle::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;


	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CEffect_RectParticle::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Transform
	m_pTransformCom = (CTransform*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	m_pBufferCom = (CBuffer_RcTex*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Blood");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEffect_RectParticle::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix			matView, matProj;

	Get_Transform(D3DTS_VIEW, &matView);
	Get_Transform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", _uint(m_fFrame));

	CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect, "g_DepthTexture", L"Target_Depth");

	Safe_Release(pEffect);

	return NOERROR;
}

CEffect_RectParticle * CEffect_RectParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_RectParticle*		pInstance = new CEffect_RectParticle(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_Effect_RectParticle Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_RectParticle::Clone_GameObject()
{
	CEffect_RectParticle*		pInstance = new CEffect_RectParticle(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CEffect_RectParticle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_RectParticle::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
