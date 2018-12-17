#include "stdafx.h"
#include "..\Headers\Minigun.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Airplane.h"
#include "Input_Device.h"
_USING(Client)

CMinigun::CMinigun(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CMinigun::CMinigun(const CMinigun & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMinigun::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CMinigun::Ready_GameObject()
{

	if (FAILED(CMinigun::Ready_Component()))
		return E_FAIL;

	m_pAirplane = CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Airplane", 0);
	if (nullptr == m_pAirplane)
		return E_FAIL;

	m_fMouseSence = 8.1f;

	//m_pBoneA = m_pMeshCom->Get_FrameMatrix("L_Misc02_XM");
	//if (nullptr == m_pBoneA)
	//	return E_FAIL;
	//m_pBoneB = m_pMeshCom->Get_FrameMatrix("L_Misc03_XM");
	//if (nullptr == m_pBoneA)
	//	return E_FAIL;
	//m_pBoneC = m_pMeshCom->Get_FrameMatrix("L_Misc04_XM");
	//if (nullptr == m_pBoneC)
	//	return E_FAIL;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(160, 100, 60));


	//Update_HandMatrix();
	m_pMeshCom->Set_AnimationSet(0);

	return NOERROR;
}

_int CMinigun::Update_GameObject(const _float & fTimeDelta)
{
	//_matrix	matRotY;
	//D3DXMatrixRotationY(&matRotY, 1 * fTimeDelta);

	//D3DXVec3TransformNormal((_vec3*)&m_pBoneA->m[0][0], (_vec3*)&m_pBoneA->m[0][0], &matRotY);
	//D3DXVec3TransformNormal((_vec3*)&m_pBoneA->m[1][0], (_vec3*)&m_pBoneA->m[1][0], &matRotY);
	//D3DXVec3TransformNormal((_vec3*)&m_pBoneA->m[2][0], (_vec3*)&m_pBoneA->m[2][0], &matRotY);

	if (m_dwMouseMove = CInput_Device::GetInstance()->Get_DIMouseMove(CInput_Device::DIMM_X))
	{
		if (m_dwMouseMove < 0)
		{
			if (m_dwMouseAcc[0] > -730)
			{
				m_dwMouseAcc[0] += m_dwMouseMove;
				m_pTransformCom->RotationAxis(_vec3(0,1,0) ,D3DXToRadian(m_dwMouseMove * m_fMouseSence), fTimeDelta);
			}
		}
		else
		{
			if (m_dwMouseAcc[0] < 553)
			{
				m_dwMouseAcc[0] += m_dwMouseMove;
				m_pTransformCom->RotationAxis(_vec3(0, 1, 0), D3DXToRadian(m_dwMouseMove * m_fMouseSence), fTimeDelta);
			}
		}
	}

	if (m_dwMouseMove = CInput_Device::GetInstance()->Get_DIMouseMove(CInput_Device::DIMM_Y))
	{
		if (m_dwMouseMove < 0)
		{
			if (m_dwMouseAcc[1] > -400)
			{
				m_dwMouseAcc[1] += m_dwMouseMove;
				m_pTransformCom->RotationAxis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(m_dwMouseMove * m_fMouseSence), fTimeDelta);
			}
		}
		else
		{
			if (m_dwMouseAcc[1] < 580)
			{
				m_dwMouseAcc[1] += m_dwMouseMove;
				m_pTransformCom->RotationAxis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(m_dwMouseMove * m_fMouseSence), fTimeDelta);
			}
		}
	}




	return _int();
}

_int CMinigun::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	m_pTransformCom->Set_ParentMatrix(*dynamic_cast<const CAirplane*>(m_pAirplane)->Get_WorldMatrix());
	_vec3 vTest = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	m_pTransformCom->Update_Matrix();



	m_pMeshCom->Set_AnimationSet(0);
	m_pMeshCom->Play_AnimationSet(fTimeDelta);


	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_NONEALPHA, this)))
		return -1;
	return _int();
}

void CMinigun::Render_GameObject()
{
	if (nullptr == m_pMeshCom ||
		nullptr == m_pShaderCom)
		return;

	//m_matRotationY = m_pMeshCom->Get_FrameMatrix("L_Misc02_XM");

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

	//Render_Axis();
}

HRESULT CMinigun::Ready_Component()
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
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Minigun");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Navigation
	m_pNavigationCom = (CNavigation*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Navigation");
	if (FAILED(Add_Component(L"Com_Navigation", m_pNavigationCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CMinigun::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CMinigun::Render_Axis()
{
	_vec3      vPointX[2], vPointY[2], vPointZ[2];
	_vec3	   vRight = m_pTransformCom->Get_WorldMatrix()->m[0];
	_vec3	   vUp = m_pTransformCom->Get_WorldMatrix()->m[1];
	_vec3	   vLook = m_pTransformCom->Get_WorldMatrix()->m[2];
	vPointX[0] = m_pTransformCom->Get_WorldMatrix()->m[3]; // Get_StateInfo(CTransform::STATE_POSITION);
	vPointX[1] = vPointX[0] + vRight * 50;

	vPointY[0] = m_pTransformCom->Get_WorldMatrix()->m[3];
	vPointY[1] = vPointY[0] + vUp * 50;

	vPointZ[0] = m_pTransformCom->Get_WorldMatrix()->m[3];
	vPointZ[1] = vPointZ[0] + vLook * 50;

	LPD3DXLINE            pLine = nullptr;
	if (FAILED(D3DXCreateLine(Get_Graphic_Device(), &pLine)))
		return;

	_matrix      matView, matProj;

	Get_Graphic_Device()->GetTransform(D3DTS_VIEW, &matView);
	Get_Graphic_Device()->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix      matTransform;
	D3DXMatrixIdentity(&matTransform);

	for (size_t i = 0; i < 2; ++i)
	{
		D3DXVec3TransformCoord(&vPointX[i], &vPointX[i], &matView);
		D3DXVec3TransformCoord(&vPointX[i], &vPointX[i], &matProj);

		D3DXVec3TransformCoord(&vPointY[i], &vPointY[i], &matView);
		D3DXVec3TransformCoord(&vPointY[i], &vPointY[i], &matProj);

		D3DXVec3TransformCoord(&vPointZ[i], &vPointZ[i], &matView);
		D3DXVec3TransformCoord(&vPointZ[i], &vPointZ[i], &matProj);

	}

	pLine->SetWidth(2.0f);

	pLine->Begin();

	pLine->DrawTransform(vPointX, 2, &matTransform, D3DXCOLOR(1.f, 0.f, 0.f, 1.f)); // Right - Red
	pLine->DrawTransform(vPointY, 2, &matTransform, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)); // Up  - Green
	pLine->DrawTransform(vPointZ, 2, &matTransform, D3DXCOLOR(0.f, 0.f, 1.f, 1.f)); // Look - Blue

	pLine->End();

	Safe_Release(pLine);
}

CMinigun * CMinigun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMinigun*		pInstance = new CMinigun(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMinigun::Clone_GameObject()
{
	CMinigun*		pInstance = new CMinigun(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CMinigun Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMinigun::Update_HandMatrix()
{

	return NOERROR;
}

void CMinigun::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pMeshCom);

	CGameObject::Free();
}
