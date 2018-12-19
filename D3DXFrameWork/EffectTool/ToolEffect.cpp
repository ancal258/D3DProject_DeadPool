#include "stdafx.h"
#include "ToolEffect.h"
#include "Component_Manager.h"
#include "ParentEffect.h"


CToolEffect::CToolEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CToolEffect::CToolEffect(const CToolEffect & rhs)
	: CGameObject(rhs)
{

}

HRESULT CToolEffect::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CToolEffect::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(rand() % 10 + 10.0f, 5.f, rand() % 10));

	return NOERROR;
}
void CToolEffect::Set_EffectInfo(CGameObject* pParent, _float fFrameSpeed, _float fFrameMax, _float fMoveSpeed, _float fSurviveTime, _float fDegreeRange, _vec3 * vSetScale, _vec3 * vSetPos, _vec3 * vDir)
{
	m_pParent = pParent;

	m_fFrameSpeed = fFrameSpeed; // 애니메이션 프레임 속도
	m_fFrameMax = fFrameMax; // 애니메이션 최대 프레임
	m_fMoveSpeed = fMoveSpeed; // 이동 속도
	m_fSurviveTime = fSurviveTime; // 생존 시간
	m_fDegreeRange = fDegreeRange; // 회전되어있는 각도 ( 범위 )
	m_vSetScale = *vSetScale; // 생성 크기
	m_vSetPos = *vSetPos; // 생성 위치
	m_vDir = *vDir; // 움직이는 방향 

	if (m_fDegreeRange != 0)
		m_fDegreeRange = rand() % (int)m_fDegreeRange;
	_float fRadian = D3DXToRadian(m_fDegreeRange);

	m_pTransformCom->Scaling(m_vSetScale);
	//m_pTransformCom->Set_AngleZ(fRadian);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &m_vSetPos);
}


_int CToolEffect::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += m_fFrameSpeed * 0.01f;
	m_fTimeAcc += 0.01f;
	if (m_fFrameMax <= m_fFrame)
		m_fFrame = 0.f;

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

	return _int();
}

_int CToolEffect::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if(nullptr != m_pParent)
		m_pTransformCom->Set_ParentMatrix(((CParentEffect*)m_pParent)->Get_ParentMatridx());

	m_pTransformCom->Update_Matrix();


	if (m_fSurviveTime <= m_fTimeAcc)
		Set_Lived(false);

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}

void CToolEffect::Render_GameObject()
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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_VIBuffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CToolEffect::Ready_Component()
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

	// For.Com_Buffer
	m_pBufferCom = (CBuffer_RcTex*)pComponent_Manager->Clone_Component(0, L"Component_Buffer_RcTex");
	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	m_pShaderCom = (CShader*)pComponent_Manager->Clone_Component(0, L"Component_Shader_Effect");
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	m_pTextureCom = (CTexture*)pComponent_Manager->Clone_Component(0, L"Component_Texture_Effect");
	if (FAILED(Add_Component(L"Com_Texture", m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CToolEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_fAlpha", m_fAlpha);
	m_pTextureCom->SetUp_OnShader(pEffect, "g_DiffuseTexture", _uint(m_fFrame));



	Safe_Release(pEffect);

	return NOERROR;
}

CToolEffect * CToolEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CToolEffect*		pInstance = new CToolEffect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSG_BOX("Prototype_CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CToolEffect::Clone_GameObject()
{
	CToolEffect*		pInstance = new CToolEffect(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CToolEffect Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CToolEffect::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
