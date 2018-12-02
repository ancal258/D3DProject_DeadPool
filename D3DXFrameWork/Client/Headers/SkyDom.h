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
class CSkyDom final : public CGameObject
{
	enum TYPE {
		TYPE_NIGHT, TYPE_DUSK, TYPE_SUNSET, TYPE_END
	};
private:
	explicit CSkyDom(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSkyDom(const CSkyDom& rhs);
	virtual ~CSkyDom() = default;

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

private:
	_uint				m_iType = TYPE_NIGHT;
private:
	HRESULT Ready_Component();
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CSkyDom* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();

};

_END