// Form.cpp : 구현 파일입니다.
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


// CForm 진단입니다.

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


// CForm 메시지 처리기입니다.




void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Delete(m_pSheet);
}
