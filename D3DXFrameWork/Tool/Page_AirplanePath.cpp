// Page_AirplanePath.cpp : ���� �����Դϴ�.
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


// CPage_AirplanePath ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPage_AirplanePath, CPropertyPage)

CPage_AirplanePath::CPage_AirplanePath()
	: CPropertyPage(IDD_PAGE_AIRPLANEPATH)
	, m_fValue(0)
	, m_fX(0)
	, m_fY(0)
	, m_fZ(0)
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
		// ������Ʈ �޽��� �߰��ɶ����� �����̳ʿ� ����, ���� ���� �� �� ��ȸ�ϸ鼭 ������ ����.
		m_vecCamPoints.push_back(pMesh);
		((CCamPoint*)pMesh)->Set_Position(&vPoint);
	}
	if (m_vecCamPoints.size() % 4 == 0)
	{
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_CamPoint", L"Layer_CamPoint", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			_tchar szName[MAX_PATH];
			wsprintf(szName, L"Point_%d", m_vecCamPoints.size());
			m_PointList.AddString(szName);
			// ������Ʈ �޽��� �߰��ɶ����� �����̳ʿ� ����, ���� ���� �� �� ��ȸ�ϸ鼭 ������ ����.
			m_vecCamPoints.push_back(pMesh);
			((CCamPoint*)pMesh)->Set_Position(&vPoint);
		}
	}
}

void CPage_AirplanePath::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fValue);
	DDX_Control(pDX, IDC_LIST1, m_PointList);
	DDX_Text(pDX, IDC_EDIT2, m_fX);
	DDX_Text(pDX, IDC_EDIT3, m_fY);
	DDX_Text(pDX, IDC_EDIT4, m_fZ);
}


BEGIN_MESSAGE_MAP(CPage_AirplanePath, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_AirplanePath::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_AirplanePath::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CPage_AirplanePath::OnBnClickedButton6)
END_MESSAGE_MAP()


// CPage_AirplanePath �޽��� ó�����Դϴ�.


BOOL CPage_AirplanePath::OnSetActive()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
	if (nullptr != pTerrain)
	{
		((CToolTerrain*)pTerrain)->Set_MouseState(STATE_ADD_AIRPLANEPATH);
	}
	return CPropertyPage::OnSetActive();
}


void CPage_AirplanePath::OnBnClickedButton1() // Save
{
	if ((m_vecCamPoints.size() - 1) % 4 != 0)
		return;

	for (auto iter = m_vecCamPoints.begin(); iter != m_vecCamPoints.end(); ) 
	{

		if (*iter == m_vecCamPoints[m_vecCamPoints.size() - 1]) 
		{

			iter = m_vecCamPoints.erase(iter);
		}
		else 
		{
			iter++;

		}
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog		FileDlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("StaticObjectInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;
		for (auto& pPathPoint : m_vecCamPoints) // �̺�Ʈ�� �� ����
		{
			WriteFile(hFile, &((CCamPoint*)pPathPoint)->Get_ObjectInfo(), sizeof(_vec3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CPage_AirplanePath::OnBnClickedButton2() // Load
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;

	CFileDialog		FileDlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TileInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;
		while (true)
		{
			_vec3 vPosition;


			ReadFile(hFile, &vPosition, sizeof(_vec3), &dwByte, nullptr);

			if (0 == dwByte)
			{
				break;
			}
			if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_CamPoint", L"Layer_CamPoint", &pMesh)))
				return;
			if (nullptr != pMesh)
			{
				_tchar szName[MAX_PATH];
				wsprintf(szName, L"Point_%d", m_vecCamPoints.size());
				m_PointList.AddString(szName);
				// ������Ʈ �޽��� �߰��ɶ����� �����̳ʿ� ����, ���� ���� �� �� ��ȸ�ϸ鼭 ������ ����.
				m_vecCamPoints.push_back(pMesh);
				((CCamPoint*)pMesh)->Set_Position(&vPosition);
			}
		}

		CloseHandle(hFile);
	}
	return;
}


void CPage_AirplanePath::OnBnClickedButton6() // Add Start Point
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(1);
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;

	if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_CamPoint", L"Layer_CamPoint", &pMesh)))
		return;
	if (nullptr != pMesh)
	{
		_tchar szName[MAX_PATH];
		wsprintf(szName, L"Point_%d", m_vecCamPoints.size());
		m_PointList.AddString(szName);
		// ������Ʈ �޽��� �߰��ɶ����� �����̳ʿ� ����, ���� ���� �� �� ��ȸ�ϸ鼭 ������ ����.
		m_vecCamPoints.push_back(pMesh);
		((CCamPoint*)pMesh)->Set_Position(&_vec3(m_fX,m_fY,m_fZ));
	}
}
