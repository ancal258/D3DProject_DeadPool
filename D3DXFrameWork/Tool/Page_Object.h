#pragma once
#include "afxwin.h"
#include "Engine_Defines.h"
#include "Defines.h"

// CPage_Object 대화 상자입니다.

_BEGIN(Engine)
class CGameObject;
_END
class CPage_Object : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_Object)

public:
	CPage_Object();
	virtual ~CPage_Object();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_OBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


private:
	enum MESH_SELECT {
		MESH_ANOTHER_SOFA, MESH_ANOTHER_BIGSOFA, MESH_BICYCLE, MESH_BOOKCASE, MESH_DOGBED, MESH_DP_BED_01, MESH_DP_BED_02, MESH_DP_SOFA, MESH_KITCHENTABLE,
		MESH_NEONSIGN_01, MESH_NEONSIGN_02, MESH_NEONSIGN_03, MESH_NEONSIGN_04, MESH_POSTER_01, MESH_POSTER_02, MESH_RUBBERDUCKIE, MESH_RUG, MESH_TV, MESH_TV_REMOTE,
		MESH_WALL_01, MESH_WALL_01B, MESH_WALL_CORNER, MESH_WALL_SIDEBTM, MESH_WALL_SIDETOP, MESH_WEIGHT_BARBEL, MESH_WEIGHT_BENCH, MESH_WDN_DOOR, MESH_WDN_FRAME,// APT
																																								  // MAP
																																								  MESH_TEST, MESH_ROCK_BRIDGE, MESH_ROCK_WALL,  // STATIC
																																								  MESH_END
	};
public:
	void Set_PickingPoint(_vec3 vPickingPoint);
private:
	void Create_Object_APT(_uint iIndex);
	void Create_Object_MAP(_uint iIndex);
	void Create_Object_STATIC(_uint iIndex);
private:
	_uint iMeshCntArray[MESH_END] = { 0 };
	_uint m_iListIdx = 0;
public:
	CComboBox m_APTCombo;
	CComboBox m_MAPCombo;
	CComboBox m_STATICCombo;
	_uint	  m_iComboNum = 0;
	vector<CGameObject*> vGameObject;

	float m_fMeshSize;
	afx_msg void OnBnClickedButton9();
	float m_fMeshPosX;
	float m_fMeshPosY;
	virtual BOOL OnInitDialog();
	CListBox m_MeshListBox;
	afx_msg void OnLbnSelchangeList1();
	float m_fTransPow;
	float m_fRotatePow;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton7();
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton12();

	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL m_isBlocked;
	afx_msg void OnBnClickedCheck1();
	float m_fMeshPosZ;
	BOOL m_isTransformMode;
	afx_msg void OnBnClickedCheck2();
};
