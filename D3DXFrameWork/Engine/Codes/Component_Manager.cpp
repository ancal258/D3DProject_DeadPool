#include "..\Headers\Component_Manager.h"

_IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()	
	: m_pMapComponents(nullptr)
{

}

HRESULT CComponent_Manager::Reserve_Component_Manager(const _uint & iMaxScene)
{
	if (nullptr != m_pMapComponents)	
		return E_FAIL;

	m_pMapComponents = new MAPCOMPONENT[iMaxScene];

	m_iMaxScene = iMaxScene;

	return NOERROR;
}

HRESULT CComponent_Manager::Add_Component(const _uint & iSceneIdx, const _tchar * pComponentTag, CComponent * pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	if (nullptr == m_pMapComponents)
		return E_FAIL;

	if (m_iMaxScene <= iSceneIdx)
		return E_FAIL;

	CComponent* pComponent_Find = Find_Component(iSceneIdx, pComponentTag);
	if (nullptr != pComponent_Find)
		return E_FAIL;

	m_pMapComponents[iSceneIdx].insert(MAPCOMPONENT::value_type(pComponentTag, pComponent));

	return NOERROR;
}

CComponent * CComponent_Manager::Clone_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	if (nullptr == m_pMapComponents)
		return nullptr;

	if (m_iMaxScene <= iSceneIdx)
		return nullptr;

	// 복제하기위한 원형객체를 찾는다.
	CComponent* pComponent = Find_Component(iSceneIdx, pComponentTag);
	if (nullptr == pComponent)
		return nullptr; 

	// 원형객체를 복제하여 리턴한다
	return pComponent->Clone_Component();
}

HRESULT CComponent_Manager::Clear_Component(const _uint & iSceneIdx)
{
	if (nullptr == m_pMapComponents)
		return E_FAIL;

	if (m_iMaxScene <= iSceneIdx)
		return E_FAIL;

	for (auto& Pair : m_pMapComponents[iSceneIdx])	
		Safe_Release(Pair.second);

	m_pMapComponents[iSceneIdx].clear();

	return NOERROR;
}

CComponent * CComponent_Manager::Find_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	auto		iter = find_if(m_pMapComponents[iSceneIdx].begin(), m_pMapComponents[iSceneIdx].end(), CFinder_Tag(pComponentTag));
	if(iter == m_pMapComponents[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iMaxScene; i++)
	{
		for (auto& Pair : m_pMapComponents[i])		
			Safe_Release(Pair.second);
		m_pMapComponents[i].clear();
	}

	Safe_Delete_Array(m_pMapComponents);
}
