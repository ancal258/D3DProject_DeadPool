#pragma once


// CPage_Effect ��ȭ �����Դϴ�.

class CPage_Effect : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Effect)

public:
	CPage_Effect();
	virtual ~CPage_Effect();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
