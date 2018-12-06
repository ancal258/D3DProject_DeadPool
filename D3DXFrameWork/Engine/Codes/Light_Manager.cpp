#include "..\Headers\Light_Manager.h"
#include "Light.h"

_IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{

}

const D3DLIGHT9 * CLight_Manager::Get_LightInfo(const _uint& iIndex) const
{
	if (m_LightList.size() <= iIndex)
		return nullptr;

	auto	iter = m_LightList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

		
	return (*iter)->Get_LightInfo();
}

HRESULT CLight_Manager::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 * pLightInfo)
{
	if (nullptr == pLightInfo)
		return E_FAIL;

	CLight*			pLight = CLight::Create(pGraphic_Device, pLightInfo);
	if (nullptr == pLight)
		return E_FAIL;

	m_LightList.push_back(pLight);

	return NOERROR;
}

HRESULT CLight_Manager::Clear_Light()
{
	for (auto& pLight : m_LightList)
		Safe_Release(pLight);
	m_LightList.clear();

	return NOERROR;
}

void CLight_Manager::Render_Light(LPD3DXEFFECT pEffect)
{
	for (auto& pLight : m_LightList)
		pLight->Render_Light(pEffect);
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_LightList)
		Safe_Release(pLight);
	m_LightList.clear();
}
