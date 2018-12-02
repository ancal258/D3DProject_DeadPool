#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CTexture;
class CShader;
_END

_BEGIN(Client)

class CPlayer;
class CDP_Gun final : public CGameObject
{
private:
	explicit CDP_Gun(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CDP_Gun(const CDP_Gun& rhs);
	virtual ~CDP_Gun() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype(_uint iSide);
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
private:
	CPlayer*			m_pPlayer = nullptr;
	_uint				m_iSide = 0; // 0 == ÁÂÃø , 1 == ¿ìÃø
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT SetUp_PlayerPointer();

public:
	static CDP_Gun* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iSide);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END