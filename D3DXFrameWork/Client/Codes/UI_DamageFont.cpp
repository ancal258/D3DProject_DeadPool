#include "stdafx.h"
#include "..\Headers\UI_DamageFont.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Player.h"
_USING(Client)

CUI_DamageFont::CUI_DamageFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CUI_DamageFont::CUI_DamageFont(const CUI_DamageFont & rhs)
	: CGameObject(rhs)
	, m_iIndex(rhs.m_iIndex)
{

}

void CUI_DamageFont::Set_Position(CGameObject* pParentObject, _uint iType)
{
	CTransform* pTransform = (CTransform*)pParentObject->Get_ComponentPointer(L"Com_Transform");
	CPlayer* pPlayer = (CPlayer*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	_matrix matPlayer = *pPlayer->Get_RealMatrix();
	_vec3 vPlayerRight = (_vec3)matPlayer.m[0];
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
	m_pParentMatrix = pTransform->Get_WorldMatrix();
	m_vPosition = _vec3(m_pParentMatrix->m[3]);
	m_vPosition.y += 1.7f;
	if (1 == iType)
		m_vPosition += vPlayerRight * 0.25f;

}

HRESULT CUI_DamageFont::Ready_GameObject_Prototype(_uint iIndex)
{
	m_iIndex = iIndex;
	return NOERROR;
}

HRESULT CUI_DamageFont::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	return NOERROR;
}

_int CUI_DamageFont::Update_GameObject(const _float & fTimeDelta)
{
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


	//_vec3 vPosition = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	////vPosition.y += fTimeDelta;
	////m_fTimeAcc += fTimeDelta;

	m_pTransformCom->Scaling(0.4f, 0.4f, 1);

	m_vPosition.y += fTimeDelta;
	m_fTimeAcc += fTimeDelta;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &m_vPosition);


	if (m_fTimeAcc > 1.0f)
		Set_Lived(false);

	return _int();
}

_int CUI_DamageFont::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}

void CUI_DamageFont::Render_GameObject()
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
	pEffect->BeginPass(3);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CUI_DamageFont::Ready_Component()
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
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Shader_UI");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_DamageFont");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CUI_DamageFont::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_fAlpha", 1 - m_fTimeAcc);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", m_iIndex);



	Safe_Release(pEffect);

	return NOERROR;
}

CUI_DamageFont * CUI_DamageFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iIndex)
{
	CUI_DamageFont*		pInstance = new CUI_DamageFont(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(iIndex)))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_DamageFont::Clone_GameObject()
{
	CUI_DamageFont*		pInstance = new CUI_DamageFont(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CUI_DamageFont Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_DamageFont::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
