#include "stdafx.h"
#include "Effect_GunFlash.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"


CEffect_GunFlash::CEffect_GunFlash(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEffect_GunFlash::CEffect_GunFlash(const CEffect_GunFlash & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_GunFlash::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEffect_GunFlash::Ready_GameObject()
{
	//m_iType = rand() % 2;

	if (FAILED(CEffect_GunFlash::Ready_Component()))
		return E_FAIL;


	_float	fPlusX = -20.f;
	_float	fPlusY = 10.f;
	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(-90));
	m_pTransformCom->Set_PlusPosition(0, fPlusX);
	m_pTransformCom->Set_PlusPosition(1, fPlusY);

	m_pTransformCom->RotationAxis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK), (rand() % 4)* 45, 1);

	CLight_Manager::GetInstance()->Set_IsRender(true, 0);
	return NOERROR;
}

_int CEffect_GunFlash::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;


	//m_pTransformCom->RotationZ(45, fTimeDelta);
	_float fScale = m_fTimeAcc * 6.f;
	m_pTransformCom->Scaling(fScale, fScale, fScale);
	m_pTransformCom->Update_Matrix();

	return _int();
}

_int CEffect_GunFlash::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if(m_pParentMatrix != nullptr)
		m_pTransformCom->Set_ParentMatrix(*m_pParentMatrix);

	m_pTransformCom->Update_Matrix();
	_vec3 vPos = *(_vec3*)&m_pTransformCom->Get_WorldMatrix()->m[3];
	vPos.y -= 7;
	CLight_Manager::GetInstance()->Set_PointInfo(CLight_Manager::STATE_POSITION, &vPos, 0);
	//CLight_Manager::GetInstance()->Set_PointInfo(CLight_Manager::STATE_POSITION, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), 0);

	if (m_fTimeAcc >= 0.05f)
	{
		CLight_Manager::GetInstance()->Set_IsRender(false, 0);

		Set_Lived(false);
	}

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

void CEffect_GunFlash::Render_GameObject()
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

HRESULT CEffect_GunFlash::Ready_Component()
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
	if(0 == m_iType)
		m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_GunFlash");
	else if(1 == m_iType)
		m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_GunFlash2");

	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEffect_GunFlash::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	pEffect->SetFloat("g_fU", 1);
	pEffect->SetFloat("g_fV", 1);

	Safe_Release(pEffect);

	return NOERROR;
}

CEffect_GunFlash * CEffect_GunFlash::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_GunFlash*		pInstance = new CEffect_GunFlash(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_GunFlash::Clone_GameObject()
{
	CEffect_GunFlash*		pInstance = new CEffect_GunFlash(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CEffect_GunFlash Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_GunFlash::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
