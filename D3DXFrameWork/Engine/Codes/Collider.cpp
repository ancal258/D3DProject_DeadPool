#include "..\Headers\Collider.h"
#include "Shader.h"


CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pShader(rhs.m_pShader)
	, m_isColl(rhs.m_isColl)
{
	if (FAILED(rhs.m_pMesh->CloneMeshFVF(rhs.m_pMesh->GetOptions(), rhs.m_pMesh->GetFVF(), rhs.Get_Graphic_Device(), &m_pMesh)))
		return;

	m_pShader->AddRef();
	m_pAdjacency->AddRef();
}

HRESULT CCollider::Ready_Collider(TYPE eType)
{
	HRESULT		hr = 0;

	switch (eType)
	{
	case TYPE_BOUNDINGBOX:
		hr = Create_BoundingBox();
		break;
	case TYPE_SPHERE:
		hr = Create_Sphere();
		break;
	}

	m_pShader = CShader::Create(Get_Graphic_Device(), L"../Bin/ShaderFiles/Shader_Collider.fx");
	if (nullptr == m_pShader)
		return E_FAIL;

	if (FAILED(hr))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCollider::SetUp_Collider(const _matrix* pWorldMatrixPointer, const _matrix * pLocalMatrix)
{
	void*		pVertices = nullptr;

	_ulong dwNumVertices = m_pMesh->GetNumVertices();

	_ushort		wOffset = 0;

	D3DVERTEXELEMENT9		Element[MAX_FVF_DECL_SIZE];
	if (FAILED(m_pMesh->GetDeclaration(Element)))
		return E_FAIL;

	for (size_t i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		if (D3DDECLUSAGE_POSITION == Element[i].Usage)
		{
			wOffset = Element[i].Offset;
			break;
		}
	}

	_uint iVertexSize = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	m_pMesh->LockVertexBuffer(0, &pVertices);

	for (size_t i = 0; i < dwNumVertices; i++)
	{
		_vec3*	pPosition = (_vec3*)((_byte*)pVertices + wOffset + i * iVertexSize);

		D3DXVec3TransformCoord(pPosition, pPosition, pLocalMatrix);
	}

	m_pMesh->UnlockVertexBuffer();


	if (FAILED(Compute_MinMax()))
		return E_FAIL;

	if (FAILED(SetUp_OBB()))
		return E_FAIL;

	m_pWorldMatrix = pWorldMatrixPointer;

	return NOERROR;
}

HRESULT CCollider::SetUp_Collider(const _vec3* pMin, const _vec3* pMax, const _matrix* pWorldMatrixPointer, const _vec3* pTransMore)
{
	_matrix			matScale, matTrans, matTransMore;
	if (nullptr == pTransMore)
		D3DXMatrixIdentity(&matTransMore);
	else
		D3DXMatrixTranslation(&matTransMore, pTransMore->x, pTransMore->y, pTransMore->z);

	D3DXMatrixScaling(&matScale, pMax->x - pMin->x, pMax->y - pMin->y, pMax->z - pMin->z);
	D3DXMatrixTranslation(&matTrans, 0.0f, (pMax->y - pMin->y) * 0.5f, 0.0f);

	matTrans.m[3][0] += matTransMore.m[3][0];
	matTrans.m[3][1] += matTransMore.m[3][1];
	matTrans.m[3][2] += matTransMore.m[3][2];
	// 추가 이동

	_matrix			matLocal = matScale * matTrans;

	void*		pVertices = nullptr;

	_ulong dwNumVertices = m_pMesh->GetNumVertices();

	_ushort		wOffset = 0;

	D3DVERTEXELEMENT9		Element[MAX_FVF_DECL_SIZE];
	if (FAILED(m_pMesh->GetDeclaration(Element)))
		return E_FAIL;

	for (size_t i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		if (D3DDECLUSAGE_POSITION == Element[i].Usage)
		{
			wOffset = Element[i].Offset;
			break;
		}
	}

	_uint iVertexSize = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	m_pMesh->LockVertexBuffer(0, &pVertices);

	for (size_t i = 0; i < dwNumVertices; i++)
	{
		_vec3*	pPosition = (_vec3*)((_byte*)pVertices + wOffset + i * iVertexSize);

		D3DXVec3TransformCoord(pPosition, pPosition, &matLocal);
	}

	m_pMesh->UnlockVertexBuffer();

	if (FAILED(Compute_MinMax()))
		return E_FAIL;

	if (FAILED(SetUp_OBB()))
		return E_FAIL;

	m_pWorldMatrix = pWorldMatrixPointer;

	return NOERROR;
}

HRESULT CCollider::SetUp_Collider(const _matrix* pWorldMatrixPointer, const _vec3 * pScale, const _vec3 * pAngle, const _vec3 * pPosition)
{
	_matrix			matScale, matRotX, matRotY, matRotZ, matTrans;
	D3DXMatrixScaling(&matScale, pScale->x, pScale->y, pScale->z);
	D3DXMatrixRotationX(&matRotX, pAngle->x);
	D3DXMatrixRotationY(&matRotY, pAngle->y);
	D3DXMatrixRotationZ(&matRotZ, pAngle->z);
	D3DXMatrixTranslation(&matTrans, pPosition->x, pPosition->y, pPosition->z);

	_matrix			matLocal = matScale * matRotX * matRotY * matRotZ * matTrans;

	void*		pVertices = nullptr;

	_ulong dwNumVertices = m_pMesh->GetNumVertices();

	_ushort		wOffset = 0;

	D3DVERTEXELEMENT9		Element[MAX_FVF_DECL_SIZE];
	if (FAILED(m_pMesh->GetDeclaration(Element)))
		return E_FAIL;

	for (size_t i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		if (D3DDECLUSAGE_POSITION == Element[i].Usage)
		{
			wOffset = Element[i].Offset;
			break;
		}
	}

	_uint iVertexSize = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	m_pMesh->LockVertexBuffer(0, &pVertices);

	for (size_t i = 0; i < dwNumVertices; i++)
	{
		_vec3*	pPosition = (_vec3*)((_byte*)pVertices + wOffset + i * iVertexSize);

		D3DXVec3TransformCoord(pPosition, pPosition, &matLocal);
	}

	m_pMesh->UnlockVertexBuffer();

	if (FAILED(Compute_MinMax()))
		return E_FAIL;

	if (FAILED(SetUp_OBB()))
		return E_FAIL;

	if (FAILED(SetUp_SPEHRE()))
		return E_FAIL;

	m_pWorldMatrix = pWorldMatrixPointer;

	return NOERROR;
}

_bool CCollider::Collision_AABB(const CCollider * pTargetCollider)
{
	// 로컬영역안에 정의되어있는 민과 맥스이기때문에 월드변환 후 충돌을 비교하자.
	_vec3		vSourMin, vDestMin;
	_vec3		vSourMax, vDestMax;

	D3DXVec3TransformCoord(&vSourMin, &m_vMin, m_pWorldMatrix);
	D3DXVec3TransformCoord(&vSourMax, &m_vMax, m_pWorldMatrix);

	D3DXVec3TransformCoord(&vDestMin, pTargetCollider->Get_Min(), pTargetCollider->Get_WorldMatrixPointer());
	D3DXVec3TransformCoord(&vDestMax, pTargetCollider->Get_Max(), pTargetCollider->Get_WorldMatrixPointer());

	// x축선상에서 (너비상으로 겹치지 않았냐?!)
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return m_isColl = false;

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return m_isColl = false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return m_isColl = false;

	return m_isColl = true;
}

_bool CCollider::Collision_OBB(const CCollider * pTargetCollider)
{
	OBB		tOBB[2];

	// 로컬영역에서의 정보.
	tOBB[0] = m_tOBB;
	tOBB[1] = *pTargetCollider->Get_OBBDesc();

	D3DXVec3TransformCoord(&tOBB[0].vCenter, &tOBB[0].vCenter, m_pWorldMatrix);
	D3DXVec3TransformCoord(&tOBB[1].vCenter, &tOBB[1].vCenter, pTargetCollider->Get_WorldMatrixPointer());

	for (size_t i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(&tOBB[0].vAlignAxis[i], &tOBB[0].vAlignAxis[i], m_pWorldMatrix);
		D3DXVec3Normalize(&tOBB[0].vAlignAxis[i], &tOBB[0].vAlignAxis[i]);

		D3DXVec3TransformNormal(&tOBB[1].vAlignAxis[i], &tOBB[1].vAlignAxis[i], pTargetCollider->Get_WorldMatrixPointer());
		D3DXVec3Normalize(&tOBB[1].vAlignAxis[i], &tOBB[1].vAlignAxis[i]);


		D3DXVec3TransformNormal(&tOBB[0].vProjDir[i], &tOBB[0].vProjDir[i], m_pWorldMatrix);
		D3DXVec3TransformNormal(&tOBB[1].vProjDir[i], &tOBB[1].vProjDir[i], pTargetCollider->Get_WorldMatrixPointer());
	}

	_float		fDistance[3] = { 0.f };

	_vec3		vCenterDir = tOBB[1].vCenter - tOBB[0].vCenter;

	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 3; ++j)

		{
			fDistance[0] = fabs(D3DXVec3Dot(&vCenterDir, &tOBB[i].vAlignAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&tOBB[0].vProjDir[0], &tOBB[i].vAlignAxis[j]))
				+ fabs(D3DXVec3Dot(&tOBB[0].vProjDir[1], &tOBB[i].vAlignAxis[j]))
				+ fabs(D3DXVec3Dot(&tOBB[0].vProjDir[2], &tOBB[i].vAlignAxis[j]));

			fDistance[2] = fabs(D3DXVec3Dot(&tOBB[1].vProjDir[0], &tOBB[i].vAlignAxis[j]))
				+ fabs(D3DXVec3Dot(&tOBB[1].vProjDir[1], &tOBB[i].vAlignAxis[j]))
				+ fabs(D3DXVec3Dot(&tOBB[1].vProjDir[2], &tOBB[i].vAlignAxis[j]));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return m_isColl = false;

		}
	}

	return m_isColl = true;
}

_bool CCollider::Collision_Sphere(const CCollider * pTargetCollider)
{
	_vec3 vSour;
	_vec3 vDest;
	D3DXVec3TransformCoord(&vSour, &m_tSphere.vCenter, m_pWorldMatrix);
	D3DXVec3TransformCoord(&vDest, &pTargetCollider->m_tSphere.vCenter, pTargetCollider->m_pWorldMatrix);
	_float fDistance = D3DXVec3Length(&(vDest - vSour));

	_vec3 vSour_Min, vSour_Max;
	_vec3 vDest_Min, vDest_Max;
	D3DXVec3TransformCoord(&vSour_Min, &m_tSphere.vMin, m_pWorldMatrix);
	D3DXVec3TransformCoord(&vSour_Max, &m_tSphere.vMax, m_pWorldMatrix);
	
	D3DXVec3TransformCoord(&vDest_Min, &pTargetCollider->m_tSphere.vMin, pTargetCollider->m_pWorldMatrix);
	D3DXVec3TransformCoord(&vDest_Max, &pTargetCollider->m_tSphere.vMax, pTargetCollider->m_pWorldMatrix);

	_float vSourRadius = D3DXVec3Length(&(vSour_Min - vSour_Max)) * 0.5f;
	_float vDestRadius = D3DXVec3Length(&(vDest_Min - vDest_Max)) * 0.5f;

	if ((vSourRadius + vDestRadius) > fDistance)
	{
		return true;
	}
	return false;
}

void CCollider::Render_Collider()
{
#ifdef _DEBUG
	if (nullptr == m_pMesh)
		return;

	if (nullptr == m_pShader)
		return;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	LPDIRECT3DDEVICE9	pGraphic_Device = Get_Graphic_Device();
	if (nullptr == pGraphic_Device)
		return;

	pGraphic_Device->AddRef();

	_matrix			matView, matProj;

	pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", m_pWorldMatrix);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetBool("g_isColl", m_isColl);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pMesh->DrawSubset(0);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pGraphic_Device);
#endif
}

HRESULT CCollider::Create_BoundingBox()
{
	if (FAILED(D3DXCreateBox(Get_Graphic_Device(), 1.f, 1.f, 1.f, &m_pMesh, &m_pAdjacency)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCollider::Create_Sphere()
{
	if (FAILED(D3DXCreateSphere(Get_Graphic_Device(), 0.5f, 10.0f, 10.0f, &m_pMesh, &m_pAdjacency)))
		return E_FAIL;
	return NOERROR;
}

HRESULT CCollider::Compute_MinMax()
{
	void*		pVertices = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertices);

	if (FAILED(D3DXComputeBoundingBox((_vec3*)pVertices, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_pMesh->GetFVF()), &m_vMin, &m_vMax)))
		return E_FAIL;

	m_pMesh->UnlockVertexBuffer();

	return NOERROR;
}

HRESULT CCollider::SetUp_OBB()
{
	ZeroMemory(&m_tOBB, sizeof(OBB));

	m_tOBB.vPoint[0] = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	m_tOBB.vPoint[1] = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	m_tOBB.vPoint[2] = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	m_tOBB.vPoint[3] = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);

	m_tOBB.vPoint[4] = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	m_tOBB.vPoint[5] = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	m_tOBB.vPoint[6] = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	m_tOBB.vPoint[7] = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);

	m_tOBB.vCenter = (m_tOBB.vPoint[0] + m_tOBB.vPoint[6]) * 0.5f;

	m_tOBB.vAlignAxis[0] = m_tOBB.vPoint[2] - m_tOBB.vPoint[3];
	m_tOBB.vAlignAxis[1] = m_tOBB.vPoint[0] - m_tOBB.vPoint[3];
	m_tOBB.vAlignAxis[2] = m_tOBB.vPoint[7] - m_tOBB.vPoint[3];

	m_tOBB.vProjDir[0] = (m_tOBB.vPoint[5] + m_tOBB.vPoint[2]) * 0.5f - m_tOBB.vCenter;
	m_tOBB.vProjDir[1] = (m_tOBB.vPoint[5] + m_tOBB.vPoint[0]) * 0.5f - m_tOBB.vCenter;
	m_tOBB.vProjDir[2] = (m_tOBB.vPoint[5] + m_tOBB.vPoint[7]) * 0.5f - m_tOBB.vCenter;

	return NOERROR;
}

HRESULT CCollider::SetUp_SPEHRE()
{
	ZeroMemory(&m_tSphere, sizeof(SPHERE));

	m_tSphere.vCenter = m_tOBB.vCenter;
	m_tSphere.vMin = _vec3(m_vMax.x*0.5f, m_vMax.y*0.5f, m_vMin.z);
	m_tSphere.vMax = _vec3(m_vMax.x*0.5f, m_vMax.y*0.5f, m_vMax.z);
	return NOERROR;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType)
{
	CCollider*		pInstance = new CCollider(pGraphic_Device);

	if (FAILED(pInstance->Ready_Collider(eType)))
	{
		_MSG_BOX("CCollider Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CCollider::Clone_Component()
{
	return new CCollider(*this);
}

void CCollider::Free()
{
	Safe_Release(m_pAdjacency);

	Safe_Release(m_pShader);

	Safe_Release(m_pMesh);

	CComponent::Free();
}
