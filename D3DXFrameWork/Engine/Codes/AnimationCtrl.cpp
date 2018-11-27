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


// �ִϸ��̼��� �ٲ� �� ���� ó����.
HRESULT CAnimationCtrl::Set_AnimationSet(const _uint & iIndex)
{
	if (m_pCurrentAnimSet == m_vecAnimSet[iIndex])
		return NOERROR;
	// �ִϸ��̼� ��Ʈ�ѷ��� ����� �ִϸ��̼� ���� ���´�.
	m_pCurrentAnimSet = m_vecAnimSet[iIndex];

	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;


	// ���ο� Ʈ���� �� �ִϸ��̼� ���� �÷����´�.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, m_pCurrentAnimSet);

	// m_pAniCtrl->GetAnimationSetByName()

	// ���� �ִϸ��̼� ���� ��ü ����ϴµ� �ɸ��� �ð�.
	/*pAS->GetPeriod();

	D3DXTRACK_DESC         TrackDesc;
	m_pAniCtrl->GetTrackDesc(Ʈ��, Ʈ�� ������ �޾ƿ� ����ü);*/

	// �߰��� �̺�Ʈ�� �߻��ϸ� ������ �ȸ��� �� �ֱ� ������ �̺�Ʈ�� ���ٰ� ��.
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	//x������ Ű������ �������� ����Ǿ� �ִ�. Ű������ �ִϸ��̼��� �� �� �̻��� ���� �̿��Ͽ� �ִϸ��̼��� ������ �� �ִ�.
	// ���� Ʈ���� ��Ȱ��ȭ��Ų��.

	if (m_callbackCheckPair)
	{
		if (true == m_callbackCheckPair(m_iCurrentTrack, m_iNewTrack))
		{
			m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fTimeAcc + 0.25f);
			m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR); // Ű ������ �ӵ�. �⺻�� 1, 0�̸� ����. duration => ������ �ϴ� �ð� ����
			m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR); // Ʈ�� ����ġ

																										 // ���ο� Ʈ���� Ȱ��ȭ ��Ų��.
			m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
			m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
			//���� Ʈ���� ����ġ // 2���� ���ڴ� 1�� ���������� ����ġ�� �ǹ��Ѵ�. --> ��Ʈ���� ���ļ� 1�� ���;ߵ�
			m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
		}
		else
		{
			m_fTimeAcc = 0;
			m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fTimeAcc + 0.25f);
			m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR); // Ű ������ �ӵ�. �⺻�� 1, 0�̸� ����. duration => ������ �ϴ� �ð� ����
			m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR); // Ʈ�� ����ġ

																										 // ���ο� Ʈ���� Ȱ��ȭ ��Ų��.
			m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
			m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
			//���� Ʈ���� ����ġ // 2���� ���ڴ� 1�� ���������� ����ġ�� �ǹ��Ѵ�. --> ��Ʈ���� ���ļ� 1�� ���;ߵ�
			m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fTimeAcc, 0.25f, D3DXTRANSITION_LINEAR);
		}
	}



	m_fTimeAcc = 0.f;

	//�� �ν��Ͻ��� ���� Ÿ���� ����
	m_pAniCtrl->ResetTime();

	//�� Ʈ���� �������� �ʱⰪ���� �ǵ�����. �ǵ����� ������ ���� �ð��� �����Ǽ� �ð��� ���ǹǷ� �ʱ�ȭ ���־�� �Ѵ�. (�ִϸ��̼��� �ٲ��� ������)
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

// �� ������ ó����
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
