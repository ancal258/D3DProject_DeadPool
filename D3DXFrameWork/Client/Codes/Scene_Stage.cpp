#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Component_Manager.h"



_USING(Client)

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{

}

HRESULT CScene_Stage::Ready_Scene()
{


	return NOERROR;
}

_int CScene_Stage::Update_Scene(const _float & fTimeDelta)
{

	return CScene::Update_Scene(fTimeDelta);
}

_int CScene_Stage::LastUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LastUpdate_Scene(fTimeDelta);
}

void CScene_Stage::Render_Scene()
{

}


CScene_Stage * CScene_Stage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Stage*		pInstance = new CScene_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSG_BOX("CScene_Stage Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CScene_Stage::Free()
{
	CScene::Free();
}
