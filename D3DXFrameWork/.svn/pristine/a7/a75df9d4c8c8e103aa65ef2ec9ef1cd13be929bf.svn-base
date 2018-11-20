#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CTexture;
class CShader;
class CCollider;
_END

_BEGIN(Client)
class CTest final : public CGameObject
{
private:
	explicit CTest(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTest(const CTest& rhs);
	virtual ~CTest() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;	
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;	
	CShader*			m_pShaderCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	// col
	_bool				m_isCol = false;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CTest* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END