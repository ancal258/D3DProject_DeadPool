#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CShader;
_END

class CEffect_GunCapT final : public CGameObject
{
private:
	explicit CEffect_GunCapT(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_GunCapT(const CEffect_GunCapT& rhs);
	virtual ~CEffect_GunCapT() = default;
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
private:
	_float				m_fU = 0.f;
	_float				m_fV = 0.f;

public:
	_float				m_fSpeed_U = 1.f;
	_float				m_fSpeed_V = 0.f;
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
public:
	static CEffect_GunCapT* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};
