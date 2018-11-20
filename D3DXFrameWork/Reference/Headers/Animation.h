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

// �÷��̾� -> Enum ���� �ѱ�� -> m_pMesh�� �ִϸ��̼� ó�� ��û
// ó���� ���?
// 