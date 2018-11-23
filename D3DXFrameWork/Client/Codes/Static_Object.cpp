#include "stdafx.h"
#include "..\Headers\Static_Object.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Player.h"

#include "Camera_Debug.h"
_USING(Client)

CStatic_Object::CStatic_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CStatic_Object::CStatic_Object(const CStatic_Object & rhs)
	: CGameObject(rhs)
	, m_pComponent_Tag(rhs.m_pComponent_Tag)
{

}

void CStatic_Object::Set_StateInfo(_vec3* pRight, _vec3* pUp, _vec3* vLook, _vec3* vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, pRight);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, pUp);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}
HRESULT CStatic_Object::Ready_GameObject_Prototype(const _tchar* pComponent_Tag)
{
	m_pComponent_Tag = pComponent_Tag;
	return NOERROR;
}

HRESULT CStatic_Object::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(20, 10, 20));
	m_pTransformCom->Scaling(_vec3(5, 5, 5));
	return NOERROR;
}

_int CStatic_Object::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CStatic_Object::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	
	m_pTransformCom->Update_Matrix();

	CCamera_Debug*	pCamera = (CCamera_Debug*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 0);
	if (nullptr == pCamera)
		return -1;

	_float fRadius = 0.f;

	fRadius = max(m_pMeshCom->Get_MaxPoint()->x, m_pMeshCom->Get_MaxPoint()->y);
	fRadius = max(fRadius, m_pMeshCom->Get_MaxPoint()->z);

	if (false == pCamera->Culling_ToFrustum(m_pTransformCom, nullptr, fRadius))
	{
		if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
			return -1;
	}

	CObject_Manager* pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return -1;

	//// OBB충돌 Player & 

	for (size_t i = 0; i < 1; i++)
	{
		const CGameObject* pPlayer = pObject_Manager->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", i);
		if (nullptr == pPlayer)
			return -1;

		// 디버깅용. 실제론 return true일 때 마다 특정 행동을 취해주자.
		if (true == m_pColliderCom->Collision_OBB((const CCollider*)pPlayer->Get_ComponentPointer(L"Com_Collider"))) 
			break;
	}


	return _int();
}

void CStatic_Object::Render_GameObject()
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

	//m_pColliderCom->Render_Collider();

}

HRESULT CStatic_Object::Ready_Component()
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
	// For.Com_Mesh
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, m_pComponent_Tag);
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	m_pColliderCom = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Box");
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	//_vec3 vTransMore = _vec3(5, 0, 0);
	m_pColliderCom->SetUp_Collider(m_pMeshCom->Get_MinPoint(), m_pMeshCom->Get_MaxPoint(), m_pTransformCom->Get_WorldMatrix());//, &vTransMore);

	//m_pColliderCom->SetUp_Collider(&_vec3(1.f, 1.f, 1.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 0.7f, 0.f));
	//m_pColliderCom->SetUp_Collider(m_pTransformCom->Get_WorldMatrix_NotRot(), &_vec3(1, 1, 1), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 0, 0.f));

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CStatic_Object::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	// For.LightInfo
	const D3DLIGHT9* pLightInfo = CLight_Manager::GetInstance()->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlSpecular", &_vec4(1.f, 1.f, 1.f, 1.f)); // 확인하기 위해 1,1,1,1 로 초기화 해서 처리. (재질정보가 없어서)
	pEffect->SetFloat("g_fPower", 30.0f);
	
	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);


	Safe_Release(pEffect);

	return NOERROR;
}

CStatic_Object * CStatic_Object::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pComponent_Tag)
{
	CStatic_Object*		pInstance = new CStatic_Object(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(pComponent_Tag)))
	{
		_MSG_BOX("CStatic_Object Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CStatic_Object::Clone_GameObject()
{
	CStatic_Object*		pInstance = new CStatic_Object(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CStatic_Object Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatic_Object::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
