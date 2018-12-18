// Page_Effect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Page_Effect.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "EffectToolView.h"

#include "ToolEffect.h"
#include "ParentEffect.h"

// CPage_Effect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Effect, CPropertyPage)

CPage_Effect::CPage_Effect()
	: CPropertyPage(IDD_PAGE_EFFECT)
	, m_fInitX(0)
	, m_fInitY(0)
	, m_fInitZ(0)
	, m_isRandomPos(false)
	, m_isSettingPos(false)
	, m_fScale(0)
	, m_fMoveSpeed(0)
	, m_fSurviveTime(0)
	, m_isAnimationTexture(false)
	, m_isOneTexture(false)
	, m_fFrameSpeed(0)
	, m_fFrameMax(0)
	, m_fRotDegree(0)
	, m_fDirX(0)
	, m_fDirY(0)
	, m_fDirZ(0)
	, m_isContinueCreate(FALSE)
	, m_fCreateTime(0)
{

}

CPage_Effect::~CPage_Effect()
{
}

void CPage_Effect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_TextureList);
	DDX_Text(pDX, IDC_EDIT1, m_fInitX);
	DDX_Text(pDX, IDC_EDIT2, m_fInitY);
	DDX_Text(pDX, IDC_EDIT3, m_fInitZ);
	DDX_Text(pDX, IDC_EDIT4, m_fScale);
	DDX_Text(pDX, IDC_EDIT5, m_fMoveSpeed);
	DDX_Text(pDX, IDC_EDIT6, m_fSurviveTime);
	DDX_Text(pDX, IDC_EDIT9, m_fFrameSpeed);
	DDX_Text(pDX, IDC_EDIT10, m_fFrameMax);
	DDX_Text(pDX, IDC_EDIT11, m_fRotDegree);
	DDX_Text(pDX, IDC_EDIT12, m_fDirX);
	DDX_Text(pDX, IDC_EDIT13, m_fDirY);
	DDX_Text(pDX, IDC_EDIT14, m_fDirZ);
	DDX_Check(pDX, IDC_CHECK1, m_isContinueCreate);
	DDX_Text(pDX, IDC_EDIT15, m_fCreateTime);
}


BEGIN_MESSAGE_MAP(CPage_Effect, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Effect::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPage_Effect::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CPage_Effect 메시지 처리기입니다.


void CPage_Effect::OnBnClickedButton1() // 생성 버튼
{
	UpdateData(1);
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (0 == m_iSelect)
	{
		if (FAILED(pMainFrame->m_pEffectToolView->Ready_Layer_Object(L"Prototype_ExplosionParent", L"Layer_ParentEffect", &pMesh)))
			return;
		_vec3 vScale = _vec3(m_fScale, m_fScale, m_fScale);
		_vec3 vPos = _vec3(m_fInitX, m_fInitY, m_fInitZ);
		_vec3 vDir = _vec3(m_fDirX, m_fDirY, m_fDirZ);
		((CParentEffect*)pMesh)->Set_EffectInfo(m_fFrameSpeed, m_fFrameMax, m_fMoveSpeed, m_fSurviveTime, m_fRotDegree, &vScale, &vPos, &vDir,m_isSettingPos,m_isRandomPos,m_fCreateTime);
	}
}


BOOL CPage_Effect::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_isRandomPos = true;
	m_isAnimationTexture = true;
	m_isContinueCreate = TRUE;

	m_TextureList.AddString(L"1. TestExplosion");


	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPage_Effect::OnLbnSelchangeList1() // ListBox
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelect = m_TextureList.GetCurSel();
}
