#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CShader;
_END
_BEGIN(Client)
class CMinigun;
class CEffect_MinigunCap final : public CGameObject
{
private:
	explicit CEffect_MinigunCap(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_MinigunCap(const CEffect_MinigunCap& rhs);
	virtual ~CEffect_MinigunCap() = default;
public:
	const _matrix& Get_HandMatrix(_uint iSide) const {
		return m_CombinedHandMatrix[iSide];
	}
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private: // For. Component
	CTransform*         m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CMesh_Static*	    m_pMeshCom = nullptr;
	CMinigun*			m_pMinigun = nullptr;
private:
	_matrix             m_CombinedHandMatrix[2];
	const _matrix*      m_pHandMatrix[2] = { nullptr };
	_matrix            m_CombinedRootMatrix;
	const _matrix*      m_pRootMatrix = nullptr;
	_matrix				m_RealMatrix;
private:
	virtual HRESULT Ready_Component();
	HRESULT Update_HandMatrix();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT SetUp_PlayerPointer();
public:
	static CEffect_MinigunCap* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END