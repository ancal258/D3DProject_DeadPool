// Page_Effect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Page_Effect.h"
#include "afxdialogex.h"


// CPage_Effect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Effect, CPropertyPage)

CPage_Effect::CPage_Effect()
	: CPropertyPage(IDD_PAGE_EFFECT)
{

}

CPage_Effect::~CPage_Effect()
{
}

void CPage_Effect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage_Effect, CPropertyPage)
END_MESSAGE_MAP()


// CPage_Effect 메시지 처리기입니다.
