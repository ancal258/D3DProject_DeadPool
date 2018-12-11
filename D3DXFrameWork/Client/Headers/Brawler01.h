#pragma once

#include "Brawler.h"

_BEGIN(Engine)
class CBrawler01 final : public CBrawler
{
private:
	enum Brawler01 {STATE_IDLE , STATE_WALK_F, STATE_ATTACK_DEFAULT, STATE_ATTACK_CROSS, STATE_DEATH_F, STATE_DEATH_L, STATE_DEATH_LL, STATE_DEATH_R,STATE_DEATH_RL};
private:
	explicit CBrawler01(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBrawler01(const CBrawler01& rhs);
	virtual ~CBrawler01() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();



private:
	virtual HRESULT Ready_Component();
private:
	void Set_DeathIndex();
public:
	static CBrawler01* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject();
protected:
	virtual void Free();
};

_END