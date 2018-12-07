#pragma once



// CForm �� ���Դϴ�.
#include "Engine_Defines.h"
#include "Defines.h"
#include "afxwin.h"
_BEGIN(Engine)
class CGameObject;
_END

class CSheet;
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	enum MESH_SELECT { MESH_TEST, MESH_END };
public:
	virtual void OnInitialUpdate();
public:
	void Set_PickingPoint(_vec3 vPickingPoint);
	void Set_VecPointNav(_vec3* pVecPoint);
	void Add_CamPoint(_vec3 vPoint);
private:
	CSheet*			m_pSheet = nullptr;

public:
	afx_msg void OnDestroy();
};


