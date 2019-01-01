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
private: 
	HRESULT Loading_Stage_APT();
	HRESULT Loading_Stage_FIELD();
	HRESULT Loading_Stage_AIRPLANE();
	HRESULT Ready_Effect();
	HRESULT Layer_Effect_FIELD();
	//HRESULT Loading_();
private:
	HRESULT Ready_LightInfo();
	HRESULT Ready_LightInfo_FIELD();
	HRESULT Ready_LightInfo_AIRPLANE();
private:
	HRESULT Ready_Static_Prototype_Component(); // Static���� �������� ���� ������Ʈ����
	HRESULT Ready_Static_Prototype_UI(); // Static���� ���� ���� UI ���� ��ü��
	HRESULT Ready_Static_Layer_UI(); // Static���� ���̴� UI��
	HRESULT Ready_Stage_Prototype_GameObject_SceneAPT(); // APT���� �������� ���� ��ü ����.
	HRESULT Ready_Componet_Prototype_SceneAPT(); // APT���� ���� ���� Mesh ���� ����
	HRESULT Ready_UI_SceneAPT();

	HRESULT Ready_Stage_Prototype_GameObject_SceneFIELD(); // FIELD���� �������� ���� ��ü ����.
	HRESULT Ready_Component_Prototype_SceneFIELD(); // FIELD���� ���� ���� Mesh ���� ����
	HRESULT Ready_UI_SceneFIELD();

	HRESULT Ready_AIRPLANE(); // AIRPLANE������ ���̴� ������Ÿ�԰� ������Ʈ ��ü��
	HRESULT Ready_UI_SceneAIRPLANE();

private:
	HRESULT Ready_Layer_Camera(const _tchar * pProtoName, const _tchar* pLayerTag, const _float fNear);
	HRESULT Ready_Layer_Test(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag); // APT �ͷ��� ���� ����ϱ����� ��ü ����.

	HRESULT Ready_Layer_Object();
	HRESULT Ready_Layer_Object_Airplane();

	HRESULT Ready_Layer_Player_FIELD(const _tchar* pLayerTag); // FIELD �÷��̾� / Į
	HRESULT Ready_Layer_BackGround_FIELD(const _tchar* pLayerTag); // FIELD �ͷ��� / ��ī��
	HRESULT Ready_Layer_BackGround_AIRPLANE(const _tchar* pLayerTag); // AIRPLANE �ͷ��� / ��ī��
private:
	HRESULT Load_Static_Object(const _tchar* pFilePath, _uint iStage = 0);
	HRESULT Load_Event_Cube(const _tchar* pFilePath);
	HRESULT Load_Trigger_CubeAPT();
	HRESULT Load_Trigger_Cube(const _tchar* pFilePath);
	HRESULT Ready_Sound();
private:
	HRESULT SetUp_CameraMove();
private:
	static _uint APIENTRY Thread_Main(void* pArg);
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	SCENEID						m_eSceneID = SCENE_END;
	HANDLE						m_hThread = 0;
	CRITICAL_SECTION			m_Critical_Section;

private:
	_tchar						m_szString[128] = L"";
	_bool						m_isFinish = false;
	_float						m_fPercent = 0;
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID);
protected:
	virtual void Free();
};

_END