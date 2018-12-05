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

class CUI_Test final : public CGameObject
{
private:
	explicit CUI_Test(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Test(const CUI_Test& rhs);
	virtual ~CUI_Test() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
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
	_float				m_fX, m_fY;
	_float				m_fSizeX, m_fSizeY;

	_ulong				m_dwTextureIdx = 0;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	_float				m_fFrame = 0.f;

public:
	static CUI_Test* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END