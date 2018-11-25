#pragma once

#include "Base.h"
#include "Defines.h"

_BEGIN(Engine)
class CInput_Device;
class CMesh_Dynamic;
class CNavigation;
class CTransform;
_END


_BEGIN(Client)

class CAnimator final : public CBase
{
private:
	enum ANIM_APT {
		SIT_IDLE_BREATH, SIT_IDLE_HANDMOVE, SIT_GETUP, SIT_SITDOWN,
		NOGUN_IDLE00, NOGUN_IDLE01, NOGUN_IDLE02, NOGUN_IDLE03, NOGUN_IDLE04, NOGUN_IDLE05, NOGUN_IDLE06,
		NOGUN_WALK_F, NOGUN_WALK_FL, NOGUN_WALK_FR, NOGUN_WALK_L, NOGUN_WALK_R, NOGUN_WALK_B, INTERACT_DOG,

		ANIM_END
	};
private:
	explicit CAnimator(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CAnimator() = default;
public:
	HRESULT Ready_Animator(CMesh_Dynamic* pMeshCom, CTransform* pTransformCom, CNavigation* pNavigationCom);

public:
	void Update_Animation(const _float & fTimeDelta);
	
	//HRESULT SetUp_OneButtonAnimation()

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;
	CInput_Device* m_pInput_Device = nullptr;
private:
	CMesh_Dynamic*      m_pMeshCom = nullptr;
	CTransform*         m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;
	_bool				m_ArrayAnimState[ANIM_END] = { 0 };
public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device,CMesh_Dynamic* pMeshCom, CTransform* pTransformCom, CNavigation* pNavigationCom);
protected:
	virtual void Free();
};

_END