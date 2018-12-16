// Page_AirplanePath.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_AirplanePath.h"
#include "afxdialogex.h"
#include "Graphic_Device.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "CamPoint.h"
#include "ToolTerrain.h"
#include "Object_Manager.h"


// CPage_AirplanePath 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_AirplanePath, CPropertyPage)

CPage_AirplanePath::CPage_AirplanePath()
	: CPropertyPage(IDD_PAGE_AIRPLANEPATH)
	, m_fValue(0)
{

}

CPage_AirplanePath::~CPage_AirplanePath()
{

}

void CPage_AirplanePath::Add_CamPoint(_vec3 vPoint)
{
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;

	if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_CamPoint", L"Layer_CamPoint", &pMesh)))
		return;
	if (nullptr != pMesh)
	{
		_tchar szName[MAX_PATH];
		wsprintf(szName, L"Point_%d", m_vecCamPoints.size());
		m_PointList.AddString(szName);
		// 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
		m_vecCamPoints.push_back(pMesh);
		((CCamPoint*)pMesh)->Set_Position(&vPoint);
	}
}

void CPage_AirplanePath::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fValue);
	DDX_Control(pDX, IDC_LIST1, m_PointList);
}


BEGIN_MESSAGE_MAP(CPage_AirplanePath, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_AirplanePath::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_AirplanePath::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPage_AirplanePath 메시지 처리기입니다.


BOOL CPage_AirplanePath::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
	if (nullptr != pTerrain)
	{
		((CToolTerrain*)pTerrain)->Set_MouseState(STATE_ADD_AIRPLANEPATH);
	}
	return CPropertyPage::OnSetActive();
}


void CPage_AirplanePath::OnBnClickedButton1() // Save
{
	if (m_vecCamPoints.size() % 4 != 0)
		return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		FileDlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("StaticObjectInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;
		for (auto& pPathPoint : m_vecCamPoints) // 이벤트들 싹 저장
		{
			WriteFile(hFile, &((CCamPoint*)pPathPoint)->Get_ObjectInfo(), sizeof(_vec3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CPage_AirplanePath::OnBnClickedButton2() // Load
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
