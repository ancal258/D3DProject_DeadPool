#pragma once

#include "Defines.h"
#include "afxwin.h"
_BEGIN(Engine)
class CGameObject;
_END
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
public:
	afx_msg void OnBnClickedButton1();
	float m_fU;
	float m_fV;

	CGameObject*	m_pMesh = nullptr;
	_uint			m_iIndex = 0;

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	CListBox m_PrototypeList;
	CListBox m_ObjectList;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList2();
};
