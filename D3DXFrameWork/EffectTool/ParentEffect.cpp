#include "stdafx.h"
#include "ParentEffect.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "ToolEffect.h"
#include "Effect_BloodT.h"
#include "Effect_ExplosionT.h"
#include "Effect_CloudT.h"

/*
이펙트 더 작업해보고
싸우는거 좀 자연스럽게,,하고
3번재 스테이지 맵 다 찍고
스파크? 트레일? 어떻게? 


*/
CParentEffect::CParentEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CParentEffect::CParentEffect(const CParentEffect & rhs)
	: CGameObject(rhs)
	, m_pProtoTag(rhs.m_pProtoTag)
{
}

void CParentEffect::DeleteChild()
{
	for (auto& pEffect : m_EffectList)
		pEffect->Set_Lived(false);
}

HRESULT CParentEffect::Ready_GameObject_Prototype(const _tchar * pProtoTag)
{
	m_pProtoTag = pProtoTag;

	return NOERROR;
}


HRESULT CParentEffect::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CParentEffect::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;


	if (m_fTimeAcc >= m_fCreateTime)
	{
		CGameObject* pMesh = nullptr;
		_vec3 vPos;
		for (size_t i = 0; i < m_iCreateCnt; i++)
		{
			if (true == m_isSettingPos)
				vPos = m_vSetPos;
			if (true == m_isRandomPos)
			{
				_float iPlusMinus[2];
				iPlusMinus[0] = rand() % 2;
				iPlusMinus[1] = rand() % 2;
				if (iPlusMinus[0] == 0)
					iPlusMinus[0] = 0.5f;
				else
					iPlusMinus[0] = -0.5f;

				if (iPlusMinus[1] == 0)
					iPlusMinus[1] = 0.5f;
				else
					iPlusMinus[1] = -0.5f;

				vPos = _vec3(((_float)(rand() % 1001) / 1000) * m_vSetPos.x, ((_float)(rand() % 1001) / 1000) * m_vSetPos.y, ((_float)(rand() % 1001) / 1000) * m_vSetPos.z);
				vPos.x *= iPlusMinus[0];
				vPos.z *= iPlusMinus[1];
				//vPos = _vec3(m_vSetScale.x / m_vSetScale.x, m_vSetScale.y / m_vSetScale.y, m_vSetScale.z / m_vSetScale.z);
			}
//			m_fDegreeRange = rand() % ((int)fDegreeRange * 2) - fDegreeRange;

			// 애니메이션 텍스쳐 이펙트
			if (0 == m_iType)
			{
				if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, L"Prototype_Explosion", 0, L"Layer_ToolEffect", &pMesh)))
					return -1;
				((CToolEffect*)pMesh)->Set_EffectInfo(this, m_fFrameSpeed, m_fFrameMax, m_fMoveSpeed, m_fSurviveTime, m_fDegreeRange, &m_vSetScale, &vPos, &m_vDir);
			}
			if (1 == m_iType)
			{
				if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, L"Prototype_EffectBlood", 0, L"Layer_ToolEffect", &pMesh)))
					return -1;
				((CEffect_BloodT*)pMesh)->Set_EffectInfo(this, m_fFrameSpeed, m_fFrameMax, m_fMoveSpeed, m_fSurviveTime, m_fDegreeRange, &m_vSetScale, &vPos, &m_vDir);
			}
			if (2 == m_iType)
			{
				if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, L"Prototype_EffectExplosion", 0, L"Layer_ToolEffect", &pMesh)))
					return -1;
				((CEffect_ExplosionT*)pMesh)->Set_EffectInfo(this, m_fFrameSpeed, m_fFrameMax, m_fMoveSpeed, m_fSurviveTime, m_fDegreeRange, &m_vSetScale, &vPos, &m_vDir);
			}




			// 단일 텍스쳐 이펙트
			if (3 == m_iType)
			{
				if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, L"Prototype_EffectCloud", 0, L"Layer_ToolEffect", &pMesh)))
					return -1;
				((CEffect_CloudT*)pMesh)->Set_EffectInfo(this, m_fFrameSpeed, m_fFrameMax, m_fMoveSpeed, m_fSurviveTime, m_fDegreeRange, &m_vSetScale, &vPos, &m_vDir);
			}
			if (4 == m_iType)
			{
				if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, L"Prototype_EffectTexBlood", 0, L"Layer_ToolEffect", &pMesh)))
					return -1;
				((CEffect_CloudT*)pMesh)->Set_EffectInfo(this, m_fFrameSpeed, m_fFrameMax, m_fMoveSpeed, m_fSurviveTime, m_fDegreeRange, &m_vSetScale, &vPos, &m_vDir);
			}
			if (5 == m_iType)
			{
				if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, L"Prototype_EffectBloodMist", 0, L"Layer_ToolEffect", &pMesh)))
					return -1;
				((CEffect_CloudT*)pMesh)->Set_EffectInfo(this, m_fFrameSpeed, m_fFrameMax, m_fMoveSpeed, m_fSurviveTime, m_fDegreeRange, &m_vSetScale, &vPos, &m_vDir);
			}

			m_EffectList.push_back(pMesh);
		}
		m_fTimeAcc = 0.f;
	}
	return _int();
}

_int CParentEffect::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pTransformCom->Update_Matrix();
	m_matParentMatrix = *m_pTransformCom->Get_WorldMatrix();
	return _int();
}

void CParentEffect::Render_GameObject()
{
}

void CParentEffect::Set_EffectInfo(_float fFrameSpeed, _float fFrameMax, _float fMoveSpeed, _float fSurviveTime, _float fDegreeRange, _vec3 * vSetScale, _vec3 * vSetPos, _vec3 * vDir, _bool isSettingPos, _bool isRandomPos, _float fCreateTime, _int iCreateCnt)
{
	m_fFrameSpeed = fFrameSpeed;
	m_fFrameMax = fFrameMax;
	m_fMoveSpeed = fMoveSpeed;
	m_fSurviveTime = fSurviveTime;
	m_fDegreeRange = fDegreeRange;
	m_vSetScale = *vSetScale;
	m_vSetPos = *vSetPos;
	m_vDir = *vDir;
	m_isSettingPos = isSettingPos;
	m_isRandomPos = isRandomPos;
	m_fCreateTime = fCreateTime;
	m_iCreateCnt = iCreateCnt;
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0, 0, 0));
	m_pTransformCom->Scaling(m_vSetScale);
	
}



HRESULT CParentEffect::Ready_Component()
{
	D3DXMatrixIdentity(&m_matParentMatrix);

	m_pTransformCom = (CTransform*)CComponent_Manager::GetInstance()->Clone_Component(0, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	return NOERROR;
}

CParentEffect * CParentEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pProtoTag)
{
	CParentEffect*		pInstance = new CParentEffect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(pProtoTag)))
	{
		_MSG_BOX("Prototype_ParentEffect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParentEffect::Clone_GameObject()
{
	CParentEffect*		pInstance = new CParentEffect(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CParentEffect Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParentEffect::Free()
{
}
