#pragma once

#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)

class CScene_Stage final : public CScene
{
private:
	explicit CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Stage() = default;
public:
	virtual HRESULT Ready_Scene() ;
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual _int LastUpdate_Scene(const _float& fTimeDelta);
	virtual void Render_Scene();

private: // Frame
	_ulong						m_dwRenderCnt = 0;
	_float						m_fTimeAcc = 0.f;
	_tchar						m_szFPS[128] = L"";
//private:
//	HRESULT Ready_LightInfo();
//private:
//	HRESULT Ready_Stage_Prototype_Component(); // Stage���� �������� ���� ������Ʈ����
//	HRESULT Ready_Stage_Prototype_GameObject(); // Stage���� �������� ���� ��ü ����.
//	HRESULT Ready_Componet_Prototype_SceneAPT(); // APT���� ���� ���� Static_Mesh ���� ����
//private:
//	HRESULT Load_Static_Object(const _tchar* pPathTag);
//private:	
//	HRESULT Ready_Layer_Camera(const _tchar * pProtoName, const _tchar* pLayerTag);
//	HRESULT Ready_Layer_Test(const _tchar* pLayerTag);
//	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
//	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // �ΰ��� ���� ����ϱ����� ��ü ����.
//private:
//	HRESULT SetUp_CameraMove();
public:
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

_END