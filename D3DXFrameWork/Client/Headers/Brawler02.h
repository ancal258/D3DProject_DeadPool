#pragma once

#include "Brawler.h"

_BEGIN(Engine)
class CBrawler02 final : public CBrawler
{
private:
	explicit CBrawler02(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBrawler02(const CBrawler02& rhs);
	virtual ~CBrawler02() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	virtual HRESULT Ready_Component();
private:
	virtual _int Update_Stage_Field(const _float& fTimeDelta) { return 1; };
	virtual _int LastUpdate_Stage_Field(const _float& fTimeDelta) { return 1; };

	virtual _int Update_Stage_Airplane(const _float& fTimeDelta) { return 1; };
	virtual _int LastUpdate_Stage_Airplane(const _float& fTimeDelta) { return 1; };
public:
	static CBrawler02* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END