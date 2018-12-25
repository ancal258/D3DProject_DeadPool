#pragma once

//#include "Engine_Defines.h"

#include "Item.h"
_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CShader;
class CCollider;
_END

class CItem_DPPoint final : public CItem
{
private:
	enum STATE_EVENT
	{
		EVENT_CHAIR, EVENT_TV, EVENT_END
	};
private:
	explicit CItem_DPPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem_DPPoint(const CItem_DPPoint& rhs);
	virtual ~CItem_DPPoint() = default;
public:
	void Set_StateInfo(_vec3 * vPos);
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	HRESULT Ready_Component();
public:
	static CItem_DPPoint* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

