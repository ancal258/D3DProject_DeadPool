#include "stdafx.h"
#include "..\Headers\Static_Object.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Player.h"
#include "Airplane.h"

#include "Camera_Target.h"
#include "Parent_Effect.h"
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

void CStatic_Object::Set_StateInfo(_vec3* pRight, _vec3* pUp, _vec3* vLook, _vec3* vPos, _bool isOffCulling, _bool isExplosion)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, pRight);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, pUp);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
	m_isOffCulling = isOffCulling;
	m_isExplosion = isExplosion;
}
HRESULT CStatic_Object::SetUp_Radius()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	_vec3 vMaxPoint = *m_pMeshCom->Get_MaxPoint();
	D3DXVec3TransformCoord(&vMaxPoint, &vMaxPoint, m_pTransformCom->Get_WorldMatrix());

	m_fRadius = max(vMaxPoint.x, vMaxPoint.y);
	m_fRadius = max(m_fRadius, vMaxPoint.z);
	m_fRadius *= 2;
	return NOERROR;
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
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(20, 10, 20));
	//m_pTransformCom->Scaling(_vec3(5, 5, 5));
	m_pAirplane = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Airplane", 0);

	m_fRadius = 550.f;
	return NOERROR;
}

_int CStatic_Object::Update_GameObject(const _float & fTimeDelta)
{

	if (FAILED(isHitScan()))
		return E_FAIL;

	m_isCol = false;
	return _int();
}

_int CStatic_Object::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	m_pTransformCom->Update_Matrix();

	CCamera_Target*	pCamera = (CCamera_Target*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 1);
	if (nullptr == pCamera)
		return -1;


	if (nullptr != m_pAirplane)
	{
		_vec3 vAirplanePos = static_cast<_vec3>((dynamic_cast<const CAirplane*>(m_pAirplane)->Get_WorldMatrix()->m[3]));
		_vec3 vMyPos = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
		//_vec3 vBrawlerPos = static_cast<_vec3>(m_pTransformCom->Get_WorldMatrix()->m[3]);
		_vec3 vDir = vAirplanePos - vMyPos;
		_float fLength = D3DXVec3Length(&vDir);


		if (true == m_isOffCulling)
		{

			if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
				return -1;
		}
		else
		{
			if (fLength < 110)
			{

				if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
					return -1;
			}
		}
	}
	else
	{
		if (false == m_isOffCulling)
		{
			if (false == pCamera->Culling_ToFrustum(m_pTransformCom, nullptr, m_fRadius))
			{
				if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
					return -1;
			}
		}
		else
		{
			if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
				return -1;
		}
	}

	if (2 == m_iStage)
	{
		if (TRUE == m_Hit)
		{
			m_isDamaged = CInput_Device::GetInstance()->Is_MinDist(m_fDist);
		}

		if (true == m_isExplosion)
		{
			// 충돌 되면
			if (true == m_isDamaged)
			{
				CGameObject* pEffect;
				_vec3 vPos = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
				if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Parent_Explosion", SCENE_STAGE, L"Layer_Effect", &pEffect)))
					return E_FAIL;
				((CParent_Effect*)pEffect)->Set_EffectInfo(64, 64, 0.0f, 1.8f, 0, &_vec3(10, 10, 10), &vPos, &_vec3(0.5f, 0.5f, 0.5f), &_vec3(0, 0, 0), false, true, 0.1f, 3, 0.5f);


				if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Parent_CloudDark", SCENE_STAGE, L"Layer_Effect", &pEffect)))
					return E_FAIL;
				((CParent_Effect*)pEffect)->Set_EffectInfo(0, 0, 0.003f, 7, 0, &_vec3(5, 5, 5), &vPos, &_vec3(1, 0, 1), &_vec3(0, 1, 0), false, true, 0.5f, 5, 3);
				// 폭발 생성
				Set_Lived(false);

				((CAirplane*)m_pAirplane)->Plus_DestroyPoint();
			}
		}
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
	m_pColliderMesh = m_pColliderCom->Get_Mesh();

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

	pEffect->SetBool("g_isCol", m_isCol);

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CStatic_Object::isHitScan()
{
	m_Hit = FALSE;

	if (FAILED(CInput_Device::GetInstance()->Picking_ToCollider(m_pColliderMesh, m_pTransformCom, &m_Hit, &m_fDist)))
		return E_FAIL;

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
