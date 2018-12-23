#include "stdafx.h"
#include "..\Headers\Effect_FireSpears.h"
#include "Component_Manager.h"

_USING(Client)

CEffect_FireSpears::CEffect_FireSpears(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CEffect_FireSpears::CEffect_FireSpears(const CEffect_FireSpears & rhs)
	: CGameObject(rhs)
{

}

void CEffect_FireSpears::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);

	//D3DXMatrixIdentity(&m_matParentMatrix);
	//_vec3			vRight, vUp, vLook, vPosition;

	//vRight = *D3DXVec3Normalize(&vRight, (_vec3*)&m_matParentMatrix.m[0][0]) * 0.2f;
	//vUp = *D3DXVec3Normalize(&vUp, (_vec3*)&m_matParentMatrix.m[1][0]) * 0.2f;
	//vLook = *D3DXVec3Normalize(&vLook, (_vec3*)&m_matParentMatrix.m[2][0]) * 0.2f;
	//vPosition = vPos;
	//memcpy(&m_matParentMatrix.m[0][0], vRight, sizeof(_vec3));
	//memcpy(&m_matParentMatrix.m[1][0], vUp, sizeof(_vec3));
	//memcpy(&m_matParentMatrix.m[2][0], vLook, sizeof(_vec3));
	//memcpy(&m_matParentMatrix.m[3][0], vPosition, sizeof(_vec3));

}

HRESULT CEffect_FireSpears::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEffect_FireSpears::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Scaling(0.1, 0.3, 1);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0,0,0));

	// 빌보드 이펙트의 상태 변환을 위해
	// 부모 행렬을 직접 만들어준다.




	return NOERROR;
}

_int CEffect_FireSpears::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += 28.f * fTimeDelta;

	if (4.f <= m_fFrame)
	{
		Set_Lived(false);
		m_fFrame = 0.f;
	}

	Compute_ViewDepth(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));

	_matrix		matView;
	Get_Transform(D3DTS_VIEW, &matView);

	// 카메라 회전상태의 역변환 상태로 갖고 있는다.
	_matrix		matBill;
	D3DXMatrixIdentity(&matBill);

	memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vec3));
	memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vec3));
	memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vec3));

	D3DXMatrixInverse(&matBill, nullptr, &matBill);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, (_vec3*)&matBill.m[0][0]);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, (_vec3*)&matBill.m[1][0]);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, (_vec3*)&matBill.m[2][0]);
	//D3DPT_TRIANGLESTRIP
	return _int();
}

_int CEffect_FireSpears::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	//m_pTransformCom->Set_ParentMatrix(*m_pParentMatrix);

	m_pTransformCom->Update_Matrix();

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}

void CEffect_FireSpears::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pShaderCom)
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

HRESULT CEffect_FireSpears::Ready_Component()
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

	// For.Com_Buffer
	m_pBufferCom = (CBuffer_RcTex*)pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(SCENE_STAGE, L"Component_Texture_Effect_FireSpears");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEffect_FireSpears::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_fAlpha", 1);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", _uint(0));



	Safe_Release(pEffect);

	return NOERROR;
}

CEffect_FireSpears * CEffect_FireSpears::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect_FireSpears*		pInstance = new CEffect_FireSpears(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect_FireSpears::Clone_GameObject()
{
	CEffect_FireSpears*		pInstance = new CEffect_FireSpears(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CEffect_FireSpears Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_FireSpears::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
