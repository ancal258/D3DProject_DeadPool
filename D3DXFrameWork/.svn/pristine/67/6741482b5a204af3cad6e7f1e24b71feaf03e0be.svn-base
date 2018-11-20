// Page_Animation.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_Animation.h"
#include "afxdialogex.h"

// View
#include "ToolView.h"
#include "MainFrm.h"

#include "Object_Manager.h"
#include "Component_Manager.h"

#include "Player.h"
// CPage_Animation 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Animation, CPropertyPage)

CPage_Animation::CPage_Animation()
	: CPropertyPage(IDD_PAGE_ANIMATION)
	, m_iAnimationIndex(0)
	, m_fSpeed(0)
	, m_fArrived(0)
	, m_fTimeDelta(0)
{

}

CPage_Animation::~CPage_Animation()
{
}

void CPage_Animation::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_iAnimationIndex);
	DDX_Text(pDX, IDC_EDIT4, m_fSpeed);
	DDX_Text(pDX, IDC_EDIT15, m_fArrived);
	DDX_Text(pDX, IDC_EDIT16, m_fTimeDelta);
}


BEGIN_MESSAGE_MAP(CPage_Animation, CPropertyPage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CPage_Animation::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_Animation::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Animation::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CPage_Animation::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON10, &CPage_Animation::OnBnClickedButton10)
END_MESSAGE_MAP()


// CPage_Animation 메시지 처리기입니다.


void CPage_Animation::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


BOOL CPage_Animation::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// Set Range
	m_SliderCtrl.SetRange(0, 255);
	// Set Min Value of Range
	m_SliderCtrl.SetRangeMin(0);
	// Set Max Value of Range
	m_SliderCtrl.SetRangeMax(255);
	// Set Position(value)
	m_SliderCtrl.SetPos(100);

	// set tick frequency *need to Tick Marks and Auto Ticks set True*
	m_SliderCtrl.SetTicFreq(10);
	// set size by keyboard move-key (<-,->)
	m_SliderCtrl.SetLineSize(1);
	// set size by mouse click or keyboard PgUp, PgDn key
	m_SliderCtrl.SetPageSize(10);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPage_Animation::OnBnClickedButton2() // Create Player
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();

	if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Player", L"Layer_Player", &m_pMesh)))
		return;

}


void CPage_Animation::OnBnClickedButton1() // Animation Index Setting
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if(nullptr != m_pMesh)
		((CPlayer*)m_pMesh)->Set_AnimationInfo(m_fTimeDelta,m_fSpeed, m_fArrived, m_iAnimationIndex);
}


void CPage_Animation::OnBnClickedButton6() // Animation & Move Start
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr != m_pMesh)
		((CPlayer*)m_pMesh)->Set_Play();
	
}


void CPage_Animation::OnBnClickedButton10() // Animation & Move Stop
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr != m_pMesh)
		((CPlayer*)m_pMesh)->Set_Stop();
}
