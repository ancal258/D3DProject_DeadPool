#include "..\Headers\Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pGraphic_Device->AddRef();
	m_pObject_Manager->AddRef();
	m_pInput_Device->AddRef();
}

HRESULT CScene::Add_Object_Prototype(const _uint & iSceneID, const _tchar * pProtoTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Object_Prototype(iSceneID, pProtoTag, pPrototype);
}

HRESULT CScene::Add_Object(const _uint & iPSceneID, const _tchar * pProtoTag, const _uint & iSceneID, const _tchar * pLayerTag, CGameObject** ppGameObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Object(iPSceneID, pProtoTag, iSceneID, pLayerTag, ppGameObject);
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Update_Object_Manager(fTimeDelta);
}

_int CScene::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->LastUpdate_Object_Manager(fTimeDelta);
}


void CScene::Free()
{
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pGraphic_Device);
}
