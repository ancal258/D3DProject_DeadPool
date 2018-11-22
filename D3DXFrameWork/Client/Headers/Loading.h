#pragma once

#include "Base.h"
#include "Defines.h"
#include "Engine_Defines.h"
#include "Object_Manager.h"


_BEGIN(Client)

class CLoading : public CBase
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading() = default;
public:
	SCENEID Get_SceneID() const {
		return m_eSceneID;
	}
	LPCRITICAL_SECTION Get_Critical_Section() {
		return &m_Critical_Section;
	}
	const _tchar* Get_String() const {
		return m_szString;
	}
	_bool Get_Finish() const {
		return m_isFinish;
	}
	LPDIRECT3DDEVICE9 Get_Graphic_Device() {
		return m_pGraphic_Device; }

	HRESULT Add_Object_Prototype(const _uint& iSceneID, const _tchar* pProtoTag, CGameObject* pPrototype);
	HRESULT Add_Object(const _uint& iPSceneID, const _tchar* pProtoTag, const _uint& iSceneID, const _tchar* pLayerTag, CGameObject** ppGameObject = nullptr);

public:
	HRESULT Ready_Loading(SCENEID eSceneID);
	HRESULT Loading_Stage_APT();
	//HRESULT Loading_();
private:
	HRESULT Ready_LightInfo();
private:
	HRESULT Ready_Stage_Prototype_Component(); // Stage���� �������� ���� ������Ʈ����
	HRESULT Ready_Stage_Prototype_GameObject(); // Stage���� �������� ���� ��ü ����.
	HRESULT Ready_Componet_Prototype_SceneAPT(); // APT���� ���� ���� Static_Mesh ���� ����

private:
	HRESULT Ready_Layer_Camera(const _tchar * pProtoName, const _tchar* pLayerTag);
	HRESULT Ready_Layer_Test(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // �ΰ����� ���� ����ϱ����� ��ü ����.
private:
	HRESULT Load_Static_Object(const _tchar* pPathTag);
private:
	HRESULT SetUp_CameraMove();
private:
	static _uint APIENTRY Thread_Main(void* pArg);
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	SCENEID						m_eSceneID;
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Critical_Section;

private:
	_tchar						m_szString[128];
	_bool						m_isFinish = false;
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID);
protected:
	virtual void Free();
};

_END