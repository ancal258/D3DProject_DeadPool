#include "stdafx.h"
#include "..\Headers\Object_Collider.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Buffer_Collider.h"

CObject_Collider::CObject_Collider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CObject_Collider::CObject_Collider(const CObject_Collider & rhs)
	:CGameObject(rhs)
{
}

void CObject_Collider::Set_Scale(_vec3 vScale)
{
	m_vScale = vScale;
	m_pTransformCom->Scaling(m_vScale);
}

bool CObject_Collider::IsCollision(COLLIDER tagDestCollider)
{
	if (m_tagCollider.vMax.x < tagDestCollider.vMin.x || m_tagCollider.vMin.x > tagDestCollider.vMax.x)
		return false;
	if (m_tagCollider.vMax.y < tagDestCollider.vMin.y || m_tagCollider.vMin.y > tagDestCollider.vMax.y)
		return false;
	if (m_tagCollider.vMax.z < tagDestCollider.vMin.z || m_tagCollider.vMin.z > tagDestCollider.vMax.z)
		return false;



	return true;
}

void CObject_Collider::Update_ColliderInfo()
{
	m_tagCollider.vCenter = m_vPosition;
	m_tagCollider.vMin = _vec3(m_vPosition.x - m_vScale.x*0.5f, m_vPosition.y - m_vScale.y*0.5f, m_vPosition.z - m_vScale.z*0.5f);
	m_tagCollider.vMax = _vec3(m_vPosition.x + m_vScale.x*0.5f, m_vPosition.y + m_vScale.y*0.5f, m_vPosition.z + m_vScale.z*0.5f);
}

HRESULT CObject_Collider::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CObject_Collider::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	//m_pTransformCom->Scaling(10, 10, 10);

	return NOERROR;
}

_int CObject_Collider::Update_GameObject(const _float & fTimeDelta)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &m_vPosition);
	Update_ColliderInfo();
	return _int();
}

_int CObject_Collider::LastUpdate_GameObject(const _float & fTimeDelta)
{

	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

void CObject_Collider::Render_GameObject()
{
	if (nullptr == m_pShaderCom)
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

HRESULT CObject_Collider::Ready_Component()
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
	m_pBufferCom = (CBuffer_Collider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Buffer_Collider");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Collider");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CObject_Collider::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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


	Safe_Release(pEffect);

	return NOERROR;
}

CObject_Collider * CObject_Collider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CObject_Collider*		pInstance = new CObject_Collider(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CObject_Collider Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CObject_Collider::Clone_GameObject()
{
	CObject_Collider*		pInstance = new CObject_Collider(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CObject_Collider Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Collider::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
