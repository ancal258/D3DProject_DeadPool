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
		MESH_ANOTHER_SOFA, MESH_ANOTHER_BIGSOFA, MESH_BICYCLE, MESH_BOOKCASE, MESH_DOGBED, MESH_DP_BED_01, MESH_DP_BED_02, MESH_DP_SOFA, MESH_KITCHENTABLE,MESH_LAMPHOUSE,
		MESH_NEONSIGN_01, MESH_NEONSIGN_02, MESH_NEONSIGN_03, MESH_NEONSIGN_04, MESH_POSTER_01, MESH_POSTER_02, MESH_RUBBERDUCKIE, MESH_RUG, MESH_TV, MESH_TV_REMOTE,
		MESH_WALL_01, MESH_WALL_01B, MESH_WALL_CORNER, MESH_WALL_SIDEBTM, MESH_WALL_SIDETOP, MESH_WEIGHT_BARBEL, MESH_WEIGHT_BENCH, MESH_WDN_DOOR, MESH_WDN_FRAME, MESH_CELLING,// APT

		//MESH_CELLING_CENTER, MESH_CELLING_HATCH, MESH_CELLING_OUTER, MESH_FLOOR, MESH_FLOOR_CENTER, MESH_FLOOR_OUTER, MESH_FLOOR_STRAIGHT, MESH_METAL_PIPES_BENT, MESH_METAL_PIPES_LONG,
		//MESH_TUNNEL_METALTRIM, MESH_TUNNEL_STRAIGHT, MESH_TUNNEL_STRAIGHT_TOP, MESH_TUNNEL_TRIM, MESH_TUNNEL_TRIM_TOP, MESH_WALL_BASE1, MESH_WALL_BASE2, MESH_WALL_BASE3, MESH_WALL_BASE_HOLE, MESH_WALL_BASE_TRIM,
		MESH_ACCESSROOF,MESH_ACCESSROOF_DOOR, MESH_ACCESSROOF02, MESH_ACCESSROOF03,ACVENT_01, MESH_ACVENT_02, MESH_BACKGROUNDBUILDING, MESH_BACKGROUNDBUILDING_DESTROY, MESH_BEERKEG, MESH_CABINET, MESH_CIRCUIT, MESH_EMERGENCYLIGHT, MESH_FLOWERBED01, MESH_FLOWERBED02,
		MESH_FUELTANK, MESH_GENERICBOX,	MESH_GWN_SIGN, MESH_GWN_WALL01, MESH_GWN_WALL02, MESH_GWN_WALL03, MESH_HALLYWAYCOVER ,MESH_INDUSTRIALBOX, MESH_INDUSTRIALCART, MESH_INDUSTRIALGENERATOR, MESH_LADDER, MESH_LAMPSTAND ,MESH_LIGHT_BASE_01, 
		MESH_LIGHT_BASE_02, MESH_MAINBUILDING_01, MESH_MAINBUILDING_02, MESH_OUTDOORPIPE, MESH_PANNELWOOD , MESH_RADIOTOWER01, MESH_ROOFTOPAC, MESH_STRUCTURE01, MESH_TOOLBOX, MESH_TRANSFORMER01, 
		MESH_TRANSFORMER02, MESH_TRASHBAGSPILLED, MESH_TRISENTINEL, MESH_VENTDECO ,MESH_WALL_RAIL01, MESH_WALL_RAIL02, MESH_WALL_LIGHT , MESH_WATERHEATER01, MESH_WOODENBOX,
		// MAP

		MESH_ROCK_BRIDGE, MESH_ROCK_WALL,  // STATIC
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
	BOOL m_isLock;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedButton2();
	BOOL m_isOffCulling;
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	BOOL m_isExplosion;
};
