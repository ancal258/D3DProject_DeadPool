#pragma once


// CPage_MeshEffect ��ȭ �����Դϴ�.

class CPage_MeshEffect : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_MeshEffect)

public:
	CPage_MeshEffect();
	virtual ~CPage_MeshEffect();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_MESHEFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
