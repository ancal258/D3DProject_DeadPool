// Page_CinemaCam.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_CinemaCam.h"
#include "afxdialogex.h"


// CPage_CinemaCam 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_CinemaCam, CPropertyPage)

CPage_CinemaCam::CPage_CinemaCam()
	: CPropertyPage(IDD_PAGE_CINEMACAM)
	, m_fCameraAtX(0)
	, m_fCameraAtY(0)
	, m_fCameraAtZ(0)
{

}

CPage_CinemaCam::~CPage_CinemaCam()
{
}

void CPage_CinemaCam::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fCameraAtX);
	DDX_Text(pDX, IDC_EDIT2, m_fCameraAtY);
	DDX_Text(pDX, IDC_EDIT3, m_fCameraAtZ);
}


BEGIN_MESSAGE_MAP(CPage_CinemaCam, CPropertyPage)
END_MESSAGE_MAP()


// CPage_CinemaCam 메시지 처리기입니다.
