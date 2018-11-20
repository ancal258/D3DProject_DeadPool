#pragma once

#include "Base.h"

_BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CAnimation() = default;


public:
	static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

_END

// 플레이어 -> Enum 값을 넘기고 -> m_pMesh에 애니메이션 처리 요청
// 처리를 어떻게?
// 