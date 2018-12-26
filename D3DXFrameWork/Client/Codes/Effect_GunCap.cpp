#include "stdafx.h"
#include "Effect_GunCap.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "DP_Gun.h"
_USING(Client)

CEffect_GunCap::CEffect_GunCap(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEffect_GunCap::CEffect_GunCap(const CEffect_GunCap & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_GunCap::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEffect_GunCap::Ready_GameObject()
{

	if (FAILED(CEffect_GunCap::Ready_Component()))
		return E_FAIL;

	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(15, 3, 10));

	_float	fPlusX = -20.f;
	_float	fPlusY = 8.f;
	m_pTransformCom->Scaling(0.15f, 0.15f, 0.15f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(-90));
	m_pTransformCom->Set_PlusPosition(0, fPlusX);
	m_pTransformCom->Set_PlusPosition(1, fPlusY);

	return NOERROR;
}

_int CEffect_GunCap::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pTransformCom->RotationX(180, fTimeDelta);

	if (m_fTimeAcc > 0.1f)
		Set_Lived(false);

	return _int();
}

_int CEffect_GunCap::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (m_pParentMatrix != nullptr)
		m_pTransformCom->Set_ParentMatrix(*m_pParentMatrix);

	m_pTransformCom->Update_Matrix();
	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

void CEffect_GunCap::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return;



	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(4);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);


}

HRESULT CEffect_GunCap::Ready_Component()
{
	CComponent_Manager*         pComponent_Manager = CComponent_Manager::GetInstance();
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

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_GunCap");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEffect_GunCap::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld");

	_matrix         matView, matProj;

	Get_Transform(D3DTS_VIEW, &matView);
	Get_Transform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	// For.LightInfo
	const D3DLIGHT9* pLightInfo = CLight_Manager::GetInstance()->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetVector("g_vMtrlAmbient", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
	pEffect->SetVector("g_vMtrlSpecular", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetFloat("g_fPower", 20.f);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

CEffect_GunCap * CEffect_GunCap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_GunCap*		pInstance = new CEffect_GunCap(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_GunCap::Clone_GameObject()
{
	CEffect_GunCap*		pInstance = new CEffect_GunCap(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CEffect_GunCap Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_GunCap::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
