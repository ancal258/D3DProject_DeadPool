
#include "..\Headers\Frame.h"


CFrame::CFrame()
{
}

HRESULT CFrame::Ready_Frame(const _uint & iCallCntPerSec)
{
	m_fCallTime = 1.f / iCallCntPerSec;

	return NOERROR;
}

_bool CFrame::Permit_Call(const _float& fTimeDelta)
{
	m_fTimeDelta_Acc += fTimeDelta;

	if (m_fTimeDelta_Acc >= m_fCallTime)
	{
		m_fTimeDelta_Acc = 0.f;

		return true;		
	}

	return false;
}

CFrame * CFrame::Create(const _uint & iCallCntPerSec)
{
	CFrame*		pInstance = new CFrame();

	if (FAILED(pInstance->Ready_Frame(iCallCntPerSec)))
	{
		_MSG_BOX("CFrame Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFrame::Free()
{
}
