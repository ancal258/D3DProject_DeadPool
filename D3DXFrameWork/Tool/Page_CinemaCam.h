#pragma once


// CPage_CinemaCam ��ȭ �����Դϴ�.

class CPage_CinemaCam : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_CinemaCam)

public:
	CPage_CinemaCam();
	virtual ~CPage_CinemaCam();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_CINEMACAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	float m_fCameraAtX;
	float m_fCameraAtY;
	float m_fCameraAtZ;
};
