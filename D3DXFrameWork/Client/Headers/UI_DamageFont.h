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

class CUI_DamageFont final : public CGameObject
{
private:
	explicit CUI_DamageFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_DamageFont(const CUI_DamageFont& rhs);
	virtual ~CUI_DamageFont() = default;
public:
	void Set_Position(CGameObject* pParentObject, _uint iType);
public:
	virtual HRESULT Ready_GameObject_Prototype(_uint iIndex);
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
	_float				m_fTimeAcc = 0.f;
	_uint				m_iIndex = 0;
	_vec3				m_vPosition;
	const _matrix*		m_pParentMatrix;
public:
	static CUI_DamageFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device,_uint iIndex);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END