// EffectForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectForm.h"
#include "EffectSheet.h"


// CEffectForm

IMPLEMENT_DYNCREATE(CEffectForm, CFormView)

CEffectForm::CEffectForm()
	: CFormView(IDD_EFFECTFORM)
{

}

CEffectForm::~CEffectForm()
{
}

void CEffectForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEffectForm, CFormView)
END_MESSAGE_MAP()


// CEffectForm �����Դϴ�.

#ifdef _DEBUG
void CEffectForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEffectForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEffectForm �޽��� ó�����Դϴ�.


void CEffectForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_pSheet = new CEffectSheet;
	m_pSheet->Create(this, WS_CHILD | WS_VISIBLE);

	m_pSheet->MoveWindow(0, 0, g_iFormCX, g_iFormCY);

	for (size_t i = 0; i < 2; i++)
	{
		m_pSheet->SetActivePage(i);
	}
	m_pSheet->SetActivePage(0);

}
