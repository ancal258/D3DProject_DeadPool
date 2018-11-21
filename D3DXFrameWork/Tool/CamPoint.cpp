#include "stdafx.h"
#include "CamPoint.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "ToolTerrain.h"

CCamPoint::CCamPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCamPoint::CCamPoint(const CCamPoint & rhs)
	: CGameObject(rhs)
{
}

_vec3 CCamPoint::Get_ObjectInfo()
{
	return *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
}

void CCamPoint::Set_StateInfo(_vec3 * pRight, _vec3 * pUp, _vec3 * vLook, _vec3 * vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, pRight);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, pUp);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);

}

void CCamPoint::Set_Position(_vec3 * pPosition) const
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
}

void CCamPoint::Set_Scale(_vec3 * pScale) const
{
	m_pTransformCom->Scaling(*pScale);
}

void CCamPoint::Set_PlusPosition(_uint iType, _float & fValue) const
{
	m_pTransformCom->Set_PlusPosition(iType, fValue);
}

void CCamPoint::RotationY(const _float & fRadian) const
{
	m_pTransformCom->RotationY(fRadian, 1);
}

void CCamPoint::Scaling(_float fX, _float fY, _float fZ) const
{
	m_pTransformCom->Scaling(fX, fY, fZ);
}


HRESULT CCamPoint::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCamPoint::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;


	return NOERROR;
}

_int CCamPoint::Update_GameObject(const _float & fTimeDelta)
{
	if (CInput_Device::GetInstance()->Get_DIMouseState(CInput_Device::DIM_RBUTTON) & 0x80)
	{
		if (false == m_isLButtonDown)
		{
			const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
			if (STATE_END != ((CToolTerrain*)pTerrain)->Get_MouseState())
			{
				if (FAILED(CInput_Device::GetInstance()->Picking_ToCollider(m_pMesh, m_pTransformCom, &m_isCol)))
					return E_FAIL;
			}
		}
		m_isLButtonDown = true;
	}
	else
	{
		m_isCol = FALSE;
		m_isLButtonDown = false;
		m_isTransformMode = false;

	}


	return _int();
}

_int CCamPoint::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (true == m_isCol)
	{
		CObject_Manager* pObject_Manager = CObject_Manager::GetInstance();
		if (nullptr == pObject_Manager)
			return -1;

		pObject_Manager->AddRef();

		const CGameObject* pTerrain = pObject_Manager->Get_ObjectPointer(0, L"Layer_Terrain", 0);
		if (nullptr == pTerrain)
		{
			Safe_Release(pObject_Manager);
			return -1;
		}

		const CBuffer_Terrain* pBufferCom = (const CBuffer_Terrain*)pTerrain->Get_ComponentPointer(L"Com_Buffer");
		if (nullptr == pBufferCom)
		{
			Safe_Release(pObject_Manager);
			return -1;
		}

		const CTransform* pTransformCom = (const CTransform*)pTerrain->Get_ComponentPointer(L"Com_Transform");
		if (nullptr == pTransformCom)
		{
			Safe_Release(pObject_Manager);
			return -1;
		}

		if (false == m_isTransformMode)
		{
			//_vec3		vPosition = pBufferCom->SetHeight_OnTerrain(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), pTransformCom);
			_vec3		vMousePos = ((CToolTerrain*)pTerrain)->Get_MousePoint();
			//vMousePos.y = vPosition.y;
			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vMousePos);
		}


		if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_LALT) & 0x8000)
		{
			_float tmp = 0.5f;
			Set_PlusPosition(1, tmp);
			m_isTransformMode = true;
		}
		if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_LCONTROL) & 0x8000)
		{
			_float tmp = -0.5f;
			Set_PlusPosition(1, tmp);
			m_isTransformMode = true;
		}
	}


	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	//Safe_Release(pObject_Manager);

	return _int();
}

void CCamPoint::Render_GameObject()
{
	const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
	if (STATE_END != ((CToolTerrain*)pTerrain)->Get_MouseState())
	{
		m_pColliderCom->Render_Collider();
	}
}

HRESULT CCamPoint::Ready_Component()
{
	CComponent_Manager*			pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Com_Transform
	m_pTransformCom = (CTransform*)pComponent_Manager->Clone_Component(0, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	m_pRendererCom = (CRenderer*)pComponent_Manager->Clone_Component(0, L"Component_Renderer");
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For.Com_Collider
	m_pColliderCom = (CCollider*)pComponent_Manager->Clone_Component(0, L"Component_Collider_Sphere");
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	m_pColliderCom->SetUp_Collider(m_pTransformCom->Get_WorldMatrix(),&_vec3(2,2,2),&_vec3(0,0,0),&_vec3(0,0,0));
	m_pMesh = m_pColliderCom->Get_Mesh();
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CCamPoint::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CCamPoint * CCamPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamPoint*		pInstance = new CCamPoint(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("CCamPoint Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamPoint::Clone_GameObject()
{
	CCamPoint*		pInstance = new CCamPoint(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CCamPoint Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamPoint::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
