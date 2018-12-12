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

class CToolEffect final : public CGameObject
{
private:
	explicit CToolEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CToolEffect(const CToolEffect& rhs);
	virtual ~CToolEffect() = default;
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
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	_float				m_fFrame = 0.f;

public:
	static CToolEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};