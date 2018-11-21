// Sheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Sheet.h"


// CSheet

IMPLEMENT_DYNAMIC(CSheet, CPropertySheet)

CSheet::CSheet()
{
	AddPage(&m_Page_Terrain);
	AddPage(&m_Page_Object);
	AddPage(&m_Page_Animation);
	AddPage(&m_Page_Navigation);
	AddPage(&m_Page_CinemaCam);
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
}


BEGIN_MESSAGE_MAP(CSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSheet 메시지 처리기입니다.
