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
#include "TriggerCube.h"

// CPage_AirplanePath ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPage_AirplanePath, CPropertyPage)

CPage_AirplanePath::CPage_AirplanePath()
	: CPropertyPage(IDD_PAGE_AIRPLANEPATH)
	, m_fValue(0)
	, m_fX(0)
	, m_fY(0)
	, m_fZ(0)
	, m_szButton(_T(""))
	, m_szString(_T(""))
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
	DDX_Control(pDX, IDC_LIST2, m_StringList);
	DDX_Control(pDX, IDC_LIST4, m_ButtonList);
	DDX_Text(pDX, IDC_EDIT6, m_szButton);
	DDX_Text(pDX, IDC_EDIT5, m_szString);
	DDX_Control(pDX, IDC_LIST3, m_TriggerList);
}


BEGIN_MESSAGE_MAP(CPage_AirplanePath, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_AirplanePath::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_AirplanePath::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CPage_AirplanePath::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON16, &CPage_AirplanePath::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON7, &CPage_AirplanePath::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON19, &CPage_AirplanePath::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON20, &CPage_AirplanePath::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON8, &CPage_AirplanePath::OnBnClickedButton8)
	ON_LBN_SELCHANGE(IDC_LIST3, &CPage_AirplanePath::OnLbnSelchangeList3)
	ON_BN_CLICKED(IDC_BUTTON9, &CPage_AirplanePath::OnBnClickedButton9)
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


void CPage_AirplanePath::OnBnClickedButton16() // Trigger ��ü ����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;

	if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_TriggerCube", L"Layer_TriggerCube", &pMesh)))
		return;
	m_vecTrigger.push_back((CTriggerCube*)pMesh);

	_tchar szName[MAX_PATH];
	wsprintf(szName, L"Trigger_%d", m_vecTrigger.size());
	m_TriggerList.AddString(szName);
}


void CPage_AirplanePath::OnBnClickedButton7() // ���ڿ� ����Ʈ�� ����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(1);
	m_StringList.AddString(m_szString);
	_tchar szTmp[128];
	wsprintf(szTmp, m_szString);
	m_vecSaveString.push_back(szTmp);

}


void CPage_AirplanePath::OnBnClickedButton19() // ��ư ���ڿ� ����Ʈ�� ����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(1);
	m_ButtonList.AddString(m_szButton);
	_tchar szTmp[128];
	wsprintf(szTmp, m_szButton);
	m_vecSaveButton.push_back(szTmp);

}


void CPage_AirplanePath::OnBnClickedButton20() // Ʈ���ſ� Set
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	for (auto& szString : m_vecSaveString)
		m_vecTrigger[m_iIndex]->Add_String(szString);

	for (auto& szButtonString : m_vecSaveButton)
		m_vecTrigger[m_iIndex]->Add_ButtonString(szButtonString);

	m_vecSaveString.clear();
	m_vecSaveButton.clear();
	m_StringList.ResetContent();
	m_ButtonList.ResetContent();
}


void CPage_AirplanePath::OnBnClickedButton8() // Ŭ���� ��ü ����
{

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog		FileDlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("StaticObjectInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		_uint iStringCnt = m_vecTrigger[m_iIndex]->m_vString.size();

		WriteFile(hFile, &iStringCnt, sizeof(_uint), &dwByte, nullptr);

		for (auto& szString : m_vecTrigger[m_iIndex]->m_vString) // ���� ���Ϳ��� ��������.
		{
			// 3. vPointsSave vPoints;
			_tchar szTmp[128];
			wsprintf(szTmp, szString);
			WriteFile(hFile, szTmp, sizeof(_tchar) * 128, &dwByte, nullptr);
		}

		for (auto& szButton: m_vecTrigger[m_iIndex]->m_vButtonString) // ���� ���Ϳ��� ��������.
		{
			// 3. vPointsSave vPoints;
			_tchar szTmp[128];
			wsprintf(szTmp, szButton);
			WriteFile(hFile, szButton, sizeof(_tchar) * 128, &dwByte, nullptr);
		}
		WriteFile(hFile, m_vecTrigger[m_iIndex]->Get_ObjectInfo(), sizeof(_vec3), &dwByte, nullptr);

		
		CloseHandle(hFile);
	}

}

void CPage_AirplanePath::OnBnClickedButton9() // Load
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;
	_tchar szMeshName[128] = L"";

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog		FileDlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TileInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		_uint iStringCnt = 0;
		_tchar szTmp[128] = L"";
		_vec3 vPoint;

		ReadFile(hFile, &iStringCnt, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iStringCnt; ++i)
		{
			ReadFile(hFile, &szTmp, sizeof(_tchar) * 128, &dwByte, nullptr);
			m_StringList.AddString(szTmp);
		}

		ReadFile(hFile, &szTmp, sizeof(_tchar) * 128, &dwByte, nullptr);
		m_ButtonList.AddString(szTmp);

		ReadFile(hFile, &szTmp, sizeof(_tchar) * 128, &dwByte, nullptr);
		m_ButtonList.AddString(szTmp);

		ReadFile(hFile, &vPoint, sizeof(_vec3), &dwByte, nullptr);

		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_TriggerCube", L"Layer_TriggerCube", &pMesh)))
			return;
		m_vecTrigger.push_back((CTriggerCube*)pMesh);
		m_TriggerList.AddString(L"Load_Trigger");
		((CTriggerCube*)pMesh)->Set_Position(&vPoint);


		CloseHandle(hFile);
	}
	return;
}


void CPage_AirplanePath::OnLbnSelchangeList3() // ListBox
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iIndex = m_TriggerList.GetCurSel();
}

