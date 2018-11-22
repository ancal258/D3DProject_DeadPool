#pragma once


// CPage_CinemaCam 대화 상자입니다.
#include "Engine_Defines.h"
#include "Defines.h"
#include "afxwin.h"
_BEGIN(Engine)
class CGameObject;
_END
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

public:
	void Add_CamPoint(_vec3 vPoint);

private:
	float m_fCameraAtX;
	float m_fCameraAtY;
	float m_fCameraAtZ;
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton17();
private:
	//각 객체
	vector<CGameObject*>		m_vecCamPoints;
	vector<CGameObject*>		m_vecEventCube;
	//Save/Load
	vector<vector<CGameObject*>> m_vvSaveWorks;
	vector<CGameObject*>		m_vecLookAt;
	vector<_float>				m_vTimes;
	vector<CString>				m_vecEventTag;
	LPD3DXMESH					m_pMesh = nullptr;
	LPD3DXBUFFER				m_pAdjacency = nullptr;
public:
	float m_fTime;
	CListBox m_CamList;
	CString m_szCamName;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnSetActive();
	CListBox m_PointList;
	CListBox m_EventList;
	CString m_szEventTag;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
};
