#include "..\Headers\Object_Manager.h"

_IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

const CGameObject * CObject_Manager::Get_ObjectPointer(const _uint& iSceneID, const _tchar * pLayerTag, const _uint & iIndex)
{
	CLayer* pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_ObjectPointer(iIndex);	
}

HRESULT CObject_Manager::Reserve_Object_Manager(const _uint & iMaxScene)
{
	if (nullptr != m_pPrototype || 
		nullptr != m_pLayer)
		return E_FAIL;

	m_pPrototype = new MAPPROTOTYPE[iMaxScene];
	m_pLayer = new MAPLAYER[iMaxScene];

	m_iMaxScene = iMaxScene;

	return NOERROR;
}

HRESULT CObject_Manager::Add_Object_Prototype(const _uint & iSceneID, const _tchar * pProtoTag, CGameObject * pPrototype)
{
	if (m_iMaxScene <= iSceneID)
		return E_FAIL;

	if (nullptr == m_pPrototype)
		return E_FAIL;

	CGameObject* pObject_Find = Find_Prototype(iSceneID, pProtoTag);
	if (nullptr != pObject_Find)
		return E_FAIL;

	m_pPrototype[iSceneID].insert(MAPPROTOTYPE::value_type(pProtoTag, pPrototype));

	return NOERROR;
}

HRESULT CObject_Manager::Add_Object(const _uint & iPSceneID, const _tchar * pProtoTag, const _uint & iSceneID, const _tchar * pLayerTag, CGameObject** ppGameObject)
{
	if (m_iMaxScene <= iPSceneID ||
		m_iMaxScene <= iSceneID)
		return E_FAIL;

	if (nullptr == m_pPrototype || 
		nullptr == m_pLayer)
		return E_FAIL;

	CGameObject* pProto = Find_Prototype(iPSceneID, pProtoTag);
	if (nullptr == pProto)
		return E_FAIL;

	CGameObject* pGameObject = pProto->Clone_GameObject();
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			goto except;

		if (FAILED(pLayer->Add_ObjectToLayer(pGameObject)))
		{
			Safe_Release(pLayer);
			goto except;		
		}

		m_pLayer[iSceneID].insert(MAPLAYER::value_type(pLayerTag, pLayer));
	}
	else
	{
		if (FAILED(pLayer->Add_ObjectToLayer(pGameObject)))
			goto except;
	}

	if (nullptr != ppGameObject)
		*ppGameObject = pGameObject;

	return NOERROR;

except:	
	Safe_Release(pGameObject);
	return E_FAIL;
}

HRESULT CObject_Manager::Clear_Object(const _uint & iSceneID)
{
	if (nullptr == m_pLayer)
		return E_FAIL;

	if (nullptr == m_pPrototype)
		return E_FAIL;

	if (m_iMaxScene <= iSceneID)
		return E_FAIL;

	for (auto& Pair : m_pPrototype[iSceneID])
		Safe_Release(Pair.second);
	m_pPrototype[iSceneID].clear();


	for (auto& Pair : m_pLayer[iSceneID])
		Safe_Release(Pair.second);
	m_pLayer[iSceneID].clear();


	return NOERROR;
}

_int CObject_Manager::Update_Object_Manager(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	for (size_t i = 0; i < m_iMaxScene; i++)
	{
		for (auto& Pair : m_pLayer[i])
		{
			iExitCode = Pair.second->Update_Layer(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return _int(iExitCode);
}

_int CObject_Manager::LastUpdate_Object_Manager(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	for (size_t i = 0; i < m_iMaxScene; i++)
	{
		for (auto& Pair : m_pLayer[i])
		{
			iExitCode = Pair.second->LastUpdate_Layer(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}

	return _int(iExitCode);
}

CGameObject * CObject_Manager::Find_Prototype(const _uint & iPSceneID, const _tchar * pProtoTag)
{
	auto iter = find_if(m_pPrototype[iPSceneID].begin(), m_pPrototype[iPSceneID].end(), CFinder_Tag(pProtoTag));

	if(iter == m_pPrototype[iPSceneID].end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(const _uint & iSceneID, const _tchar * pLayerTag)
{
	auto iter = find_if(m_pLayer[iSceneID].begin(), m_pLayer[iSceneID].end(), CFinder_Tag(pLayerTag));

	if (iter == m_pLayer[iSceneID].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < m_iMaxScene; i++)
	{
		for (auto& Pair : m_pLayer[i])		
			Safe_Release(Pair.second);
		m_pLayer[i].clear();
	}
	Safe_Delete_Array(m_pLayer);

	for (size_t i = 0; i < m_iMaxScene; i++)
	{
		for (auto& Pair : m_pPrototype[i])
			Safe_Release(Pair.second);
		m_pPrototype[i].clear();
	}
	Safe_Delete_Array(m_pPrototype);
}
