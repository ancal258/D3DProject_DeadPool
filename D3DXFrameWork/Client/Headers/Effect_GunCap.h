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
class CDP_Gun;
class CEffect_GunCap final : public CGameObject
{
private:
	explicit CEffect_GunCap(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_GunCap(const CEffect_GunCap& rhs);
	virtual ~CEffect_GunCap() = default;
public:

	void Set_ParentMatrix(const _matrix* pMatrix) {
		m_pParentMatrix = pMatrix;	}
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

	const _matrix*		m_pParentMatrix = nullptr;

	_float				m_fTimeAcc = 0;

private:
	virtual HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CEffect_GunCap* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END