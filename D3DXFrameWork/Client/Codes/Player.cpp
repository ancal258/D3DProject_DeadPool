#include "stdafx.h"
#include "..\Headers/Player.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Input_Device.h"
#include "Camera_Cinematic.h"
#include "Camera_Debug.h"
#include "Camera_Target.h"


_USING(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{

}

const _matrix * CPlayer::Get_WorldMatrix() const
{
	return m_pTransformCom->Get_WorldMatrix();
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CPlayer::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	m_fMouseSence = 3.f;
	m_pTransformCom->Scaling(0.05f, 0.05f, 0.05f);
	//m_pTransformCom->Set_AngleY(D3DXToRadian(0));
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(15, 0.f, 13));
	//m_pTransformCom->Go_Straight(0.8,1);
	m_pInput_Device = CInput_Device::GetInstance();
	m_pInput_Device->AddRef();

	m_pHandMatrix[0] = m_pMeshCom->Get_FrameMatrixByName("L_Weapon01_Wpn_XW");
	if (nullptr == m_pHandMatrix[0])
		return E_FAIL;
	m_pHandMatrix[1] = m_pMeshCom->Get_FrameMatrixByName("R_Weapon01_Wpn_XW");
	if (nullptr == m_pHandMatrix[1])
		return E_FAIL;
	m_pRootMatrix = m_pMeshCom->Get_FrameMatrixByName("C_Root_Reference_XR");
	if (nullptr == m_pRootMatrix)
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(SIT_IDLE_BREATH);
	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	// Camera SetUp
	if (FAILED(SetUp_Camera()))
		return -1;

	if (m_pInput_Device->Get_DIKeyState(DIK_NUMPAD1) & 0x8000)
	{
		m_Camera_State = CAMERA_DEBUG;
		m_pCamera_Debug->Set_IsCameraOn(true);
		m_pCamera_Target->Set_IsCameraOn(false);
		m_pCamera_Cinematic->Set_IsCameraOn(false);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_NUMPAD2) & 0x8000)
	{
		m_Camera_State = CAMERA_TARGET;
		m_pCamera_Debug->Set_IsCameraOn(false);
		m_pCamera_Target->Set_IsCameraOn(true);
		m_pCamera_Cinematic->Set_IsCameraOn(false);
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_NUMPAD3) & 0x8000)
	{
		m_Camera_State = CAMERA_CINEMATIC;
		m_pCamera_Debug->Set_IsCameraOn(false);
		m_pCamera_Target->Set_IsCameraOn(false);
		m_pCamera_Cinematic->Set_IsCameraOn(true);
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_UP) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_F);
		m_pNavigationCom->Move_OnNavigation(m_pTransformCom, 8.7f, fTimeDelta*0.9f);

	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_DOWN) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_B);
		m_pTransformCom->Go_Straight(-23.3, fTimeDelta*0.9f);
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_Q) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_FL);
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_E) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(NOGUN_WALK_FR);
	}
	else if (m_pInput_Device->Get_DIKeyState(DIK_R) & 0x8000)
	{
		m_pMeshCom->Set_AnimationSet(SIT_GETUP);
	}
	else
	{
		int iIndex = rand() % 6 + 4;
		m_pMeshCom->Set_AnimationSet(SIT_IDLE_BREATH);
	}
	

	// Debug
	if (m_Camera_State != CAMERA_TARGET)
	{
		if (m_pInput_Device->Get_DIKeyState(DIK_RIGHT) & 0x8000)
		{
			m_pTransformCom->RotationY(D3DXToRadian(90.0f), fTimeDelta);
		}
		if (m_pInput_Device->Get_DIKeyState(DIK_LEFT) & 0x8000)
		{
			m_pTransformCom->RotationY(D3DXToRadian(-90.0f), fTimeDelta);
		}
	}
	else
	{
		if (m_dwMouseMove[0] = CInput_Device::GetInstance()->Get_DIMouseMove(CInput_Device::DIMM_X))
		{
		   m_pTransformCom->RotationY(D3DXToRadian(m_dwMouseMove[0] * m_fMouseSence) , fTimeDelta);
		}

		if (m_dwMouseMove[1] = CInput_Device::GetInstance()->Get_DIMouseMove(CInput_Device::DIMM_Y))
		{
		 //  m_pTransformCom->RotationX(D3DXToRadian(m_dwMouseMove[1] * m_fMouseSence), fTimeDelta);
		}
	}


	// Release


	Update_HandMatrix();

	m_pMeshCom->Play_AnimationSet(fTimeDelta*0.9f);


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

	const CGameObject* pTerrain = pObject_Manager->Get_ObjectPointer(SCENE_STAGE, L"Layer_BackGround", 0);
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

	_vec3      vPosition = pBufferCom->SetHeight_OnTerrain(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), pTransformCom);

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

	m_pNavigationCom->Render_Navigation();

	//m_pColliderCom->Render_Collider();

	Render_Axis();

}

void CPlayer::Render_Axis()
{
	_vec3      vPointX[2], vPointY[2], vPointZ[2];

	vPointX[0] = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPointX[1] = vPointX[0] + *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT) * 10;

	vPointY[0] = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPointY[1] = vPointY[0] + *m_pTransformCom->Get_StateInfo(CTransform::STATE_UP) * 10;

	vPointZ[0] = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vPointZ[1] = vPointZ[0] + *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK) * 10;

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

	pLine->DrawTransform(vPointX, 2, &matTransform, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	pLine->DrawTransform(vPointY, 2, &matTransform, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	pLine->DrawTransform(vPointZ, 2, &matTransform, D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	pLine->End();

	Safe_Release(pLine);
}

HRESULT CPlayer::SetUp_Camera()
{
	if (nullptr == m_pCamera_Debug)
		m_pCamera_Debug = (CCamera_Debug*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 0);

	if (nullptr == m_pCamera_Target)
		m_pCamera_Target = (CCamera_Target*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 1);

	if (nullptr == m_pCamera_Cinematic)
	{
		m_pCamera_Cinematic = (CCamera_Cinematic*)CObject_Manager::GetInstance()->Get_ObjectPointer(SCENE_STAGE, L"Layer_Camera", 2);
		if (FAILED(SetUp_CameraMove())) // 나중에 FilePath 받아서 만들자.
			return E_FAIL;
	}
	return NOERROR;
}

HRESULT CPlayer::SetUp_CameraMove()
{

	

	vector<_vec3> vecEye;
	vecEye.push_back(_vec3(24, 30, 1.7f));
	vecEye.push_back(_vec3(23, 20, 24));
	vecEye.push_back(_vec3(46.28, 10, 23.12));
	vecEye.push_back(_vec3(61.73, 20, 12.86));

	m_pCamera_Cinematic->SetUp_CameraMove(vecEye, _vec3(42, 10, 12.86), 10);


	return NOERROR;
}

HRESULT CPlayer::Ready_Component()
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
	m_pMeshCom = (CMesh_Dynamic*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Mesh_Player");
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

	// Form.Com_Collider
	m_pColliderCom = (CCollider*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Collider_Box");
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	m_pColliderCom->SetUp_Collider(&m_CombinedRootMatrix, &_vec3(50, 140, 50), &_vec3(0.0f, 0.f, 0.f), &_vec3(0.f, 70.f, 0.f));

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CPlayer::Update_HandMatrix()
{
	if (nullptr == m_pHandMatrix[0] ||
		nullptr == m_pHandMatrix[1])
		return E_FAIL;

	m_CombinedHandMatrix[0] = *m_pHandMatrix[0] * *m_pTransformCom->Get_WorldMatrix();
	m_CombinedHandMatrix[1] = *m_pHandMatrix[1] * *m_pTransformCom->Get_WorldMatrix();
	m_CombinedRootMatrix = *m_pRootMatrix * *m_pTransformCom->Get_WorldMatrix();
	return NOERROR;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*      pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CPlayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject()
{
	CPlayer*      pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CPlayer Clone Failed");
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
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pInput_Device);

	CGameObject::Free();
}