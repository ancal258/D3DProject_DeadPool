#pragma once


// CPage_ParticleEffect ��ȭ �����Դϴ�.

class CPage_ParticleEffect : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_ParticleEffect)

public:
	CPage_ParticleEffect();
	virtual ~CPage_ParticleEffect();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_PARTICLEEFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
