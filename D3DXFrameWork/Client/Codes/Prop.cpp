#include "stdafx.h"
#include "..\Headers\Prop.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Airplane.h"
#include "Static_Airplane.h"
_USING(Client)

CProp::CProp(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CProp::CProp(const CProp & rhs)
	: CGameObject(rhs)
{
}

HRESULT CProp::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CProp::Ready_GameObject()
{

	if (FAILED(CProp::Ready_Component()))
		return E_FAIL;

	m_pAirplane = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Airplane_Field", 0);
	if (nullptr == m_pAirplane)
	{
		++m_iStage;
		m_pAirplane = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Airplane", 0);
		if (nullptr == m_pAirplane)
			return E_FAIL;
	}
	//m_pHandMatrix[0] = m_pMeshCom->Get_FrameMatrixByName("L_Weapon01_Wpn_XW");
	//if (nullptr == m_pHandMatrix[0])
	//	return E_FAIL;
	//m_pHandMatrix[1] = m_pMeshCom->Get_FrameMatrixByName("R_Weapon01_Wpn_XW");
	//if (nullptr == m_pHandMatrix[1])
	//	return E_FAIL;
	//m_pRootMatrix = m_pMeshCom->Get_FrameMatrixByName("C_Root_Reference_XR");
	//if (nullptr == m_pRootMatrix)
	//	return E_FAIL;
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0, 550, 0));


	return NOERROR;
}

_int CProp::Update_GameObject(const _float & fTimeDelta)
{
	//cout << m_fAngle << endl;
	if (GetKeyState('N') & 0x80)
		m_fAngle += 10;
	if (GetKeyState('M') & 0x80)
		m_fAngle -= 10;
	m_pTransformCom->RotationY(D3DXToRadian(m_fAngle), fTimeDelta);


	//Update_HandMatrix();






	return _int();
}

_int CProp::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	if(1 == m_iStage)
		m_pTransformCom->Set_ParentMatrix(*dynamic_cast<const CStatic_Airplane*>(m_pAirplane)->Get_WorldMatrix());
	if(2 == m_iStage)
		m_pTransformCom->Set_ParentMatrix(*dynamic_cast<const CAirplane*>(m_pAirplane)->Get_WorldMatrix());
	_vec3 vTest = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	m_pTransformCom->Update_Matrix();


	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;
	return _int();
}

void CProp::Render_GameObject()
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
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);


}

HRESULT CProp::Ready_Component()
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
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Prop");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Prop");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Navigation
	m_pNavigationCom = (CNavigation*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Navigation");
	if (FAILED(Add_Component(L"Com_Navigation", m_pNavigationCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CProp::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CProp * CProp::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CProp*		pInstance = new CProp(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CProp::Clone_GameObject()
{
	CProp*		pInstance = new CProp(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CProp Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CProp::Update_HandMatrix()
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

void CProp::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
