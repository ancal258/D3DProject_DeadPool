#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Dynamic;
class CShader;
class CNavigation;
_END

_BEGIN(Client)
class CMinigun final : public CGameObject
{
private:
	explicit CMinigun(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMinigun(const CMinigun& rhs);
	virtual ~CMinigun() = default;
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
	CNavigation*		m_pNavigationCom = nullptr;
	CMesh_Dynamic*      m_pMeshCom = nullptr;

	_matrix             m_CombinedHandMatrix[2];
	const _matrix*      m_pHandMatrix[2] = { nullptr };
	_matrix            m_CombinedRootMatrix;
	const _matrix*      m_pRootMatrix = nullptr;
	_matrix				m_RealMatrix;
private:
	const CGameObject*	m_pAirplane = nullptr;
private:
	virtual HRESULT Ready_Component();
	HRESULT Update_HandMatrix();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CMinigun* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END