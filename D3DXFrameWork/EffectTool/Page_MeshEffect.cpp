// Page_MeshEffect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Page_MeshEffect.h"
#include "afxdialogex.h"


// CPage_MeshEffect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_MeshEffect, CPropertyPage)

CPage_MeshEffect::CPage_MeshEffect()
	: CPropertyPage(IDD_PAGE_MESHEFFECT)
{

}

CPage_MeshEffect::~CPage_MeshEffect()
{
}

void CPage_MeshEffect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage_MeshEffect, CPropertyPage)
END_MESSAGE_MAP()


// CPage_MeshEffect 메시지 처리기입니다.
