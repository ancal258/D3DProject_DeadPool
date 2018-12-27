#include "stdafx.h"
#include "../Headers/EffectC.h"
#include "Component_Manager.h"
#include "Parent_Effect.h"
#include "Target_Manager.h"


CEffectC::CEffectC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CEffectC::CEffectC(const CEffectC & rhs)
	: CGameObject(rhs)
{

}

HRESULT CEffectC::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CEffectC::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(rand() % 10 + 10.0f, 5.f, rand() % 10));

	return NOERROR;
}
void CEffectC::Set_EffectInfo(CGameObject* pParent, _float fFrameSpeed, _float fFrameMax, _float fMoveSpeed, _float fSurviveTime, _float fDegreeRange, _vec3 * vSetScale, _vec3 * vSetPos, _vec3 * vDir)
{
	m_pParent = pParent;

	m_fFrameSpeed = fFrameSpeed; // �ִϸ��̼� ������ �ӵ�
	m_fFrameMax = fFrameMax; // �ִϸ��̼� �ִ� ������
	m_fMoveSpeed = fMoveSpeed; // �̵� �ӵ�
	m_fSurviveTime = fSurviveTime; // ���� �ð�
	m_vSetScale = *vSetScale; // ���� ũ��
	m_vSetPos = *vSetPos; // ���� ��ġ
	m_vDir = *vDir; // �����̴� ���� 

	if (0 != fDegreeRange)
		m_fDegreeRange = rand() % ((int)fDegreeRange * 2) - fDegreeRange;
	else
		m_fDegreeRange = fDegreeRange; // ȸ���Ǿ��ִ� ���� ( ���� )

	m_pTransformCom->Scaling(m_vSetScale);

	_matrix		matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fDegreeRange));
	D3DXVec3TransformCoord(&m_vDir, &m_vDir, &matRotZ);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &m_vSetPos);
}


_int CEffectC::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += m_fFrameSpeed * fTimeDelta;
	m_fTimeAcc += fTimeDelta;
	if (m_fFrameMax <= m_fFrame)
		m_fFrame = 0.f;

	Compute_ViewDepth(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));

	_matrix		matView;
	Get_Transform(D3DTS_VIEW, &matView);

	// ī�޶� ȸ�������� ����ȯ ���·� ���� �ִ´�.
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

_int CEffectC::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (nullptr != m_pParent)
		m_pTransformCom->Set_ParentMatrix(((CParent_Effect*)m_pParent)->Get_ParentMatridx());

	_vec3 vPos = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vMove = m_vDir * m_fMoveSpeed;


	vPos += vMove;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);


	m_pTransformCom->Update_Matrix();


	if (m_fSurviveTime <= m_fTimeAcc)
		Set_Lived(false);

	if (FAILED(m_pRendererCom->Add_Render_Group(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}


void CEffectC::Render_GameObject()
{

}

HRESULT CEffectC::Ready_Component()
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

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CEffectC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect, "g_DepthTexture", L"Target_Depth");



	Safe_Release(pEffect);

	return NOERROR;
}

void CEffectC::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}