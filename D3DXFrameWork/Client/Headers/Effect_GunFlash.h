#pragma once

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CShader;
_END

class CEffect_GunFlash final : public CGameObject
{
private:
	explicit CEffect_GunFlash(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_GunFlash(const CEffect_GunFlash& rhs);
	virtual ~CEffect_GunFlash() = default;
public:
	void Set_ParentMatrix(const _matrix* pMatrix) {
		m_pParentMatrix = pMatrix;
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
	_float				m_fTimeAcc = 0.f;
	_uint				m_iType = 0.f;
private:
	const _matrix*		m_pParentMatrix = nullptr;

private:
	virtual HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CEffect_GunFlash* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};
