// Page_Object.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Page_Object.h"
#include "afxdialogex.h"

// View
#include "ToolView.h"
#include "MainFrm.h"

#include "Object_Manager.h"
#include "Component_Manager.h"

// Mesh
#include "Static_Object.h"
#include "ToolTerrain.h"
// CPage_Object 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage_Object, CPropertyPage)

CPage_Object::CPage_Object()
	: CPropertyPage(IDD_PAGE_OBJECT)
	, m_fMeshSize(0.05f)
	, m_fMeshPosX(0)
	, m_fMeshPosY(0)
	, m_fTransPow(0)
	, m_fRotatePow(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fScaleZ(0)
	, m_isBlocked(FALSE)
	, m_fMeshPosZ(0)
	, m_isTransformMode(FALSE)
{

}

CPage_Object::~CPage_Object()
{
}

void CPage_Object::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_APTCombo);
	DDX_Text(pDX, IDC_EDIT7, m_fMeshSize);
	DDX_Text(pDX, IDC_EDIT8, m_fMeshPosX);
	DDX_Text(pDX, IDC_EDIT10, m_fMeshPosY);
	DDX_Control(pDX, IDC_LIST1, m_MeshListBox);
	DDX_Text(pDX, IDC_EDIT1, m_fTransPow);
	DDX_Text(pDX, IDC_EDIT11, m_fRotatePow);
	DDX_Text(pDX, IDC_EDIT12, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT13, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT14, m_fScaleZ);
	DDX_Control(pDX, IDC_COMBO2, m_MAPCombo);
	DDX_Control(pDX, IDC_COMBO3, m_STATICCombo);
	DDX_Check(pDX, IDC_CHECK1, m_isBlocked);
	DDX_Text(pDX, IDC_EDIT19, m_fMeshPosZ);
	DDX_Check(pDX, IDC_CHECK2, m_isTransformMode);
}


BEGIN_MESSAGE_MAP(CPage_Object, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON9, &CPage_Object::OnBnClickedButton9)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPage_Object::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_Object::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CPage_Object::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON10, &CPage_Object::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CPage_Object::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON14, &CPage_Object::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CPage_Object::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON7, &CPage_Object::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON17, &CPage_Object::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CPage_Object::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON12, &CPage_Object::OnBnClickedButton12)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPage_Object::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CPage_Object::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CPage_Object::OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON8, &CPage_Object::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON16, &CPage_Object::OnBnClickedButton16)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK1, &CPage_Object::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CPage_Object::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CPage_Object 메시지 처리기입니다.


void CPage_Object::Set_PickingPoint(_vec3 vPickingPoint)
{
	m_fMeshPosX = vPickingPoint.x;
	m_fMeshPosY = vPickingPoint.y;
	m_fMeshPosZ = vPickingPoint.z;



	UpdateData(0);
}

void CPage_Object::Create_Object_APT(_uint iIndex)
{
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;
	_tchar szMeshName[128] = L"";

	switch (iIndex)
	{
	case MESH_ANOTHER_SOFA:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Another_Sofa", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Another_Sofa_%d", iMeshCntArray[MESH_ANOTHER_SOFA]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_ANOTHER_SOFA];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Another_Sofa");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_ANOTHER_BIGSOFA:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Another_BigSofa", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Another_BigSofa_%d", iMeshCntArray[MESH_ANOTHER_BIGSOFA]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_ANOTHER_BIGSOFA];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Another_BigSofa");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_BICYCLE:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Bicycle", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Bicycle_%d", iMeshCntArray[MESH_BICYCLE]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_BICYCLE];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Bicycle");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_BOOKCASE:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_BookCase", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"BookCase_%d", iMeshCntArray[MESH_BOOKCASE]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_BOOKCASE];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_BookCase");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_DOGBED:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_DogPoolBed", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"DogPoolBed_%d", iMeshCntArray[MESH_DOGBED]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_DOGBED];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_DogPoolBed");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_DP_BED_01:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_DP_Bed_01", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"DP_Bed01_%d", iMeshCntArray[MESH_DP_BED_01]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_DP_BED_01];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_DP_Bed_01");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_DP_BED_02:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_DP_Bed_02", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"DP_Bed02_%d", iMeshCntArray[MESH_DP_BED_02]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_DP_BED_02];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_DP_Bed_02");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_DP_SOFA:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_DP_Sofa", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"DP_Sofa_%d", iMeshCntArray[MESH_DP_SOFA]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_DP_SOFA];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_DP_Sofa");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_KITCHENTABLE:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_KitchenTable", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"KitchenTable_%d", iMeshCntArray[MESH_KITCHENTABLE]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_KITCHENTABLE];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_KitchenTable");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_NEONSIGN_01:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_NeonSign_01", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"NeonSign01_%d", iMeshCntArray[MESH_NEONSIGN_01]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_NEONSIGN_01];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_NeonSign_01");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_NEONSIGN_02:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_NeonSign_02", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"NeonSign02_%d", iMeshCntArray[MESH_NEONSIGN_02]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_NEONSIGN_02];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_NeonSign_02");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_NEONSIGN_03:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_NeonSign_03", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"NeonSign03_%d", iMeshCntArray[MESH_NEONSIGN_03]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_NEONSIGN_03];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_NeonSign_03");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_NEONSIGN_04:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_NeonSign_04", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"NeonSign04_%d", iMeshCntArray[MESH_NEONSIGN_04]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_NEONSIGN_04];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_NeonSign_04");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_POSTER_01:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Poster_01", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Poster01_%d", iMeshCntArray[MESH_POSTER_01]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_POSTER_01];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Poster_01");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_POSTER_02:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Poster_02", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Poster02_%d", iMeshCntArray[MESH_POSTER_02]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_POSTER_02];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Poster_02");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_RUBBERDUCKIE:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_RubberDuckie", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"RubberDuckie_%d", iMeshCntArray[MESH_RUBBERDUCKIE]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_RUBBERDUCKIE];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_RubberDuckie");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_RUG:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Rug", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Rug_%d", iMeshCntArray[MESH_RUG]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_RUG];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Rug");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;

	case MESH_TV:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_TV", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"TV_%d", iMeshCntArray[MESH_TV]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_TV];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_TV");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_TV_REMOTE:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_TV_Remote", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"TV_Remote_%d", iMeshCntArray[MESH_TV_REMOTE]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_TV_REMOTE];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_TV_Remote");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_WALL_01:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Wall_01", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Wall_01_%d", iMeshCntArray[MESH_WALL_01]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.  
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WALL_01];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Wall_01");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_WALL_01B:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Wall_01b", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Wall_01b_%d", iMeshCntArray[MESH_WALL_01B]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WALL_01B];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Wall_01b");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_WALL_CORNER:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Wall_Corner", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Wall_Corner_%d", iMeshCntArray[MESH_WALL_CORNER]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WALL_CORNER];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Wall_Corner");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_WALL_SIDEBTM:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Wall_SideBoard", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Wall_SideBTM_%d", iMeshCntArray[MESH_WALL_SIDEBTM]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WALL_SIDEBTM];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Wall_SideBoard");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_WALL_SIDETOP:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Wall_SideBoardTopSide", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Wall_SideTOP_%d", iMeshCntArray[MESH_WALL_SIDETOP]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WALL_SIDETOP];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Wall_SideBoardTopSide");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;

	case MESH_WEIGHT_BARBEL:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Weight_Barbel", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Weight_Barbel_%d", iMeshCntArray[MESH_WEIGHT_BARBEL]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WEIGHT_BARBEL];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Weight_Barbel");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_WEIGHT_BENCH:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Weight_Bench", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Weight_Bench_%d", iMeshCntArray[MESH_WEIGHT_BENCH]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WEIGHT_BENCH];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Weight_Bench");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_WDN_DOOR:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Wooden_Door", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Wooden_Door_%d", iMeshCntArray[MESH_WDN_DOOR]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WDN_DOOR];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Wooden_Door");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_WDN_FRAME:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Wooden_DoorFrame", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"DoorFrame_%d", iMeshCntArray[MESH_WDN_FRAME]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_WDN_FRAME];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Wooden_DoorFrame");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
	case MESH_CELLING:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Celling", L"Layer_Static_APT", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"Celling_%d", iMeshCntArray[MESH_CELLING]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_CELLING];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Celling");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	default:
		break;
	}

}

void CPage_Object::Create_Object_MAP(_uint iIndex)
{
}

void CPage_Object::Create_Object_STATIC(_uint iIndex)
{
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;
	_tchar szMeshName[128] = L"";


	switch (iIndex)
	{
	case MESH_ROCK_BRIDGE:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Rock_Bridge", L"Layer_Rock_Bridge", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"MESH_ROCK_BRIDGE_%d", iMeshCntArray[MESH_ROCK_BRIDGE]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_ROCK_BRIDGE];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Rock_Bridge");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	case MESH_ROCK_WALL:
		if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(L"Prototype_Rock_Wall", L"Layer_Rock_Wall", &pMesh)))
			return;
		if (nullptr != pMesh)
		{
			wsprintf(szMeshName, L"MESH_ROCK_WALL_%d", iMeshCntArray[MESH_ROCK_WALL]);

			vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
			m_MeshListBox.AddString(szMeshName);
			++iMeshCntArray[MESH_ROCK_WALL];
			((CStatic_Object*)pMesh)->Set_PrototypeTag(L"Prototype_Rock_Wall");
			((CStatic_Object*)pMesh)->Set_Position(&_vec3(0, 0, 0));
			((CStatic_Object*)pMesh)->Set_Scale(&_vec3(m_fMeshSize, m_fMeshSize, m_fMeshSize));
		}
		break;
	default:
		break;
	}
}

void CPage_Object::OnBnClickedButton9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);

	int iIndex = 0;
	if (0 == m_iComboNum)
		iIndex = m_APTCombo.GetCurSel();
	if (1 == m_iComboNum)
		iIndex = m_APTCombo.GetCount() + m_MAPCombo.GetCurSel();
	if (2 == m_iComboNum)
		iIndex = m_APTCombo.GetCount() + m_MAPCombo.GetCount() + m_STATICCombo.GetCurSel();

	if (iIndex < m_APTCombo.GetCount())
		Create_Object_APT(iIndex);
	else if (m_APTCombo.GetCount() <= iIndex && iIndex < m_APTCombo.GetCount() + m_MAPCombo.GetCount())
		Create_Object_MAP(iIndex);
	else
		Create_Object_STATIC(iIndex);


	UpdateData(0);

}


BOOL CPage_Object::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_APTCombo.AddString(L"00. Another_Sofa");
	m_APTCombo.AddString(L"01. Another_BigSofa");
	m_APTCombo.AddString(L"02. Bicycle");
	m_APTCombo.AddString(L"03. BookCase");
	m_APTCombo.AddString(L"04. DogPoolBed");
	m_APTCombo.AddString(L"05. DP_Bed01");
	m_APTCombo.AddString(L"06. DP_Bed02");
	m_APTCombo.AddString(L"07. DP_Sofa");
	m_APTCombo.AddString(L"08. KitchenTable");
	m_APTCombo.AddString(L"09. NeonSign01");
	m_APTCombo.AddString(L"10. NeonSign02");
	m_APTCombo.AddString(L"11. NeonSign03");
	m_APTCombo.AddString(L"12. NeonSign04");
	m_APTCombo.AddString(L"13. Poster01");
	m_APTCombo.AddString(L"14. Poster02");
	m_APTCombo.AddString(L"15. RubberDuckie");
	m_APTCombo.AddString(L"16. Rug");
	m_APTCombo.AddString(L"17. TV");
	m_APTCombo.AddString(L"18. TV_Remote");
	m_APTCombo.AddString(L"19. Wall_01");
	m_APTCombo.AddString(L"20. Wall_01b");
	m_APTCombo.AddString(L"21. Wall_Corner");
	m_APTCombo.AddString(L"22. Wall_SideBtm");
	m_APTCombo.AddString(L"23. Wall_SideTop");
	m_APTCombo.AddString(L"24. Weight_Barbel");
	m_APTCombo.AddString(L"25. Weight_Bench");
	m_APTCombo.AddString(L"26. Door");
	m_APTCombo.AddString(L"27. Door_Frame");
	m_APTCombo.AddString(L"28. Celling");


	m_STATICCombo.AddString(L"00.MESH_TEST");
	m_STATICCombo.AddString(L"01.MESH_ROCK_BRIDGE");
	m_STATICCombo.AddString(L"02.MESH_ROCK_WALL");

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPage_Object::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iListIdx = m_MeshListBox.GetCurSel();
}


void CPage_Object::OnBnClickedButton1() // Trans_Foward
{
	UpdateData(1);
	for (auto& pGameObject : vGameObject)
	{
		if (true == ((CStatic_Object*)pGameObject)->Get_ClickedObject())
		{
			((CStatic_Object*)(pGameObject))->Set_PlusPosition(0, m_fTransPow);
		}
	}
}


void CPage_Object::OnBnClickedButton6() // Trans_Back
{
	UpdateData(1);
	m_fTransPow *= -1;

	for (auto& pGameObject : vGameObject)
	{
		if (true == ((CStatic_Object*)pGameObject)->Get_ClickedObject())
		{
			((CStatic_Object*)(pGameObject))->Set_PlusPosition(0, m_fTransPow);
		}
	}
}


void CPage_Object::OnBnClickedButton10() // Trans_Right
{
	UpdateData(1);

	for (auto& pGameObject : vGameObject)
	{
		if (true == ((CStatic_Object*)pGameObject)->Get_ClickedObject())
		{
			((CStatic_Object*)(pGameObject))->Set_PlusPosition(2, m_fTransPow);
		}
	}
}


void CPage_Object::OnBnClickedButton11() // Trans_Left
{
	UpdateData(1);
	m_fTransPow *= -1;

	for (auto& pGameObject : vGameObject)
	{
		if (true == ((CStatic_Object*)pGameObject)->Get_ClickedObject())
		{
			((CStatic_Object*)(pGameObject))->Set_PlusPosition(2, m_fTransPow);
		}
	}
}

void CPage_Object::OnBnClickedButton18() // Trans_Y UP
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	for (auto& pGameObject : vGameObject)
	{
		if (true == ((CStatic_Object*)pGameObject)->Get_ClickedObject())
		{
			((CStatic_Object*)(pGameObject))->Set_PlusPosition(1, m_fTransPow);
		}
	}
}


void CPage_Object::OnBnClickedButton12() // Trans_Y DOWN
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	m_fTransPow *= -1;
	for (auto& pGameObject : vGameObject)
	{
		if (true == ((CStatic_Object*)pGameObject)->Get_ClickedObject())
		{
			((CStatic_Object*)(pGameObject))->Set_PlusPosition(1, m_fTransPow);
		}
	}
}


void CPage_Object::OnBnClickedButton14() // Rotate_Right
{
	UpdateData(1);
	_float fRadian = D3DXToRadian(m_fRotatePow);
	for (auto& pGameObject : vGameObject)
	{
		if (true == ((CStatic_Object*)pGameObject)->Get_ClickedObject())
		{
			((CStatic_Object*)(pGameObject))->RotationY(fRadian);
		}
	}
}


void CPage_Object::OnBnClickedButton15() // Rotate_Left
{
	UpdateData(1);
	_float fRadian = -D3DXToRadian(m_fRotatePow);
	for (auto& pGameObject : vGameObject)
	{
		if (true == ((CStatic_Object*)pGameObject)->Get_ClickedObject())
		{
			((CStatic_Object*)(pGameObject))->RotationY(fRadian);
		}
	}
}


void CPage_Object::OnBnClickedButton7() // Delete
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MeshListBox.DeleteString(m_iListIdx);
	vGameObject[m_iListIdx]->Set_Lived(false);
	vGameObject.erase(vGameObject.begin() + m_iListIdx);
}


void CPage_Object::OnBnClickedButton17() // Scale
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	((CStatic_Object*)(vGameObject[m_iListIdx]))->Scaling(m_fScaleX, m_fScaleY, m_fScaleZ);
}



void CPage_Object::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iComboNum = 0;
}


void CPage_Object::OnCbnSelchangeCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iComboNum = 1;
}


void CPage_Object::OnCbnSelchangeCombo3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iComboNum = 2;
}

void CPage_Object::OnBnClickedButton8() // Save
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		FileDlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("StaticObjectInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		for (auto& pGameObject : vGameObject)
		{
			WriteFile(hFile, &((CStatic_Object*)pGameObject)->Get_ObjectInfo(), sizeof(STATIC_OBJECT_INFO), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}

	return;
}


void CPage_Object::OnBnClickedButton16() // Load
{
	CMainFrame*		pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGameObject*	pMesh = nullptr;
	_tchar szMeshName[128] = L"";

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		FileDlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TileInfo Files (*.dat)|*.dat|"), this);

	if (IDOK == FileDlg.DoModal())
	{
		HANDLE			hFile = 0;
		_ulong			dwByte = 0;

		hFile = CreateFile(FileDlg.GetPathName(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;
		while (true)
		{
			STATIC_OBJECT_INFO		ObjectInfo;

			ReadFile(hFile, &ObjectInfo, sizeof(STATIC_OBJECT_INFO), &dwByte, nullptr);

			_tchar* pPrototype_Tag = new _tchar[MAX_PATH];
			lstrcpy(pPrototype_Tag, ObjectInfo.szPrototype_Tag); 
			if (0 == dwByte)
				break;

			if (FAILED(pMainFrame->m_pToolView->Ready_Layer_Object(pPrototype_Tag, L"Layer_Load", &pMesh)))
				return;
			if (nullptr != pMesh)
			{
				wsprintf(szMeshName, L"Load_%d", iMeshCntArray[MESH_END]);

				vGameObject.push_back(pMesh); // 오브젝트 메쉬들 추가될때마다 컨테이너에 저장, 파일 저장 할 때 순회하면서 데이터 저장.
				m_MeshListBox.AddString(szMeshName);
				++iMeshCntArray[MESH_END];
				((CStatic_Object*)pMesh)->Set_PrototypeTag(pPrototype_Tag);
				((CStatic_Object*)pMesh)->Set_StateInfo(&ObjectInfo.vRight, &ObjectInfo.vUp, &ObjectInfo.vLook, &ObjectInfo.vPos);
			}
		}

		CloseHandle(hFile);
	}
	return;
}


void CPage_Object::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CPropertyPage::OnLButtonDown(nFlags, point);
}


void CPage_Object::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	const CGameObject* pTerrain = CObject_Manager::GetInstance()->Get_ObjectPointer(0, L"Layer_Terrain", 0);
	if (true == m_isBlocked)
		((CToolTerrain*)pTerrain)->Set_MouseState(STATE_END);
	else
		((CToolTerrain*)pTerrain)->Set_MouseState(STATE_ADD_OBJECT);
}


void CPage_Object::OnBnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	for (auto& pGameObject : vGameObject)
	{
		((CStatic_Object*)pGameObject)->Set_TransformMode(m_isTransformMode);
	}
}
