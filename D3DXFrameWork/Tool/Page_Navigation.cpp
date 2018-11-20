// Page_Navigation.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_Navigation.h"
#include "afxdialogex.h"
#include "ToolTerrain.h"
#include "Object_Manager.h"
#include "MainFrm.h"
// CPage_Navigation 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Navigation, CPropertyPage)

CPage_Navigation::CPage_Navigation()
	: CPropertyPage(IDD_PAGE_NAVIGATION)
	, m_fNavPosX(0)
	, m_fNavPosY(0)
	, m_fNavPosZ(0)
{

}

CPage_Navigation::~CPage_Navigation()
{
}

void CPage_Navigation::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fNavPosX);
	DDX_Text(pDX, IDC_EDIT2, m_fNavPosY);
	DDX_Text(pDX, IDC_EDIT18, m_fNavPosZ);
	DDX_Control(pDX, IDC_LIST2, m_PointList);
	DDX_Control(pDX, IDC_LIST1, m_NavList);
}

void CPage_Navigation::Set_VecPointNav(vector<_vec3>* pVecPoint)
{
	m_vecPoint = *pVecPoint;
	int a = m_vecPoint.size();
}
//
//void CPage_Navigation::Set_PickingPoint(_vec3 vPickingPoint)
//{
//	//if (vPickingPoint.x < 0 ||
//	//	vPickingPoint.y < 0 ||
//	//	vPickingPoint.z < 0)
//	//	return;
//
//	//if (0.5 <= vPickingPoint.x - (int)vPickingPoint.x)
//	//	m_fNavPosX = (int)vPickingPoint.x + 1;
//	//else
//	//	m_fNavPosX = (int)vPickingPoint.x;
//	//if (0.5 <= vPickingPoint.z - (int)vPickingPoint.z)
//	//	m_fNavPosZ = (int)vPickingPoint.z + 1;
//	//else
//	//	m_fNavPosZ = (int)vPickingPoint.z;
//	//m_fNavPosY = vPickingPoint.y + 0.5f;
//
//	//_tchar szPoint[MAX_PATH] = L"";
//
//	//_stprintf_s(szPoint, L"(%f,%f,%f)", m_fNavPosX, m_fNavPosY, m_fNavPosZ);
//	//m_PointList.AddString(szPoint);
//	//m_vArray[m_PointList.GetCount() - 1] = _vec3(m_fNavPosX, m_fNavPosY, m_fNavPosZ);
//	//if (m_PointList.GetCount() == 3)
//	//{
//	//	const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
//	//	if (nullptr != pTerrain)
//	//		((CToolTerrain*)pTerrain)->Add_Cell(m_vArray);
//	//	
//	//	for (size_t i = 0; i < 3; i++)
//	//	{
//	//		m_vecPoint.push_back(m_vArray[i]); // 데이터 저장을 위함
//	//	}
//	//	wsprintf(szPoint, L"Cell_%d", m_NavList.GetCount());
//	//	m_NavList.AddString(szPoint);
//	//	m_PointList.ResetContent();
//	//}
//	//UpdateData(0);
//}


// 테스트용 글씨 입니다.

BEGIN_MESSAGE_MAP(CPage_Navigation, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Navigation::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON10, &CPage_Navigation::OnBnClickedButton10)
END_MESSAGE_MAP()


// CPage_Navigation 메시지 처리기입니다.


void CPage_Navigation::OnBnClickedButton1() // Save
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		FileDlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("StaticObjectInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		_ulong		dwNumCell = m_vecPoint.size();

		WriteFile(hFile, &dwNumCell, sizeof(_ulong), &dwByte, nullptr);

		for (auto& vPoint : m_vecPoint)
		{
			WriteFile(hFile, vPoint , sizeof(_vec3), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}

	return;
}


void CPage_Navigation::OnBnClickedButton10() // Load
{
	m_vecPoint.clear();
	m_NavList.ResetContent();
	m_PointList.ResetContent();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		FileDlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TileInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;
		_ulong		dwReserve = 0;
		ReadFile(hFile, &dwReserve, sizeof(_ulong), &dwByte, nullptr);

		m_vecPoint.reserve(dwReserve);

		while (true)
		{
			_vec3		vPoint[3];

			ReadFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

			if (0 == dwByte)
				break;

			const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
			if (nullptr != pTerrain)
				((CToolTerrain*)pTerrain)->Add_Cell(vPoint);


			for (size_t i = 0; i < 3; i++)
			{
				m_vecPoint.push_back(vPoint[i]);
			}
			_tchar szPoint[MAX_PATH] = L"";
			wsprintf(szPoint, L"Cell_%d", m_NavList.GetCount());
			m_NavList.AddString(szPoint);
		}

		CloseHandle(hFile);
	}
	UpdateData(0);


	return;
}



BOOL CPage_Navigation::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
	if (nullptr != pTerrain)
	{
		((CToolTerrain*)pTerrain)->Set_MouseState(STATE_ADD_NAV);
	}
	return CPropertyPage::OnSetActive();
}
