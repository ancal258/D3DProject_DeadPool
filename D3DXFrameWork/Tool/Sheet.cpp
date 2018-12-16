// Sheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Sheet.h"
#include "Object_Manager.h"
#include "ToolTerrain.h"

// CSheet

IMPLEMENT_DYNAMIC(CSheet, CPropertySheet)

void CSheet::Add_CamPoint(_vec3 vPoint)
{
	CGameObject* pTerrain = const_cast<CGameObject*>(CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0));
	if (nullptr != pTerrain)
	{
		if (STATE_ADD_CAM == ((CToolTerrain*)pTerrain)->Get_MouseState())
		{
			m_Page_CinemaCam.Add_CamPoint(vPoint);
		}
		if (STATE_ADD_AIRPLANEPATH == ((CToolTerrain*)pTerrain)->Get_MouseState())
		{
			m_Page_AirplanePath.Add_CamPoint(vPoint);
		}
	}


}

CSheet::CSheet()
{
	AddPage(&m_Page_Terrain);
	AddPage(&m_Page_Object);
	AddPage(&m_Page_Animation);
	AddPage(&m_Page_Navigation);
	AddPage(&m_Page_CinemaCam);
	AddPage(&m_Page_AirplanePath);
}

CSheet::CSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CSheet::CSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CSheet::~CSheet()
{
	m_Page_Terrain.DestroyWindow();
	m_Page_Object.DestroyWindow();
	m_Page_Animation.DestroyWindow();
	m_Page_Navigation.DestroyWindow();
	m_Page_CinemaCam.DestroyWindow();
	m_Page_AirplanePath.DestroyWindow();
}


BEGIN_MESSAGE_MAP(CSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSheet 메시지 처리기입니다.
