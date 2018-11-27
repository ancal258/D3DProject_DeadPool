#include "..\Headers\AnimationCtrl.h"



CAnimationCtrl::CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	:m_pAniCtrl(pAniCtrl)
{
	m_pAniCtrl->AddRef();
}

CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl & rhs)
	:m_pAniCtrl(nullptr)
{
	//( 출력될 갯수?, X파일 내에 정의되어 있는 애니메이션 갯수 , 
	if (FAILED(rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(), rhs.m_pAniCtrl->GetMaxNumAnimationSets(), rhs.m_pAniCtrl->GetMaxNumTracks(),
		rhs.m_pAniCtrl->GetMaxNumEvents(), &m_pAniCtrl)))
		return;

	m_vecAnimSet.reserve(m_pAniCtrl->GetMaxNumAnimationSets());
	for (size_t i = 0; i < m_pAniCtrl->GetMaxNumAnimationSets(); i++)
	{
		LPD3DXANIMATIONSET pAS = nullptr;
		m_pAniCtrl->GetAnimationSet(i, &pAS);
		m_vecAnimSet.push_back(pAS);
	}
}

HRESULT CAnimationCtrl::Ready_AnimationCtrl()
{
	return NOERROR;
}


// 애니메이션이 바뀔 때 마다 처리됨.
HRESULT CAnimationCtrl::Set_AnimationSet(const _uint & iIndex)
{
	if (m_pCurrentAnimSet == m_vecAnimSet[iIndex])
		return NOERROR;
	// 애니메이션 컨트롤러에 저장된 애니메이션 셋을 얻어온다.
	m_pCurrentAnimSet = m_vecAnimSet[iIndex];

	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;


	// 새로운 트랙에 새 애니메이션 셋을 올려놓는다.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, m_pCurrentAnimSet);

	// m_pAniCtrl->GetAnimationSetByName()

	// 현재 애니메이션 셋을 전체 재생하는데 걸리는 시간.
	/*pAS->GetPeriod();

	D3DXTRACK_DESC         TrackDesc;
	m_pAniCtrl->GetTrackDesc(트랙, 트랙 정보를 받아올 구조체);*/

	// 중간에 이벤트가 발생하면 보간이 안먹을 수 있기 때문에 이벤트를 끈다고 함.
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	//x파일은 키프레임 기준으로 저장되어 있다. 키프레임 애니메이션은 두 개 이상의 값을 이용하여 애니메이션을 보간할 수 있다.
	// 기존 트랙을 비활성화시킨다.

	if (m_callbackCheckPair)
	{
		if (true == m_callbackCheckPair(m_iCurrentTrack, m_iNewTrack))
		{
			m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fTimeAcc + 0.25f);
			m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR); // 키 프레임 속도. 기본은 1, 0이면 멈춤. duration => 보간을 하는 시간 간격
			m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR); // 트랙 가중치

																										 // 새로운 트랙을 활성화 시킨다.
			m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
			m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
			//현재 트랙의 가중치 // 2번쨰 인자는 1을 기준으로한 가중치를 의미한다. --> 뉴트랙과 합쳐서 1이 나와야됨
			m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
		}
		else
		{
			m_fTimeAcc = 0;
			m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fTimeAcc + 0.25f);
			m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR); // 키 프레임 속도. 기본은 1, 0이면 멈춤. duration => 보간을 하는 시간 간격
			m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR); // 트랙 가중치

																										 // 새로운 트랙을 활성화 시킨다.
			m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
			m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
			//현재 트랙의 가중치 // 2번쨰 인자는 1을 기준으로한 가중치를 의미한다. --> 뉴트랙과 합쳐서 1이 나와야됨
			m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
		}
	}



	m_fTimeAcc = 0.f;

	//이 인스턴스의 로컬 타임을 리셋
	m_pAniCtrl->ResetTime();

	//새 트랙의 포지션을 초기값으로 되돌린다. 되돌리지 않으면 원본 시간에 누적되서 시간이 계산되므로 초기화 해주어야 한다. (애니메이션이 바꼈기 때문에)
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iCurrentTrack = m_iNewTrack;

	return NOERROR;
}

_bool CAnimationCtrl::Is_Finish()
{
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &m_TrackDesc);
	if (m_TrackDesc.Enable == FALSE)
		return false;
	double period = m_pCurrentAnimSet->GetPeriod();
	if (period <= m_TrackDesc.Position + 0.3f)
	{
		//m_isChangeMatrix = true;
		return true;
	}

	return false;
}

// 매 프레임 처리됨
void CAnimationCtrl::Play_AnimationSet(const _float & fTimeDelta)
{
	double period = m_pCurrentAnimSet->GetPeriod();
	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &m_TrackDesc);
	double fCurrentAnimFactor = m_TrackDesc.Position / period;
	double fNextAnimFactor = (m_TrackDesc.Position + fTimeDelta) / period;

	if (_int(fNextAnimFactor) > _int(fCurrentAnimFactor))
	{
		if (m_callbackFunc)
			m_callbackFunc();
	}


	m_fTimeAcc += fTimeDelta;	
	m_pAniCtrl->AdvanceTime(fTimeDelta, nullptr);

}

void CAnimationCtrl::Set_TrackPosition(DOUBLE dlPosition)
{
	m_pAniCtrl->SetTrackPosition(m_iCurrentTrack,dlPosition);
}

CAnimationCtrl * CAnimationCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAnimationCtrl*		pInstance = new CAnimationCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
	{
		_MSG_BOX("CAnimationCtrl Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimationCtrl * CAnimationCtrl::Create(const CAnimationCtrl & rhs)
{
	return new CAnimationCtrl(rhs);
}

void CAnimationCtrl::Free()
{
	Safe_Release(m_pAniCtrl);
}
