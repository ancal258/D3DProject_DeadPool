// Form.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Form.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Sheet.h"
// CForm
#include "Graphic_Device.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "ToolCamera.h"
#include "ToolTerrain.h"


// Mesh

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)

	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CForm �����Դϴ�.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm �޽��� ó�����Դϴ�.




void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_pSheet = new CSheet;
	m_pSheet->Create(this, WS_CHILD | WS_VISIBLE);

	m_pSheet->MoveWindow(0, 0, g_iFormCX, g_iFormCY);

	for (size_t i = 0; i < 2; i++)
	{
		m_pSheet->SetActivePage(i);
	}
	m_pSheet->SetActivePage(0);
}

void CForm::Set_PickingPoint(_vec3 vPickingPoint)
{

	m_pSheet->Set_PickingPoint(vPickingPoint);
}

void CForm::Set_VecPointNav(_vec3* pVecPoint)
{
	m_pSheet->Set_VecPointNav(pVecPoint);
}

void CForm::Add_CamPoint(_vec3 vPoint)
{
	m_pSheet->Add_CamPoint(vPoint);
}

void CForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Safe_Delete(m_pSheet);
}
