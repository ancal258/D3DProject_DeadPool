#pragma once


// CPage_ParticleEffect 대화 상자입니다.

class CPage_ParticleEffect : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_ParticleEffect)

public:
	CPage_ParticleEffect();
	virtual ~CPage_ParticleEffect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_PARTICLEEFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
