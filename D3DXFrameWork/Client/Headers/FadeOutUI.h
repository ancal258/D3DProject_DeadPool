#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CBuffer_TextUI;
class CTexture;
class CShader;
_END

_BEGIN(Client)
class CPlayer;
class CFadeOutUI final : public CGameObject
{
private:
	explicit CFadeOutUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFadeOutUI(const CFadeOutUI& rhs);
	virtual ~CFadeOutUI() = default;
public:
	void Set_Info(_vec2 vPos, _vec2 vScale);
	void Set_Info(_vec2 vPos, _vec2 vScale, _float fDegree);
	void Set_EndStage() {
		m_isEndStage = true;}
	_bool Get_EndFadeOut();
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	void Render_StaticText();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CBuffer_TextUI*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
private:
	_float				m_fX = 0, m_fY = 0;
	_float				m_fSizeX = 0, m_fSizeY = 0;
	_tchar				m_szTalk[MAX_PATH] = { 0 };
	_uint				m_iStrlen = 0;
	_ulong				m_dwTextureIdx = 0;

	_float				m_fAlpha = 0.f;
	_bool				m_isEndStage = false;
private:
	CPlayer*			m_pPlayer = nullptr;
private:
	_float				m_fTimeAcc = 0.f;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	_float				m_fFrame = 0.f;
	_bool				m_isLoad = false;
public:
	static CFadeOutUI* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END