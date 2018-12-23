#include "stdafx.h"
#include "..\Headers\Brawler_Manager.h"

_IMPLEMENT_SINGLETON(CBrawler_Manager)

CBrawler_Manager::CBrawler_Manager()
{
}

HRESULT CBrawler_Manager::Add_Object(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_BrawlerList.push_back(pGameObject);

	return NOERROR;
}

HRESULT CBrawler_Manager::Clear_Object(CGameObject * pGameObject)
{
	for (auto iter = m_BrawlerList.begin(); iter != m_BrawlerList.end();)
	{
		if (*iter == pGameObject)
		{
			iter = m_BrawlerList.erase(iter);
			continue;
		}
		++iter;
	}
	return NOERROR;
}

_bool CBrawler_Manager::Collision_Chech(CBrawler * pBrawler, _float *pLength)
{
	//for (auto& pGameObject : m_BrawlerList)
	//{
	//	if (pGameObject == pBrawler)
	//		continue;

	//	if(pGameObject)
	//}

	return false;
}

void CBrawler_Manager::Free()
{
	//for (auto& pGameObject : m_BrawlerList)
	//	Safe_Release(pGameObject);
}
