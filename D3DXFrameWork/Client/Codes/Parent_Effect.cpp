#include "stdafx.h"
#include "Parent_Effect.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "EffectC.h"

/*
3번재 스테이지 맵 다 찍고
스파크? 트레일? 어떻게? 


*/
CParent_Effect::CParent_Effect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CParent_Effect::CParent_Effect(const CParent_Effect & rhs)
	: CGameObject(rhs)
	, m_pProtoTag(rhs.m_pProtoTag)
{
}

void CParent_Effect::DeleteChild()
{
	for (auto& pEffect : m_EffectList)
		pEffect->Set_Lived(false);
}

HRESULT CParent_Effect::Ready_GameObject_Prototype(const _tchar * pProtoTag)
{
	m_pProtoTag = pProtoTag;

	return NOERROR;
}


HRESULT CParent_Effect::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CParent_Effect::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;


	if (m_fTimeAcc >= m_fCreateTime)
	{
		CGameObject* pMesh = nullptr;
		_vec3 vPos;
		for (size_t i = 0; i < m_iCreateCnt; i++)
		{
			if (true == m_isSettingPos)
				vPos = _vec3(0,0,0);
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

				vPos = _vec3(((_float)(rand() % 1001) / 1000) * m_vSetPosRange.x, ((_float)(rand() % 1001) / 1000) * m_vSetPosRange.y, ((_float)(rand() % 1001) / 1000) * m_vSetPosRange.z);
				vPos.x *= iPlusMinus[0];
				vPos.z *= iPlusMinus[1];
				//vPos = _vec3(m_vSetScale.x / m_vSetScale.x, m_vSetScale.y / m_vSetScale.y, m_vSetScale.z / m_vSetScale.z);
			}

			// 애니메이션 텍스쳐 이펙트


			// 생성 시간마다 계속 생성.
			if (FAILED(CObject_Manager::GetInstance()->Add_Object(SCENE_STAGE, m_pProtoTag, SCENE_STAGE, L"Layer_Effect", &pMesh)))
				return -1;
			((CEffectC*)pMesh)->Set_EffectInfo(this, m_fFrameSpeed, m_fFrameMax, m_fMoveSpeed, m_fSurviveTime, m_fDegreeRange, &m_vSetScale, &vPos, &m_vDir);


			m_EffectList.push_back(pMesh);
		}
		m_fTimeAcc = 0.f;
	}
	return _int();
}

_int CParent_Effect::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pTransformCom->Update_Matrix();
	m_matParentMatrix = *m_pTransformCom->Get_WorldMatrix();
	return _int();
}

void CParent_Effect::Render_GameObject()
{
}

void CParent_Effect::Set_EffectInfo(_float fFrameSpeed, _float fFrameMax, _float fMoveSpeed, _float fSurviveTime, 
	_float fDegreeRange, _vec3 * vSetScale, _vec3 * vSetPos, _vec3 * vSetPosRange, _vec3 * vDir, _bool isSettingPos, _bool isRandomPos, _float fCreateTime, _int iCreateCnt)
{
	m_fFrameSpeed = fFrameSpeed;
	m_fFrameMax = fFrameMax;
	m_fMoveSpeed = fMoveSpeed;
	m_fSurviveTime = fSurviveTime;
	m_fDegreeRange = fDegreeRange;
	m_vSetScale = *vSetScale;
	m_vSetPos = *vSetPos;
	m_vSetPosRange = *vSetPosRange;
	m_vDir = *vDir;
	m_isSettingPos = isSettingPos;
	m_isRandomPos = isRandomPos;
	m_fCreateTime = fCreateTime;
	m_iCreateCnt = iCreateCnt;
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_vSetPos));
	m_pTransformCom->Scaling(m_vSetScale);
	
}



HRESULT CParent_Effect::Ready_Component()
{
	D3DXMatrixIdentity(&m_matParentMatrix);

	m_pTransformCom = (CTransform*)CComponent_Manager::GetInstance()->Clone_Component(0, L"Component_Transform");
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	return NOERROR;
}

CParent_Effect * CParent_Effect::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pProtoTag)
{
	CParent_Effect*		pInstance = new CParent_Effect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype(pProtoTag)))
	{
		_MSG_BOX("Prototype_ParentEffect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParent_Effect::Clone_GameObject()
{
	CParent_Effect*		pInstance = new CParent_Effect(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX("CParent_Effect Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParent_Effect::Free()
{
	Safe_Release(m_pTransformCom);
	CGameObject::Free();
}
