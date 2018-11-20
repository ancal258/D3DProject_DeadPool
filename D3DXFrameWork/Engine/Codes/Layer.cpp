#include "..\Headers\Layer.h"

CLayer::CLayer()
{
}

const CGameObject * CLayer::Get_ObjectPointer(const _uint & iIndex)
{
	if (iIndex >= m_ObjectList.size())
		return nullptr;

	auto		iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if(iter == m_ObjectList.end())
		return nullptr;

	return *iter;
}

HRESULT CLayer::Ready_Layer()
{
	return NOERROR;
}

HRESULT CLayer::Add_ObjectToLayer(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return NOERROR;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int iExitCode = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
		{		
			iExitCode = pGameObject->Update_GameObject(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}
	return _int(iExitCode);
}

_int CLayer::LastUpdate_Layer(const _float & fTimeDelta)
{
	_int iExitCode = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
		{
			iExitCode = pGameObject->LastUpdate_GameObject(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}
	auto iter_begin = m_ObjectList.begin();
	auto iter_end = m_ObjectList.end();
	for (auto iter = iter_begin; iter != iter_end;)
	{
		if (false == (*iter)->Get_Lived())
		{
			Safe_Release(*iter);
			iter = m_ObjectList.erase(iter);
		}
		else
			++iter;
	}

	return _int(iExitCode);
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer();

	if (FAILED(pInstance->Ready_Layer()))
	{
		_MSG_BOX("CLayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList )
	{
		Safe_Release(pGameObject);
	}
	m_ObjectList.clear();
}
