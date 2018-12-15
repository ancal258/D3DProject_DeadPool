#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CBuffer_RcTex;
class CTexture;
class CShader;
_END

_BEGIN(Client)

class CTalkBox final : public CGameObject
{
private:
	explicit CTalkBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTalkBox(const CTalkBox& rhs);
	virtual ~CTalkBox() = default;
public:
	void Set_Info(_vec2 vPos,_tchar* pTalkString);
public:
	virtual HRESULT Ready_GameObject_Prototype(_uint iKind);
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CBuffer_RcTex*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
private:
	_float				m_fX = 0, m_fY = 0;
	_float				m_fSizeX = 0, m_fSizeY = 0;
	_tchar				m_szTalk[MAX_PATH] = { 0 };
	_uint				m_iStrlen = 0;
	_ulong				m_dwTextureIdx = 0;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	_float				m_fFrame = 0.f;

public:
	static CTalkBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iKind);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END