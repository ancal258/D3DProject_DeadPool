
#include "..\Headers\Frame_Manager.h"
#include "Frame.h"

_IMPLEMENT_SINGLETON(CFrame_Manager)

CFrame_Manager::CFrame_Manager()
{
}


HRESULT CFrame_Manager::Add_Frame(const _tchar * pFrameTag, const _uint & iCallCntPerSec)
{
	CFrame* pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(iCallCntPerSec);
	if (nullptr == pFrame)
		return E_FAIL;

	m_mapFrames.insert(MAPFRAMES::value_type(pFrameTag, pFrame));

	return NOERROR;
}

_bool CFrame_Manager::Permit_Call(const _tchar * pFrameTag, const _float & fTimeDelta)
{
	CFrame* pFrame = Find_Frame(pFrameTag);

	if (nullptr == pFrame)
		return false;

	return pFrame->Permit_Call(fTimeDelta);	
}

CFrame * CFrame_Manager::Find_Frame(const _tchar * pFrameTag)
{
	auto iter = find_if(m_mapFrames.begin(), m_mapFrames.end(), CFinder_Tag(pFrameTag));

	if (iter == m_mapFrames.end())
		return nullptr;

	return iter->second;
}

void CFrame_Manager::Free()
{
	for (auto& Pair : m_mapFrames)
	{
		Safe_Release(Pair.second);
	}
	m_mapFrames.clear();
}
