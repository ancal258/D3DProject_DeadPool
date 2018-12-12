// Page_ParticleEffect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Page_ParticleEffect.h"
#include "afxdialogex.h"


// CPage_ParticleEffect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_ParticleEffect, CPropertyPage)

CPage_ParticleEffect::CPage_ParticleEffect()
	: CPropertyPage(IDD_PAGE_PARTICLEEFFECT)
{

}

CPage_ParticleEffect::~CPage_ParticleEffect()
{
}

void CPage_ParticleEffect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage_ParticleEffect, CPropertyPage)
END_MESSAGE_MAP()


// CPage_ParticleEffect 메시지 처리기입니다.
