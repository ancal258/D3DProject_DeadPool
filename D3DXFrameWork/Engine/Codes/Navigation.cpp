#include "..\Headers\Navigation.h"
#include "Cell.h"
#include "Transform.h"
CNavigation::CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_dwCurrentIdx(rhs.m_dwCurrentIdx)
	, m_dwNumCell(rhs.m_dwNumCell)
	, m_vecCell(rhs.m_vecCell)
	, m_pLine(rhs.m_pLine)
{
	m_pLine->AddRef();
	for (auto& pCell : m_vecCell)
		pCell->AddRef();
}

HRESULT CNavigation::Ready_Navigation(const _tchar * pFilePath)
{
	if (FAILED(D3DXCreateLine(Get_Graphic_Device(), &m_pLine)))
		return E_FAIL;


	HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	_ulong		dwByte = 0;

	ReadFile(hFile, &m_dwNumCell, sizeof(_ulong), &dwByte, nullptr);

	m_vecCell.reserve(m_dwNumCell);

	while (true)
	{
		_vec3		vPoint[3];

		ReadFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(Get_Graphic_Device(), vPoint, m_vecCell.size());
		if (nullptr == pCell)
			break;

		m_vecCell.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CNavigation::SetUp_Navigation(const _uint & iNaviIndex)
{
	m_dwCurrentIdx = iNaviIndex;
	return NOERROR;
}

_bool CNavigation::Move_OnNavigation(CTransform * pTransformCom, const _float & fSpeedPerSec, const _float & fTimeDelta)
{
	_vec3		vPosition = *pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);

	_vec3		vLook;
	D3DXVec3Normalize(&vLook, pTransformCom->Get_StateInfo(CTransform::STATE_LOOK));

	vPosition += vLook * fSpeedPerSec * fTimeDelta;

	_ulong		dwMoveIndex = 0;

	if (true == m_vecCell[m_dwCurrentIdx]->Is_Move(&vPosition, &dwMoveIndex))
	{
		pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);
		m_dwCurrentIdx = dwMoveIndex;
	}
	else
	{

	}




	return _bool();



	//_vec3		vPosition = *pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	//_vec3		vLook;
	//D3DXVec3Normalize(&vLook, pTransformCom->Get_StateInfo(CTransform::STATE_LOOK));


	//vPosition += vLook * fSpeedPerSec * fTimeDelta;
	//_ulong dwMoveIndex = 0;
	//_ulong dwSlideIndex = 0;

	//if (true == m_vecCell[m_dwCurrentIdx]->Is_Move(&vPosition, &dwMoveIndex,&dwSlideIndex))
	//{
	//	pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);
	//	m_dwCurrentIdx = dwMoveIndex; // 현재 인덱스
	//}
	//else // 슬라이딩
	//{
	//	int a = 0;
	//	//vPosition -= vLook * fSpeedPerSec * fTimeDelta;
	//	//_vec2	vSlide = m_vecCell[m_dwCurrentIdx]->Sliding_Move(&vLook, dwSlideIndex);
	//	//_vec3   vSlide_v3 = _vec3(vSlide.x, vPosition.y, vSlide.y); // slide y == z
	//	//vPosition += vSlide_v3 * fSpeedPerSec * fTimeDelta;
	//	////vPosition += -vLook * 0.1f;
	//	//if (true == m_vecCell[m_dwCurrentIdx]->Is_Sliding(&vPosition, &dwMoveIndex))
	//	//{
	//	//	pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);
	//	//	m_dwCurrentIdx = dwMoveIndex; // 현재 인덱스.
	//	//}
	//}

	//return _bool();
}

HRESULT CNavigation::Add_Cell(_vec3* vPosArray)
{
	CCell*		pCell = CCell::Create(Get_Graphic_Device(), vPosArray, m_vecCell.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_vecPoint.push_back(vPosArray[0]);
	m_vecPoint.push_back(vPosArray[1]);
	m_vecPoint.push_back(vPosArray[2]);
	m_vecCell.push_back(pCell);

	//if (FAILED(SetUp_Neighbor()))
	//	return E_FAIL;

	return NOERROR;
}

vector<_vec3>* CNavigation::Get_vecPoint()
{
	return &m_vecPoint;
}

void CNavigation::Render_Navigation()
{
	for (auto& pCell : m_vecCell)
	{
		if (nullptr != pCell)
			pCell->Render_Cell(m_pLine);
	}
}

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pCellSour : m_vecCell)
	{
		for (auto& pCellDest : m_vecCell)
		{
			if (pCellSour == pCellDest)
				continue;

			if (true == pCellDest->Compare_Point(pCellSour->Get_Point(CCell::POINT_A), pCellSour->Get_Point(CCell::POINT_B)))
				pCellSour->Set_Neighbor(CCell::NEIGHBOR_AB, pCellDest);

			if (true == pCellDest->Compare_Point(pCellSour->Get_Point(CCell::POINT_B), pCellSour->Get_Point(CCell::POINT_C)))
				pCellSour->Set_Neighbor(CCell::NEIGHBOR_BC, pCellDest);

			if (true == pCellDest->Compare_Point(pCellSour->Get_Point(CCell::POINT_C), pCellSour->Get_Point(CCell::POINT_A)))
				pCellSour->Set_Neighbor(CCell::NEIGHBOR_CA, pCellDest);
		}
	}

	return NOERROR;
}

CNavigation * CNavigation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath)
{
	CNavigation*		pInstance = new CNavigation(pGraphic_Device);

	if (FAILED(pInstance->Ready_Navigation(pFilePath)))
	{
		_MSG_BOX("CNavigation Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CNavigation::Clone_Component()
{
	return new CNavigation(*this);
}


void CNavigation::Free()
{
	Safe_Release(m_pLine);

	for (auto& pCell : m_vecCell)
		Safe_Release(pCell);

	m_vecCell.clear();

	CComponent::Free();
}
