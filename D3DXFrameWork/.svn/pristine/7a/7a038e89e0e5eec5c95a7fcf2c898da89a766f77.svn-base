#include "..\Headers\GameObject.h"
#include "Component.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
	, m_isLived(rhs.m_isLived)
{
	m_pGraphic_Device->AddRef();
}

CComponent * CGameObject::Get_ComponentPointer(const _tchar * pComponentTag) const
{
	if (nullptr == pComponentTag)
		return nullptr;

	CComponent* pComponent = Find_Component(pComponentTag);
	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

HRESULT CGameObject::Add_Component(const _tchar * pComponentTag, CComponent * pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	CComponent* pComponent_Find = Find_Component(pComponentTag);
	if (nullptr != pComponent_Find)
		return E_FAIL;

	m_mapComponent.insert(MAPCOMPONENT::value_type(pComponentTag, pComponent));

	pComponent->AddRef();

	return NOERROR;
}

HRESULT CGameObject::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CGameObject::Ready_GameObject()
{
	return NOERROR;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CGameObject::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CGameObject::Render_GameObject()
{
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag) const
{
	auto	iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CFinder_Tag(pComponentTag));

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_mapComponent)
		Safe_Release(Pair.second);

	m_mapComponent.clear();

	Safe_Release(m_pGraphic_Device);
}
