#pragma once

// 이 컴포넌트를 추가한 객체의 월드 상태변환의 정보를 가진다. m_matWorld
// 2d : CGameObject선언, 기능구현.
// 3d : CTransform클래스로 선언과 기능을 이전시킨다.
#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END};
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public:
	const _matrix* Get_WorldMatrix() const {
		return &m_CombinedMatrix; }
	const _matrix* Get_WorldMatrix_NotRot() const {
		return &m_CombinedMatrix_NotRot;}
	const _vec3* Get_StateInfo(STATE eState) const {
		return (_vec3*)&m_matWorld.m[eState][0]; }
	_vec3 Get_Scale() const;

	void Set_StateInfo(STATE eState, const _vec3* pState) {
		memcpy(&m_matWorld.m[eState][0], pState, sizeof(_vec3)); }
	void Set_ParentMatrix(const _matrix& matParent) {
		m_matParent = matParent;}
public:
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
public:
	HRESULT Ready_Transform();
	void Update_Matrix();
public:
	void Scaling(const _vec3& vScale);
	void Scaling(const _float& fX, const _float& fY, const _float& fZ);
	void Go_Straight(const _float& fSpeedPerSec, const _float& fTimeDelta);
	void Set_AngleX(const _float& fRadian); // 특정각도로 고정시킨다.
	void Set_AngleY(const _float& fRadian); // 특정각도로 고정시킨다.
	void Set_AngleZ(const _float& fRadian); // 특정각도로 고정시킨다.
	void RotationX(const _float& fRadianPerSec, const _float& fTimeDelta); // 특정 각도만큼 추가적으로 회전한다.
	void RotationY(const _float& fRadianPerSec, const _float& fTimeDelta); // 특정 각도만큼 추가적으로 회전한다.
	void RotationZ(const _float& fRadianPerSec, const _float& fTimeDelta); // 특정 각도만큼 추가적으로 회전한다.
	void RotationAxis(const _vec3 vAxis, const _float& fRadianPerSec, const _float& fTimeDelta); // 특정 각도만큼 추가적으로 회전한다.
	void Set_PlusPosition(_uint iType, _float& fValue); 

private:
	_matrix					m_matWorld; // 자기만의 행렬.
	_matrix					m_matParent; // 부모행렬.
private:
	_matrix					m_CombinedMatrix; // 내 행렬 * 부모행렬.
	_matrix					m_CombinedMatrix_NotRot;
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};

_END