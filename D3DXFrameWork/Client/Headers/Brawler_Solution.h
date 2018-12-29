#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Dynamic;
class CTexture;
class CShader;
class CCollider;
_END

_BEGIN(Client)

class CBrawler03;
class CBrawler_Solution final : public CGameObject
{
private:
	explicit CBrawler_Solution(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBrawler_Solution(const CBrawler_Solution& rhs);
	virtual ~CBrawler_Solution() = default;
public:
	HRESULT SetUp_ParentPointer(CBrawler03* pBrawler);
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	//CCollider*			m_pColliderCom_Sphere[5] = { nullptr , nullptr, nullptr, nullptr };
private:
	CBrawler03*			m_pBrawler = nullptr;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CBrawler_Solution* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END