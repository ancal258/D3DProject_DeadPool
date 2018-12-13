#include "stdafx.h"
#include "Static_Object.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "ToolTerrain.h"

CStatic_Object::CStatic_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CStatic_Object::CStatic_Object(const CStatic_Object & rhs)
	: CGameObject(rhs)
	, m_pComponent_Tag(rhs.m_pComponent_Tag)
	, m_pPrototype_Tag(rhs.m_pPrototype_Tag)
{
}

STATIC_OBJECT_INFO CStatic_Object::Get_ObjectInfo()
{
	STATIC_OBJECT_INFO tObjectInfo;
	ZeroMemory(&tObjectInfo, sizeof(STATIC_OBJECT_INFO));
	_matrix matWorld = *m_pTransformCom->Get_WorldMatrix();
	lstrcpy(tObjectInfo.szPrototype_Tag, m_pPrototype_Tag);

	tObjectInfo.vRight = matWorld.m[0];
	tObjectInfo.vUp = matWorld.m[1];
	tObjectInfo.vLook = matWorld.m[2];
	tObjectInfo.vPos = matWorld.m[3];
	tObjectInfo.isOffCulling = m_isOffCulling;

	return tObjectInfo;
}

void CStatic_Object::Set_StateInfo(_vec3 * pRight, _vec3 * pUp, _vec3 * vLook, _vec3 * vPos, _bool isOffCulling)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, pRight);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, pUp);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, vLook);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, vPos);
	m_isOffCulling = isOffCulling;
	m_isLock = true;
}

void CStatic_Object::Set_Position(_vec3 * pPosition) const
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
}

void CStatic_Object::Set_Scale(_vec3 * pScale) const
{
	m_pTransformCom->Scaling(*pScale);
}

void CStatic_Object::Set_PlusPosition(_uint iType, _float & fValue) const
{
	m_pTransformCom->Set_PlusPosition(iType, fValue);
}

void CStatic_Object::RotationY(const _float & fRadian) const
{
	m_pTransformCom->RotationY(fRadian, 1);
}

void CStatic_Object::Scaling(_float fX, _float fY, _float fZ) const
{
	m_pTransformCom->Scaling(fX, fY, fZ);
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


	return NOERROR;
}

_int CStatic_Object::Update_GameObject(const _float & fTimeDelta)
{
	if (true == m_isLock)
		return 0;
	BOOL pHit = FALSE;
	if (CInput_Device::GetInstance()->Get_DIMouseState(CInput_Device::DIM_LBUTTON) & 0x80)
	{
		if (false == m_isLButtonDown)
		{
			const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
			if (STATE_END != ((CToolTerrain*)pTerrain)->Get_MouseState())
			{
				if (FAILED(CInput_Device::GetInstance()->Picking_ToMesh(m_pMeshCom, m_pTransformCom, &pHit)))
					return E_FAIL;
			}
		}
		m_isLButtonDown = true;
	}
	else
		m_isLButtonDown = false;
	if (TRUE == pHit)
		m_isCol ^= 0x1;

	return _int();
}

_int CStatic_Object::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (true == m_isLock)
		m_isCol = false;

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

		_vec3		vPosition = pBufferCom->SetHeight_OnTerrain(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), pTransformCom);
		if (false == m_isTransformMode)
		{
			_vec3		vMousePos = ((CToolTerrain*)pTerrain)->Get_MousePoint();
			vMousePos.y = vPosition.y;
			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vMousePos);
		}
	}


	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	//Safe_Release(pObject_Manager);

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

	const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
	if (STATE_END != ((CToolTerrain*)pTerrain)->Get_MouseState())
	{
		//m_pColliderCom->Render_Collider();
	}
}

HRESULT CStatic_Object::Ready_Component()
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

	// For.Com_Mesh
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(0, m_pComponent_Tag);
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(0, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;
	// For.Com_Collider
	m_pColliderCom = (CCollider*)pComponent_Manager->Clone_Component(0, L"Component_Collider_Box");
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	m_pColliderCom->SetUp_Collider(m_pMeshCom->Get_MinPoint(), m_pMeshCom->Get_MaxPoint(), m_pTransformCom->Get_WorldMatrix());

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

	CGameObject::Free();
}
