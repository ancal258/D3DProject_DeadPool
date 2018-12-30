#pragma once

#include "Component.h"

_BEGIN(Engine)
class CShader;
class CGameObject;
class CTarget_Manager;
class CTarget;
class _ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP {RENDER_PRIORITY, RENDER_SHADOW ,RENDER_NONEALPHA, RENDER_BACKUI,RENDER_PLAYER, RENDER_ALPHA, RENDER_UI, RENDER_END};
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);	
	virtual ~CRenderer() = default;
public:
	HRESULT Add_Render_Group(RENDERGROUP eGroup, CGameObject* pGameObject);
public:
	HRESULT Ready_Renderer();
	void Render_Renderer();
	void Set_Trigger(_bool isTrigger) {
		m_isTrigger = isTrigger;	}
	_bool Get_Trigger() {
		return m_isTrigger;	}

	void Set_SecondTrigger(_bool isTrigger) {
		m_isSecondTrigger = isTrigger;	}
	_bool Get_SecondTrigger() {
		return m_isSecondTrigger;	}
private:
	list<CGameObject*>				m_RenderList[RENDER_END];
	typedef list<CGameObject*>		RENDERLIST;
private:
	CTarget_Manager*				m_pTarget_Manager = nullptr;
	CShader*						m_pShader_LightAcc = nullptr;
	CShader*						m_pShader_Blend = nullptr;
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;

	CTarget*						m_pBloom_Target = nullptr;
	CTarget*						m_pColor_Target = nullptr;
private:
	_bool							m_isTrigger = false;
	_bool							m_isSecondTrigger = false;
private:
	void Render_Priority();
	void Render_Shadow();
	void Render_NoneAlpha();
	void Render_BackUI();
	void Render_Player();
	void Render_Alpha();
	void Render_UI();
private:
	void Render_Deferred();
	void Render_LightAcc();
	void Render_Blend();
public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
public:
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();

};

_END