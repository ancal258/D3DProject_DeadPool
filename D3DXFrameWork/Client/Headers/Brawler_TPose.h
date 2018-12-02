#pragma once

#include "Brawler.h"

_BEGIN(Engine)
class CBrawler_TPose final : public CBrawler
{
private:
	explicit CBrawler_TPose(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBrawler_TPose(const CBrawler_TPose& rhs);
	virtual ~CBrawler_TPose() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	virtual HRESULT Ready_Component();
public:
	static CBrawler_TPose* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END