#pragma once

#include "Component.h"

_BEGIN(Engine)

class CGameObject;
class _ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP {RENDER_PRIORITY, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END};
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);	
	virtual ~CRenderer() = default;
public:
	HRESULT Add_Render_Group(RENDERGROUP eGroup, CGameObject* pGameObject);
public:
	HRESULT Ready_Renderer();
	void Render_Renderer();
private:
	list<CGameObject*>				m_RenderList[RENDER_END];
	typedef list<CGameObject*>		RENDERLIST;
private:
	void Render_Priority();
	void Render_NoneAlpha();
	void Render_Alpha();
	void Render_UI();
public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
public:
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();

};

_END