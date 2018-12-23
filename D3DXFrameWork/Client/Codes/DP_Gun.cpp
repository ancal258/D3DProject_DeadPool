#include "stdafx.h"
#include "..\Headers\DP_Gun.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Player.h"
#include "Effect_FireSpears.h"
_USING(Client)

CDP_Gun::CDP_Gun(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CDP_Gun::CDP_Gun(const CDP_Gun & rhs)
	: CGameObject(rhs)
	, m_iSide(rhs.m_iSide)
{
}

HRESULT CDP_Gun::Ready_GameObject_Prototype(_uint iSide)
{
	m_iSide = iSide;
	return NOERROR;
}

HRESULT CDP_Gun::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(SetUp_PlayerPointer()))
		return E_FAIL;

	//if (0 == m_iSide)
	//	m_pTransformCom->Set_AngleZ(D3DXToRadian(90.0f));
	if (1 == m_iSide)
		m_pTransformCom->Set_AngleZ(D3DXToRadian(180.0f));

	return NOERROR;
}

_int CDP_Gun::Update_GameObject(const _float & fTimeDelta)
{
	//		STATE_SWORD, STATE_AIM, STATE_RUN, STATE_NORMAL, STATE_END
	if (1 == m_pPlayer->Get_AnimState())
		m_isActive = true;
	else
		m_isActive = false;

	if (true == (m_pPlayer->Get_IsButtonDown(7)))
	{
		CGameObject* pEffect;

		if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, L"Prototype_Effect_FireSpears", SCENE_STAGE, L"Layer_Effect", &pEffect)))
			return -1;
		_vec3 vPos = (_vec3)m_pTransformCom->Get_WorldMatrix()->m[3];
		((CEffect_FireSpears*)pEffect)->Set_Position(vPos);
		//((CEffect_FireSpears*)pEffect)->Set_ParentMatrix(m_pTransformCom->Get_WorldMatrix());

	}
	

	return _int();
}

_int CDP_Gun::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	if (m_isActive == false)
		return 0;
	m_pTransformCom->Set_ParentMatrix(m_pPlayer->Get_HandMatrix(m_iSide));

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_PLAYER, this)))
		return -1;


	return _int();
}

void CDP_Gun::Render_GameObject()
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

	//Render_Axis();
}

void CDP_Gun::Render_Axis()
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

HRESULT CDP_Gun::Ready_Component()
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
	m_pMeshCom = (CMesh_Static*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Gun");
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;


	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CDP_Gun::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CDP_Gun::SetUp_PlayerPointer()
{
	CObject_Manager*			pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;

	pObject_Manager->AddRef();

	m_pPlayer = (CPlayer*)pObject_Manager->Get_ObjectPointer(SCENE_STAGE, L"Layer_Player", 0);
	if (nullptr == m_pPlayer)
		return E_FAIL;

	Safe_Release(pObject_Manager);

	return NOERROR;
}

CDP_Gun * CDP_Gun::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iSide)
{
	CDP_Gun*		pInstance = new CDP_Gun(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(iSide)))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDP_Gun::Clone_GameObject()
{
	CDP_Gun*		pInstance = new CDP_Gun(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CDP_Gun Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDP_Gun::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	CGameObject::Free();
}
