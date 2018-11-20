#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CBuffer_Terrain;
class CTexture;
class CShader;
class CBrush;
class CPicking;
_END

_BEGIN(Client)

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;	
	CRenderer*			m_pRendererCom = nullptr;
	CBuffer_Terrain*	m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTexture*			m_pFilterCom = nullptr;
	CTexture*			m_pBrushCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CPicking*			m_pPickingCom = nullptr;
private:
	LPDIRECT3DTEXTURE9	m_pFilterTexture = nullptr;
	D3DMATERIAL9		m_MtrlInfo;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END