#pragma once


// CPage_Effect 대화 상자입니다.

class CPage_Effect : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Effect)

public:
	CPage_Effect();
	virtual ~CPage_Effect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
