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
}

HRESULT CAnimationCtrl::Ready_AnimationCtrl()
{
	return NOERROR;
}


// 애니메이션이 바뀔 때 마다 처리됨.
HRESULT CAnimationCtrl::Set_AnimationSet(const _uint & iIndex)
{
	if (m_iOldAniIdx == iIndex)
	{
		m_isChangeMatrix = false;
		return NOERROR;
	}
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0; // 0이면 1로, 아니면 0으로

	LPD3DXANIMATIONSET pAniSet = nullptr;

	// 애니메이션 컨트롤러에 저장된 애니메이션 셋을 얻어온다.
	if (FAILED(m_pAniCtrl->GetAnimationSet(iIndex, &pAniSet)))
		return E_FAIL;
	
	//AniSet이 복사되어서 들어간다.
	//새로운 트랙에 가져온 애니메이션 셋을 올려놓는다.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack,pAniSet);
	//m_pAniCtrl->GetAnimationSetByName(); // 이름으로 가져오기
	

	// 트랙의 위치를 가져온다.  트랙이 재생되는 시간과 단위가 같다.
	// 현재 애니메이션 셋을 전체 재생하는데 걸리는 시간. 
	m_dlPeriod = pAniSet->GetPeriod();
	ZeroMemory(&m_TrackDesc, sizeof(D3DXTRACK_DESC));
	

	Safe_Release(pAniSet);

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	//기존 트랙을 비활성화 시킨다.
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack,FALSE,m_fTimeAcc + 0.25f);
	//현재 트랙의 스피드 // 1로 주면 기본 재생속도이다. // 마지막 인자 : 어떻게 보간할거냐
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack,1.f,m_fTimeAcc,0.25f,D3DXTRANSITION_LINEAR);
	//현재 트랙의 가중치 // 2번쨰 인자는 1을 기준으로한 가중치를 의미한다. --> 뉴트랙과 합쳐서 1이 나와야됨
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack,0.1f,m_fTimeAcc,0.25f, D3DXTRANSITION_LINEAR);
	
	// 새로운 트랙을 활성화시킨다.
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack,0.9f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);

	m_fTimeAcc = 0.f;
	m_pAniCtrl->ResetTime();

	m_iOldAniIdx = iIndex;

	// 가장 앞으로 돌려!
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iCurrentTrack = m_iNewTrack;
	
	//m_isChangeMatrix = true;

	return NOERROR;
}

_bool CAnimationCtrl::Is_Finish()
{
	if (m_TrackDesc.Enable == FALSE)
		return false;

	if (m_dlPeriod <= m_TrackDesc.Position + 0.3f)
	{
		//m_isChangeMatrix = true;
		return true;
	}

	return false;
}

// 매 프레임 처리됨
void CAnimationCtrl::Play_AnimationSet(const _float & fTimeDelta)
{


	if (nullptr == m_pAniCtrl)
		return;

	if (FAILED(m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &m_TrackDesc))) // 어디까지 재생되었니?
		return;

	double fTmpBefore = m_TrackDesc.Position / m_dlPeriod;
	double fTmpAfter = (m_TrackDesc.Position + fTimeDelta) / m_dlPeriod;

	_int iComputeBA = (_int)fTmpAfter - (_int)fTmpBefore;
	if (iComputeBA >= 1)
		m_isChangeMatrix = true;
	else
		m_isChangeMatrix = false;

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
