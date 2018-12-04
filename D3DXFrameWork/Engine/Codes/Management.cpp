#include "..\Headers\Management.h"
#include "Timer_Manager.h"
#include "Frame_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Target_Manager.h"
_IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
{
}

HRESULT CManagement::Ready_Management(const _uint & iMaxScene)
{
	if (FAILED(CComponent_Manager::GetInstance()->Reserve_Component_Manager(iMaxScene)))
		return E_FAIL;

	if (FAILED(CObject_Manager::GetInstance()->Reserve_Object_Manager(iMaxScene)))
		return E_FAIL;

	return NOERROR;
}

_int CManagement::Update_Management(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	_int		iExitCode = 0;

	iExitCode = m_pScene->Update_Scene(fTimeDelta);
	if (iExitCode & 0x80000000)
		return iExitCode;

	iExitCode = m_pScene->LastUpdate_Scene(fTimeDelta);
	if (iExitCode & 0x80000000)
		return iExitCode;

	return _int();
}

void CManagement::Render_Management()
{
	if (nullptr == m_pScene)
		return ;

	m_pScene->Render_Scene();
}

HRESULT CManagement::SetUp_CurrentScene(CScene * pScene)
{
	Safe_Release(m_pScene);

	m_pScene = pScene;

	return NOERROR;
}

void CManagement::Release_Engine()
{
	_ulong			dwRefCnt = 0;

	if (dwRefCnt = CFrame_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX("CFrame_Manager Release Failed");

	if (dwRefCnt = CTimer_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX("CTimer_Manager Release Failed");

	

	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
		_MSG_BOX("CManagement Release Failed");

	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX("CObject_Manager Release Failed");

	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX("CComponent_Manager Release Failed");

	if (dwRefCnt = CLight_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX("CLight_Manager Release Failed");

	if (dwRefCnt = CTarget_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX("CTarget_Manager Release Failed");

	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
		_MSG_BOX("CGraphic_Device Release Failed");

	if (dwRefCnt = CInput_Device::GetInstance()->DestroyInstance())
		_MSG_BOX("CInput_Device Release Failed");
}

void CManagement::Free()
{
	Safe_Release(m_pScene);
}
