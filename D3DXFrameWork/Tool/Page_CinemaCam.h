#pragma once


// CPage_CinemaCam 대화 상자입니다.

class CPage_CinemaCam : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_CinemaCam)

public:
	CPage_CinemaCam();
	virtual ~CPage_CinemaCam();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_CINEMACAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	float m_fCameraAtX;
	float m_fCameraAtY;
	float m_fCameraAtZ;
};
