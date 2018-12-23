// Page_MeshEffect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Page_MeshEffect.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "EffectToolView.h"

//GameObject
#include "Effect_Mesh.h"

// CPage_MeshEffect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_MeshEffect, CPropertyPage)

CPage_MeshEffect::CPage_MeshEffect()
	: CPropertyPage(IDD_PAGE_MESHEFFECT)
	, m_fU(0)
	, m_fV(0)
{

}

CPage_MeshEffect::~CPage_MeshEffect()
{
}

void CPage_MeshEffect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, m_fU);
	DDX_Text(pDX, IDC_EDIT8, m_fV);
	DDX_Control(pDX, IDC_LIST2, m_PrototypeList);
	DDX_Control(pDX, IDC_LIST3, m_ObjectList);
}


BEGIN_MESSAGE_MAP(CPage_MeshEffect, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_MeshEffect::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage_MeshEffect::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CPage_MeshEffect::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &CPage_MeshEffect::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CPage_MeshEffect::OnBnClickedButton7)
	ON_LBN_SELCHANGE(IDC_LIST2, &CPage_MeshEffect::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// CPage_MeshEffect 메시지 처리기입니다.


void CPage_MeshEffect::OnBnClickedButton1() // Load Mesh
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (0 == m_iIndex)
	{
		CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if (FAILED(pMainFrame->m_pEffectToolView->Ready_Layer_Object(L"Prototype_EffectSwordHeavy03R", L"Layer_MeshEffect", &m_pMesh)))
			return;
		m_fU = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_U;
		m_fV = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_V;
	}
	else if (1 == m_iIndex)
	{
		CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if (FAILED(pMainFrame->m_pEffectToolView->Ready_Layer_Object(L"Prototype_EffectGunFlash", L"Layer_MeshEffect", &m_pMesh)))
			return;
		m_fU = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_U;
		m_fV = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_V;
	}
	else if (2 == m_iIndex)
	{
		CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if (FAILED(pMainFrame->m_pEffectToolView->Ready_Layer_Object(L"Prototype_EffectGunCap", L"Layer_MeshEffect", &m_pMesh)))
			return;
		m_fU = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_U;
		m_fV = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_V;
	}
	else if (3 == m_iIndex)
	{

	}
	else if (4 == m_iIndex)
	{

	}


	UpdateData(0);
}


void CPage_MeshEffect::OnBnClickedButton4() // U ▲
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CEffect_Mesh*)m_pMesh)->m_fSpeed_U += 0.01f;
	m_fU = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_U;
	UpdateData(0);
}


void CPage_MeshEffect::OnBnClickedButton6() // U ▼
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CEffect_Mesh*)m_pMesh)->m_fSpeed_U -= 0.01f;
	m_fU = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_U;
	UpdateData(0);

}


void CPage_MeshEffect::OnBnClickedButton5() // V ▲
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CEffect_Mesh*)m_pMesh)->m_fSpeed_V += 0.01f;
	m_fV = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_V;
	UpdateData(0);

}


void CPage_MeshEffect::OnBnClickedButton7() // V ▼
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CEffect_Mesh*)m_pMesh)->m_fSpeed_V -= 0.01f;
	m_fV = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_V;

	UpdateData(0);
}


BOOL CPage_MeshEffect::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_PrototypeList.AddString(L"0. SwordHeavy03 Ground");
	m_PrototypeList.AddString(L"1. Gun Flash");
	m_PrototypeList.AddString(L"2. Gun Cap");
	m_PrototypeList.AddString(L"3. MiniGun Flash");
	m_PrototypeList.AddString(L"4. MiniGun Cap");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPage_MeshEffect::OnLbnSelchangeList2() // Prototype List
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iIndex = m_PrototypeList.GetCurSel();
	UpdateData(0);

}
