#include "..\Headers\Renderer.h"

#include "GameObject.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}


HRESULT CRenderer::Add_Render_Group(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (RENDER_END <= eGroup)
		return E_FAIL;

	if (nullptr == pGameObject)
		return E_FAIL;

	m_RenderList[eGroup].push_back(pGameObject);

	pGameObject->AddRef();

	return NOERROR;
}

HRESULT CRenderer::Ready_Renderer()
{
	return NOERROR;
}

void CRenderer::Render_Renderer()
{
	Render_Priority();
	Render_NoneAlpha();
	Render_Alpha();
	Render_UI();
}

void CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_PRIORITY].clear();
}

void CRenderer::Render_NoneAlpha()
{
	for (auto& pGameObject : m_RenderList[RENDER_NONEALPHA])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_NONEALPHA].clear();
}

_bool Sort(CGameObject * pSour, CGameObject * pDest)
{
	return pSour->Get_ViewDepth() > pDest->Get_ViewDepth();
}

void CRenderer::Render_Alpha()
{
	m_RenderList[RENDER_ALPHA].sort(Sort);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_ALPHA].clear();
}

void CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_UI].clear();
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer*		pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		_MSG_BOX("CRenderer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone_Component()
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
		{
			Safe_Release(pGameObject);
		}
		m_RenderList[i].clear();
	}
	

	CComponent::Free();
}
