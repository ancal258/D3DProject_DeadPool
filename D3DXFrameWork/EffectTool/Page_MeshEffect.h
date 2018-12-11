#pragma once


// CPage_MeshEffect 대화 상자입니다.

class CPage_MeshEffect : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_MeshEffect)

public:
	CPage_MeshEffect();
	virtual ~CPage_MeshEffect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_MESHEFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
