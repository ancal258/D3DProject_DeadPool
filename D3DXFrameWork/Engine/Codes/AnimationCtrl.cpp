#include "..\Headers\AnimationCtrl.h"



CAnimationCtrl::CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	:m_pAniCtrl(pAniCtrl)
{
	m_pAniCtrl->AddRef();
}

CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl & rhs)
	:m_pAniCtrl(nullptr)
{
	//( ��µ� ����?, X���� ���� ���ǵǾ� �ִ� �ִϸ��̼� ���� , 
	if (FAILED(rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(), rhs.m_pAniCtrl->GetMaxNumAnimationSets(), rhs.m_pAniCtrl->GetMaxNumTracks(),
		rhs.m_pAniCtrl->GetMaxNumEvents(), &m_pAniCtrl)))
		return;
}

HRESULT CAnimationCtrl::Ready_AnimationCtrl()
{
	return NOERROR;
}


// �ִϸ��̼��� �ٲ� �� ���� ó����.
HRESULT CAnimationCtrl::Set_AnimationSet(const _uint & iIndex)
{
	if (m_iOldAniIdx == iIndex)
	{
		m_isChangeMatrix = false;
		return NOERROR;
	}
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0; // 0�̸� 1��, �ƴϸ� 0����

	LPD3DXANIMATIONSET pAniSet = nullptr;

	// �ִϸ��̼� ��Ʈ�ѷ��� ����� �ִϸ��̼� ���� ���´�.
	if (FAILED(m_pAniCtrl->GetAnimationSet(iIndex, &pAniSet)))
		return E_FAIL;
	
	//AniSet�� ����Ǿ ����.
	//���ο� Ʈ���� ������ �ִϸ��̼� ���� �÷����´�.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack,pAniSet);
	//m_pAniCtrl->GetAnimationSetByName(); // �̸����� ��������
	

	// Ʈ���� ��ġ�� �����´�.  Ʈ���� ����Ǵ� �ð��� ������ ����.
	// ���� �ִϸ��̼� ���� ��ü ����ϴµ� �ɸ��� �ð�. 
	m_dlPeriod = pAniSet->GetPeriod();
	ZeroMemory(&m_TrackDesc, sizeof(D3DXTRACK_DESC));
	

	Safe_Release(pAniSet);

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	//���� Ʈ���� ��Ȱ��ȭ ��Ų��.
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack,FALSE,m_fTimeAcc + 0.25f);
	//���� Ʈ���� ���ǵ� // 1�� �ָ� �⺻ ����ӵ��̴�. // ������ ���� : ��� �����Ұų�
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack,1.f,m_fTimeAcc,0.25f,D3DXTRANSITION_LINEAR);
	//���� Ʈ���� ����ġ // 2���� ���ڴ� 1�� ���������� ����ġ�� �ǹ��Ѵ�. --> ��Ʈ���� ���ļ� 1�� ���;ߵ�
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack,0.1f,m_fTimeAcc,0.25f, D3DXTRANSITION_LINEAR);
	
	// ���ο� Ʈ���� Ȱ��ȭ��Ų��.
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack,0.9f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);

	m_fTimeAcc = 0.f;
	m_pAniCtrl->ResetTime();

	m_iOldAniIdx = iIndex;

	// ���� ������ ����!
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

// �� ������ ó����
void CAnimationCtrl::Play_AnimationSet(const _float & fTimeDelta)
{


	if (nullptr == m_pAniCtrl)
		return;

	if (FAILED(m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &m_TrackDesc))) // ������ ����Ǿ���?
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
