// Page_Terrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_Terrain.h"
#include "afxdialogex.h"

// View
#include "ToolView.h"
#include "MainFrm.h"
// CPage_Terrain
#include "Graphic_Device.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "ToolCamera.h"
#include "ToolTerrain.h"
#include "Texture_Bmp.h"


// CPage_Terrain 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Terrain, CPropertyPage)

CPage_Terrain::CPage_Terrain()
	: CPropertyPage(IDD_PAGE_TERRAIN)
	, m_iTerrainX(0)
	, m_iTerrainZ(0)
	, m_fKeyboardSpeed(0)
	, m_fMouseSpeed(0)
	, m_fRadius(0)
	, m_fPower(0)
{

}

CPage_Terrain::~CPage_Terrain()
{
}

void CPage_Terrain::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iTerrainX);
	DDX_Text(pDX, IDC_EDIT2, m_iTerrainZ);
	DDX_Text(pDX, IDC_EDIT4, m_fKeyboardSpeed);
	DDX_Text(pDX, IDC_EDIT5, m_fMouseSpeed);
	DDX_Text(pDX, IDC_EDIT3, m_fRadius);
	DDX_Text(pDX, IDC_EDIT6, m_fPower);
	DDX_Control(pDX, IDD_PICTURE, m_Picture1);
	DDX_Control(pDX, IDD_PICTURE2, m_Picture2);
	DDX_Control(pDX, IDD_PICTURE3, m_Picture3);
	DDX_Control(pDX, IDD_PICTURE4, m_Picture4);
}


BEGIN_MESSAGE_MAP(CPage_Terrain, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Terrain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_Terrain::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage_Terrain::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage_Terrain::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPage_Terrain::OnBnClickedButton5)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON6, &CPage_Terrain::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CPage_Terrain::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_RADIO1, &CPage_Terrain::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CPage_Terrain::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CPage_Terrain::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CPage_Terrain::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CPage_Terrain::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CPage_Terrain::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CPage_Terrain::OnBnClickedRadio7)
END_MESSAGE_MAP()


// CPage_Terrain 메시지 처리기입니다.


BOOL CPage_Terrain::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pBmp = CTexture_Bmp::Create(CGraphic_Device::GetInstance()->Get_GraphicDev(), L"Bin/Resources/Textures/Terrain/Grass_%d.bmp",4);
	if (nullptr == m_pBmp)
		return FALSE;
	m_pBmp->SetUp_OnControl(&m_Picture1, 0);
	m_pBmp->SetUp_OnControl(&m_Picture2, 1);
	m_pBmp->SetUp_OnControl(&m_Picture3, 2);
	m_pBmp->SetUp_OnControl(&m_Picture4, 3);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPage_Terrain::OnBnClickedButton1()
{
	UpdateData(1);

	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	
	if (FAILED(CComponent_Manager::GetInstance()->Add_Component(0, L"Component_Buffer_Terrain", CBuffer_Terrain::Create(CGraphic_Device::GetInstance()->Get_GraphicDev(), m_iTerrainX, m_iTerrainZ, 4.f))))
		return;

	//if (FAILED(CObject_Manager::GetInstance()->Add_Object(0, L"Prototype_Terrain", 0, L"Layer_Terrain", nullptr)))
	//	return;
	if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Terrain", L"Layer_Terrain", &m_pTerrain)))
		return;



	((CToolTerrain*)m_pTerrain)->Set_TerrainSize(m_iTerrainX, m_iTerrainZ);

	CGameObject* pCamera = const_cast<CGameObject*>(CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Camera", 0));
	m_fMouseSpeed = ((CToolCamera*)pCamera)->m_Rotate_Speed;
	m_fKeyboardSpeed = ((CToolCamera*)pCamera)->m_Move_Speed;
	UpdateData(0);

	pMainFrame->m_pToolView->Invalidate(FALSE);
}

void CPage_Terrain::OnBnClickedButton2() // KeyBoard Up
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGameObject* pCamera = const_cast<CGameObject*>(CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Camera", 0));
	if (pCamera != nullptr)
	{
		m_fKeyboardSpeed += 0.01f;
		((CToolCamera*)pCamera)->m_Move_Speed = m_fKeyboardSpeed;
		UpdateData(0);
	}

}


void CPage_Terrain::OnBnClickedButton3()// KeyBoard Down
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGameObject* pCamera = const_cast<CGameObject*>(CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Camera", 0));
	if (pCamera != nullptr)
	{
		m_fKeyboardSpeed -= 0.01f;
		((CToolCamera*)pCamera)->m_Move_Speed = m_fKeyboardSpeed;
		UpdateData(0);
	}

}

void CPage_Terrain::OnBnClickedButton4() // Mouse Down
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGameObject* pCamera = const_cast<CGameObject*>(CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Camera", 0));
	if (pCamera != nullptr)
	{
		m_fMouseSpeed += 0.01f;
		((CToolCamera*)pCamera)->m_Rotate_Speed = m_fMouseSpeed;
		UpdateData(0);
	}
}

void CPage_Terrain::OnBnClickedButton5()// Mouse Down
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGameObject* pCamera = const_cast<CGameObject*>(CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Camera", 0));
	if (pCamera != nullptr)
	{
		m_fMouseSpeed -= 0.01f;
		((CToolCamera*)pCamera)->m_Rotate_Speed = m_fMouseSpeed;
		UpdateData(0);
	}
}


void CPage_Terrain::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (m_pTerrain != nullptr)
	{
		((CToolTerrain*)m_pTerrain)->Set_BrushRadius(m_fRadius);
		((CToolTerrain*)m_pTerrain)->Set_BrushPower(m_fPower);
	}

}
void CPage_Terrain::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pTerrain != nullptr)
		((CToolTerrain*)m_pTerrain)->Save_HeightMap();

}



void CPage_Terrain::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eMouseState = STATE_DRAWUP;
	if (m_pTerrain != nullptr)
		((CToolTerrain*)m_pTerrain)->Set_MouseState(m_eMouseState);
}

void CPage_Terrain::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eMouseState = STATE_TILE1;
	if (m_pTerrain != nullptr)
		((CToolTerrain*)m_pTerrain)->Set_MouseState(m_eMouseState);

}


void CPage_Terrain::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eMouseState = STATE_TILE2;
	if (m_pTerrain != nullptr)
		((CToolTerrain*)m_pTerrain)->Set_MouseState(m_eMouseState);

}


void CPage_Terrain::OnBnClickedRadio4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eMouseState = STATE_DRAWDOWN;
	if (m_pTerrain != nullptr)
		((CToolTerrain*)m_pTerrain)->Set_MouseState(m_eMouseState);
}


void CPage_Terrain::OnBnClickedRadio5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eMouseState = STATE_ADD_OBJECT;
	if (m_pTerrain != nullptr)
		((CToolTerrain*)m_pTerrain)->Set_MouseState(m_eMouseState);
}




void CPage_Terrain::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CPropertyPage::OnMouseMove(nFlags, point);
}


void CPage_Terrain::OnBnClickedRadio6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eMouseState = STATE_TILE3;
	if (m_pTerrain != nullptr)
		((CToolTerrain*)m_pTerrain)->Set_MouseState(m_eMouseState);
}


void CPage_Terrain::OnBnClickedRadio7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_eMouseState = STATE_TILE4;
	if (m_pTerrain != nullptr)
		((CToolTerrain*)m_pTerrain)->Set_MouseState(m_eMouseState);
}
