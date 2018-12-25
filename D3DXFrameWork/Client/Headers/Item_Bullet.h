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

class CItem_Bullet final : public CItem
{
private:
	explicit CItem_Bullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CItem_Bullet(const CItem_Bullet& rhs);
	virtual ~CItem_Bullet() = default;
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
	static CItem_Bullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

