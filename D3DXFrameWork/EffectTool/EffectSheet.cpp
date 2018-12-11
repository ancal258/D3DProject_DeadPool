// EffectSheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectSheet.h"


// CEffectSheet

IMPLEMENT_DYNAMIC(CEffectSheet, CPropertySheet)

CEffectSheet::CEffectSheet()
{
	AddPage(&m_Page_Effect);
	AddPage(&m_Page_MeshEffect);
}

CEffectSheet::CEffectSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CEffectSheet::CEffectSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CEffectSheet::~CEffectSheet()
{
	m_Page_Effect.DestroyWindow();
	m_Page_MeshEffect.DestroyWindow();
}


BEGIN_MESSAGE_MAP(CEffectSheet, CPropertySheet)
END_MESSAGE_MAP()


// CEffectSheet 메시지 처리기입니다.
