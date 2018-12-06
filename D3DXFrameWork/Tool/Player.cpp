#include "stdafx.h"
#include "Player.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Input_Device.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{

}

void CPlayer::Set_TrackPosition(DOUBLE dlTrackPosition)
{
	m_pMeshCom->Set_TrackPosition(dlTrackPosition);
}

DOUBLE CPlayer::Get_TrackPeriod()
{
	return	m_pMeshCom->Get_TrackPeriod();
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CPlayer::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Set_AngleY(D3DXToRadian(0));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0, 0.f, 0));

	m_pMeshCom->Set_AnimationSet(0);
	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	//if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD0) & 0x8000)
	//{
	//	m_pMeshCom->Set_AnimationSet(0);
	//}
	//else if(CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD1) & 0x8000)
	//{
	//	m_pMeshCom->Set_AnimationSet(1);
	//}
	//else if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD2) & 0x8000)
	//{
	//	m_pMeshCom->Set_AnimationSet(2);
	//}
	//else if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD3) & 0x8000)
	//{
	//	m_pMeshCom->Set_AnimationSet(3);
	//}
	//else if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD4) & 0x8000)
	//{
	//	m_pMeshCom->Set_AnimationSet(4);
	//}
	//else if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD5) & 0x8000)
	//{
	//	m_pMeshCom->Set_AnimationSet(5);
	//}
	//else if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD6) & 0x8000)
	//{
	//	m_pMeshCom->Set_AnimationSet(6);
	//}
	//else if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_NUMPAD7) & 0x8000)
	//{
	//	m_pMeshCom->Set_AnimationSet(7);
	//}
	

	m_pMeshCom->Set_AnimationSet(m_iAnimationIndex);
	if (true == m_isPlay)
	{
		m_pTransformCom->Go_Straight(m_fSpeed, m_fTimeDelta);

		if ((m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION))->z > m_fArrived)
			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0, 0, 0));
		m_pMeshCom->Play_AnimationSet(m_fTimeDelta);
	}
	else
	{
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0, 0, 0));
	}

	return _int();
}

_int CPlayer::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

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

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);
	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	Safe_Release(pObject_Manager);

	return _int();
}

void CPlayer::Render_GameObject()
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

HRESULT CPlayer::Ready_Component()
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
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(0, L"Component_Mesh_Player");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(0, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	pEffect->SetVector("g_vMtrlDiffuse", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetVector("g_vMtrlAmbient", &_vec4(0.3f, 0.3f, 0.3f, 1.f));
	pEffect->SetVector("g_vMtrlSpecular", &_vec4(1.f, 1.f, 1.f, 1.f));
	pEffect->SetFloat("g_fPower", 20.f);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);


	Safe_Release(pEffect);

	return NOERROR;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*		pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject()
{
	CPlayer*		pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CTest Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
