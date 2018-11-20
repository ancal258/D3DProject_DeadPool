#pragma once

typedef struct tagMeshTextures
{
	LPDIRECT3DTEXTURE9			pDiffuse;
	LPDIRECT3DTEXTURE9			pNormal;
	LPDIRECT3DTEXTURE9			pSpecular;
}MESHTEXTURE;

typedef struct tagD3DXMeshContainer_Derived : public D3DXMESHCONTAINER
{
	MESHTEXTURE*	pTextures;

	// 정점정보를 변경하는것이 아니라, 제공만 해줄 메시를 생성한다.
	LPD3DXMESH		pMesh_Original;

	// 현재 메시에 영향을 미치는 뼈의 갯수.
	unsigned long	dwNumBones;

	// 모두다 동적배열로 할당.
	D3DXMATRIX**	ppCombinedMatrices;
	D3DXMATRIX*		pOffsetMatrices;
	D3DXMATRIX*		pRenderingMatrices;

}D3DXMESHCONTAINER_DERIVED;

typedef struct tagD3DXFrame_Derived : public D3DXFRAME
{
	// 현재 뼈의 TransformationMatrix * 부모의 CombinedTransformationMatrix 를 해놓은 행렬.
	D3DXMATRIX			CombinedTransformationMatrix;
}D3DXFRAME_DERIVED;


typedef struct tagCamera_Desc
{
	D3DXVECTOR3			vEye; // 카메라의 위치.
	D3DXVECTOR3			vAt; // 카메라가 바라보고 있는 점.
	D3DXVECTOR3			vAxisY; // 사용하고 있는 좌표공간에서의 업(y)벡터
}CAMERADESC;

typedef struct tagProjection_Desc
{
	float				fFovy; // 내 절두체의 y각도
	float				fAspect; // wincx / wincy
	float				fNear;
	float				fFar;
}PROJDESC;

typedef struct tagVertex_Normal_Texture
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vNormal;
	D3DXVECTOR2			vTexUV;
}VTXNORTEX;

typedef struct tagVertex_Texture
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR2			vTexUV;
}VTXTEX;

typedef struct tagVertex_Color
{
	D3DXVECTOR3			vPosition;
	DWORD				dwColor;
}VTXCOL;

typedef struct tagPolygon_16
{
	unsigned short		_1, _2, _3;
}POLYGON16;

typedef struct tagPolygon_32
{
	unsigned long		_1, _2, _3;
}POLYGON32;

// Collider

typedef struct tagAABBCollider {
	D3DXVECTOR3 vCenter;
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
}COLLIDER;

typedef struct tagOBBCollider {
	D3DXVECTOR3 vCenter; // 상자 중앙의 좌표
	D3DXVECTOR3 vAxisDir[3]; //상자에 평행한 세 축의 단위벡터
	float  fAxisLen[3]; // 상자의 평행한 세 축의 길이 
}OBB_COLLIDER;


typedef struct tagStaticObjectInfo {
	D3DXVECTOR3 vRight;
	D3DXVECTOR3 vUp;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vPos;
	wchar_t szPrototype_Tag[128];			// Component_Tag
}STATIC_OBJECT_INFO;					// Collider -> MinMax Setup Setting