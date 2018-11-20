#pragma once

#include "Base.h"
#include "Scene.h"

// 씬을 보관 관리한다.
// 필요한 공간할당에 대한 작업을 수행한다.(오브젝트 매니져)
// dll에서 사용된 관리클래스에 대한 메모리 해제.

_BEGIN(Engine)

class _ENGINE_DLL CManagement final : public CBase
{
	_DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement();
	virtual ~CManagement() = default;
public:
	HRESULT Ready_Management(const _uint& iMaxScene);
	_int Update_Management(const _float& fTimeDelta);
	void Render_Management();
public:
	HRESULT SetUp_CurrentScene(CScene* pScene);
public:
	static void Release_Engine();
private:
	CScene*				m_pScene = nullptr;
protected:
	virtual void Free();

};

_END