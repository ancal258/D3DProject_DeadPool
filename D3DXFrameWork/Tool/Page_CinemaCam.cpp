// Page_CinemaCam.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_CinemaCam.h"
#include "Graphic_Device.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "CamPoint.h"
#include "ToolTerrain.h"
#include "Object_Manager.h"


// CPage_CinemaCam ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPage_CinemaCam, CPropertyPage)

CPage_CinemaCam::CPage_CinemaCam()
	: CPropertyPage(IDD_PAGE_CINEMACAM)
	, m_fCameraAtX(0)
	, m_fCameraAtY(0)
	, m_fCameraAtZ(0)
	, m_fTime(0)
	, m_szCamName(_T(""))
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
	DDX_Text(pDX, IDC_EDIT18, m_fCameraAtZ);
	DDX_Text(pDX, IDC_EDIT3, m_fTime);
	DDX_Control(pDX, IDC_LIST3, m_CamList);
	DDX_Text(pDX, IDC_EDIT4, m_szCamName);
	DDX_Control(pDX, IDC_LIST1, m_PointList);
}


BEGIN_MESSAGE_MAP(CPage_CinemaCam, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage_CinemaCam::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage_CinemaCam::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPage_CinemaCam::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON17, &CPage_CinemaCam::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_CinemaCam::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPage_CinemaCam �޽��� ó�����Դϴ�.


void CPage_CinemaCam::Add_CamPoint(_vec3 vPoint)
{
		CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CGameObject*	pMesh = nullptr;
	if (m_fCameraAtX == 0 &&
		m_fCameraAtY == 0 &&
		m_fCameraAtZ == 0)
	{
		m_fCameraAtX = vPoint.x;
		m_fCameraAtY = vPoint.y;
		m_fCameraAtZ = vPoint.z;

		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_CamPoint", L"Layer_CamPoint", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			//wsprintf(szMeshName, L"Another_Sofa_%d", iMeshCntArray[MESH_ANOTHER_SOFA]);

			m_vecLookAt.push_back(pMesh); // ������Ʈ �޽��� �߰��ɶ����� �����̳ʿ� ����, ���� ���� �� �� ��ȸ�ϸ鼭 ������ ����.
											  //m_MeshListBox.AddString(szMeshName);
			((CCamPoint*)pMesh)->Set_Position(&vPoint);
			((CCamPoint*)pMesh)->Set_Scale(&_vec3(2, 2, 2));
		}
	}
	else
	{
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_CamPoint", L"Layer_CamPoint", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			//wsprintf(szMeshName, L"Another_Sofa_%d", iMeshCntArray[MESH_ANOTHER_SOFA]);
			_tchar szName[MAX_PATH];
			wsprintf(szName, L"Point_%d", m_vecCamPoints.size());
			m_PointList.AddString(szName);
			m_vecCamPoints.push_back(pMesh); // ������Ʈ �޽��� �߰��ɶ����� �����̳ʿ� ����, ���� ���� �� �� ��ȸ�ϸ鼭 ������ ����.
			((CCamPoint*)pMesh)->Set_Position(&vPoint);
		}
	}
	UpdateData(0);
}

void CPage_CinemaCam::OnBnClickedButton3() // Add
{
	UpdateData(1);

	if (0 == m_vecCamPoints.size() % 4)
	{
		m_vvSaveWorks.push_back(m_vecCamPoints);
		m_vTimes.push_back(m_fTime);

		_tchar szName[MAX_PATH];
		m_szCamName = L"%d." + m_szCamName;
		wsprintf(szName, m_szCamName, m_vvSaveWorks.size());
		m_CamList.AddString(szName);

		m_vecCamPoints.clear();
		m_PointList.ResetContent();
		m_szCamName = L"";

		m_fCameraAtX = 0;
		m_fCameraAtY = 0;
		m_fCameraAtZ = 0;
	}
	UpdateData(0);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CPage_CinemaCam::OnBnClickedButton4() // Delete Cam
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CPage_CinemaCam::OnBnClickedButton5() // Save
{
	UpdateData(1);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog		FileDlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("StaticObjectInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;
		_uint dwIdx = m_CamList.GetCurSel();
		_uint iNumPoints = m_vvSaveWorks[dwIdx].size();

		WriteFile(hFile, &iNumPoints, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &m_vTimes[dwIdx], sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &((CCamPoint*)m_vecLookAt[dwIdx])->Get_ObjectInfo(), sizeof(_vec3), &dwByte, nullptr);
		for (auto& vPoints : m_vvSaveWorks[dwIdx]) // �ϳ��� ķ ��ũ
		{
			// 3. vPointsSave vPoints;
			WriteFile(hFile, &((CCamPoint*)vPoints)->Get_ObjectInfo(), sizeof(_vec3), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CPage_CinemaCam::OnBnClickedButton17() // Load
{
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
		while (true)
		{
			_uint iNumPoints = 0;
			_float fTime = 0;
			_vec3 vLookAt;
			_vec3 vPoint;

			ReadFile(hFile, &iNumPoints, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &fTime, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &vLookAt, sizeof(_vec3), &dwByte, nullptr);
			for (int i = 0; i < iNumPoints; ++i)
			{
				ReadFile(hFile, &vPoint, sizeof(_vec3), &dwByte, nullptr);
			}

			if (0 == dwByte)
				break;
		}

		CloseHandle(hFile);
	}
	return;
}


void CPage_CinemaCam::OnBnClickedButton1() // Set
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(0);
}


BOOL CPage_CinemaCam::OnSetActive()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
	if (nullptr != pTerrain)
	{
		((CToolTerrain*)pTerrain)->Set_MouseState(STATE_ADD_CAM);
	}
	return CPropertyPage::OnSetActive();
}