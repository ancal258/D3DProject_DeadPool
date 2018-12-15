// Page_MeshEffect.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Page_MeshEffect.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "EffectToolView.h"

//GameObject
#include "Effect_Mesh.h"

// CPage_MeshEffect ��ȭ �����Դϴ�.

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
}


BEGIN_MESSAGE_MAP(CPage_MeshEffect, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_MeshEffect::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage_MeshEffect::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CPage_MeshEffect::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &CPage_MeshEffect::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CPage_MeshEffect::OnBnClickedButton7)
END_MESSAGE_MAP()


// CPage_MeshEffect �޽��� ó�����Դϴ�.


void CPage_MeshEffect::OnBnClickedButton1() // Load Mesh
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (FAILED(pMainFrame->m_pEffectToolView->Ready_Layer_Object(L"Prototype_EffectSwordHeavy03R", L"Layer_MeshEffect", &m_pMesh)))
		return;
	m_fU = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_U;
	m_fV = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_V;

	UpdateData(0);
}


void CPage_MeshEffect::OnBnClickedButton4() // U ��
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CEffect_Mesh*)m_pMesh)->m_fSpeed_U += 0.01f;
	m_fU = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_U;
	UpdateData(0);
}


void CPage_MeshEffect::OnBnClickedButton6() // U ��
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CEffect_Mesh*)m_pMesh)->m_fSpeed_U -= 0.01f;
	m_fU = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_U;
	UpdateData(0);

}


void CPage_MeshEffect::OnBnClickedButton5() // V ��
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CEffect_Mesh*)m_pMesh)->m_fSpeed_V += 0.01f;
	m_fV = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_V;
	UpdateData(0);

}


void CPage_MeshEffect::OnBnClickedButton7() // V ��
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	((CEffect_Mesh*)m_pMesh)->m_fSpeed_V -= 0.01f;
	m_fV = ((CEffect_Mesh*)m_pMesh)->m_fSpeed_V;

	UpdateData(0);
}
