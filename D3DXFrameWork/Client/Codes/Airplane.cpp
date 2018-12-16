#include "stdafx.h"
#include "..\Headers\Airplane.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Player.h"


#include "Camera_Debug.h"
#include "Camera_Minigun.h"
_USING(Client)

CAirplane::CAirplane(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CAirplane::CAirplane(const CAirplane & rhs)
	: CGameObject(rhs)
{

}

void CAirplane::Set_StateInfo(_vec3* pRight, _vec3* pUp, _vec3* vLook, _vec3* vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, pRight);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, pUp);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
}

const _matrix * CAirplane::Get_WorldMatrix() const
{
	return m_pTransformCom->Get_WorldMatrix();
}

HRESULT CAirplane::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CAirplane::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Scaling(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(41.64, 5.5f, 46.22));
	m_fSpeed = 10.f;
	Load_Path(nullptr);
	return NOERROR;
}

_int CAirplane::Update_GameObject(const _float & fTimeDelta)
{
	if (FAILED(SetUp_Camera()))
		return -1;

	if (4 * m_iCurrentIndex >= m_vecPath.size())
	{
		// 카메라 끝 시점
		m_dlCurrentOffset = 1;
		m_isFinish = true;
	}
	else
	{
		m_dlCurrentOffset += (fTimeDelta / m_fSpeed);
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &Bezier4(m_vecPath[0 + 4 * m_iCurrentIndex], m_vecPath[1 + 4 * m_iCurrentIndex], m_vecPath[2 + 4 * m_iCurrentIndex], m_vecPath[3 + 4 * m_iCurrentIndex], m_dlCurrentOffset));
	}

	if (m_dlCurrentOffset >= 1)
	{
		m_dlCurrentOffset = 0;
		++m_iCurrentIndex;
	}

	return _int();
}

_int CAirplane::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)

		return -1;

	m_pTransformCom->Update_Matrix();

	_matrix matTmp = *m_pTransformCom->Get_WorldMatrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;
	return _int();
}

void CAirplane::Render_GameObject()
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

HRESULT CAirplane::Ready_Component()
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
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Helicopter");
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

HRESULT CAirplane::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CAirplane::SetUp_Camera()
{
	if (nullptr == m_pCamera_Debug)
		m_pCamera_Debug = (CCamera_Debug*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 0);

	if (nullptr == m_pCamera_Minigun)
		m_pCamera_Minigun = (CCamera_Minigun*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 1);


	if (nullptr != m_pCamera_Debug&&
		nullptr != m_pCamera_Minigun)
	{
		if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD8) & 0x8000)
		{
			m_pCamera_Debug->Set_IsCameraOn(true);
			m_pCamera_Minigun->Set_IsCameraOn(false);
		}
		if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD9) & 0x8000)
		{
			m_pCamera_Debug->Set_IsCameraOn(false);
			m_pCamera_Minigun->Set_IsCameraOn(true);
		}
	}

	return NOERROR;
}

HRESULT CAirplane::Load_Path(_tchar * pFilePath)
{
	m_vecPath.push_back(_vec3(41.64, 5.5f, 46.22));
	m_vecPath.push_back(_vec3(45.64, 8.5f, 40.22));
	m_vecPath.push_back(_vec3(52.64, 12.5f, 56.22));
	m_vecPath.push_back(_vec3(61.64, 8.5f, 66.22));

	return NOERROR;
}

_vec3 CAirplane::Bezier4(_vec3 vPoint_1, _vec3 vPoint_2, _vec3 vPoint_3, _vec3 vPoint_4, double Offset)
{
	double OffsetA, OffsetB, OffsetC;

	_vec3 vResult;

	OffsetA = 1 - Offset;
	OffsetB = OffsetA * OffsetA * OffsetA;
	OffsetC = Offset * Offset * Offset;

	vResult.x = OffsetB*vPoint_1.x + 3 * Offset*OffsetA*OffsetA*vPoint_2.x + 3 * Offset*Offset*OffsetA*vPoint_3.x + OffsetC*vPoint_4.x;
	vResult.y = OffsetB*vPoint_1.y + 3 * Offset*OffsetA*OffsetA*vPoint_2.y + 3 * Offset*Offset*OffsetA*vPoint_3.y + OffsetC*vPoint_4.y;
	vResult.z = OffsetB*vPoint_1.z + 3 * Offset*OffsetA*OffsetA*vPoint_2.z + 3 * Offset*Offset*OffsetA*vPoint_3.z + OffsetC*vPoint_4.z;

	return(vResult);
}
 
CAirplane * CAirplane::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAirplane*		pInstance = new CAirplane(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CAirplane Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAirplane::Clone_GameObject()
{
	CAirplane*		pInstance = new CAirplane(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CAirplane Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAirplane::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	CGameObject::Free();
}
