#pragma once

#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)
class CLoading;

class CScene_Logo final : public CScene
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Logo() = default;
public:
	virtual HRESULT Ready_Scene() ;
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual _int LastUpdate_Scene(const _float& fTimeDelta);
	virtual void Render_Scene();
private:
	HRESULT Ready_Logo_Prototype_Component(); // 로고에서 쓰기위한 원형 컴포넌트생성
	HRESULT Ready_Logo_Prototype_GameObject(); // 로고에서 쓰기위한 원형 객체 생성.
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // 로고에서 실제 사용하기위한 객체 생성.

private:
	CLoading*		m_pLoading = nullptr;
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

_END