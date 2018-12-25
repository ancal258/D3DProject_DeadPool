#pragma once

//#include "Engine_Defines.h"
#include "Defines.h"
#include "GameObject.h"
_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CShader;
class CCollider;
_END

class CItem abstract : public CGameObject
{
protected:
	enum STATE_EVENT
	{
		EVENT_CHAIR, EVENT_TV, EVENT_END
	};
protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem(const CItem& rhs);
	virtual ~CItem() = default;
public:
	_bool Get_ClickedObject() {
		return m_isCol;
	}

public:
	void Set_StateInfo(_vec3* pRight, _vec3* pUp, _vec3* vLook, _vec3* vPos);
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
protected:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

protected:
	BOOL				m_isCol = false;
private:
	HRESULT Ready_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

protected:
	virtual void Free();
};

