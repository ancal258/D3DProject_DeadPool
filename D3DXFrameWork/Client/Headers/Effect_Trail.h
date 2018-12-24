#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CBuffer_Trail;
class CTexture;
class CShader;
_END

_BEGIN(Client)
class CDP_Sword;
class CEffect_Trail final : public CGameObject
{
private:
	explicit CEffect_Trail(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_Trail(const CEffect_Trail& rhs);
	virtual ~CEffect_Trail() = default;
public:
	void Set_Position(_vec3 vPos);
	void SetUp_Sword(CDP_Sword* pGameObject) {
		m_pSword = pGameObject;		}
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CBuffer_Trail*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

	CDP_Sword*		m_pSword = nullptr;
	const _matrix*		m_pParentMatrix;
	_float			m_fTimeAcc = 0.f;


private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CEffect_Trail* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END