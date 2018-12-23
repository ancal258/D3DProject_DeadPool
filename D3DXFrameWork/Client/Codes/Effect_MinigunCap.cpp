#include "stdafx.h"
#include "Effect_MinigunCap.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Minigun.h"
_USING(Client)

CEffect_MinigunCap::CEffect_MinigunCap(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CEffect_MinigunCap::CEffect_MinigunCap(const CEffect_MinigunCap & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_MinigunCap::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEffect_MinigunCap::Ready_GameObject()
{

	if (FAILED(CEffect_MinigunCap::Ready_Component()))
		return E_FAIL;

	if (FAILED(SetUp_PlayerPointer()))
		return E_FAIL;

	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(15, 3, 10));

	_float	fPlus = -120.f;
	m_pTransformCom->Scaling(0.6f, 0.6f, 0.6f);
	m_pTransformCom->Set_AngleY(D3DXToRadian(-90));
	m_pTransformCom->Set_PlusPosition(0, fPlus);

	return NOERROR;
}

_int CEffect_MinigunCap::Update_GameObject(const _float & fTimeDelta)
{

	m_pTransformCom->RotationX(180, fTimeDelta);


	return _int();
}

_int CEffect_MinigunCap::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Set_ParentMatrix(m_pMinigun->Get_HeadMatrix());

	m_pTransformCom->Update_Matrix();
	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

void CEffect_MinigunCap::Render_GameObject()
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

HRESULT CEffect_MinigunCap::Ready_Component()
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
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_MinigunCap");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEffect_MinigunCap::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CEffect_MinigunCap::SetUp_PlayerPointer()
{
	CObject_Manager*			pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;

	pObject_Manager->AddRef();

	m_pMinigun = (CMinigun*)pObject_Manager->Get_ObjectPointer(SCENE_STAGE, L"Layer_Minigun", 0);
	if (nullptr == m_pMinigun)
		return E_FAIL;

	Safe_Release(pObject_Manager);

	return NOERROR;
}

CEffect_MinigunCap * CEffect_MinigunCap::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_MinigunCap*		pInstance = new CEffect_MinigunCap(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_MinigunCap::Clone_GameObject()
{
	CEffect_MinigunCap*		pInstance = new CEffect_MinigunCap(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CEffect_MinigunCap Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CEffect_MinigunCap::Update_HandMatrix()
{
	if (nullptr == m_pHandMatrix[0] ||
		nullptr == m_pHandMatrix[1] ||
		nullptr == m_pRootMatrix)
		return E_FAIL;

	m_CombinedHandMatrix[0] = *m_pHandMatrix[0] * *m_pTransformCom->Get_WorldMatrix();
	m_CombinedHandMatrix[1] = *m_pHandMatrix[1] * *m_pTransformCom->Get_WorldMatrix();
	m_CombinedRootMatrix = *m_pRootMatrix * *m_pTransformCom->Get_WorldMatrix();

	return NOERROR;
}

void CEffect_MinigunCap::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
