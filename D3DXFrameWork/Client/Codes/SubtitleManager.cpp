#include "stdafx.h"
#include "..\Headers\SubtitleManager.h"

_IMPLEMENT_SINGLETON(CSubtitle_Manager)


CSubtitle_Manager::CSubtitle_Manager()
{
}

void CSubtitle_Manager::Ready_Subtitle()
{
	m_vecSubtitle.push_back(L"�ڸ��� �۾�1");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�2");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�3");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�4");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�5");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�6");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�7");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�8");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�9"); 
	m_vecSubtitle.push_back(L"�ڸ��� �۾�10");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�11");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�12");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�13");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�14");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�15");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�16");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�17");
	m_vecSubtitle.push_back(L"�ڸ��� �۾�18");
}

const _tchar * CSubtitle_Manager::Get_Subtitle()
{
	return m_vecSubtitle[m_iCurrentIndex++];
}

void CSubtitle_Manager::Ready_Mission()
{
	m_vecMission.push_back(L"�̼� 1");
	m_vecMission.push_back(L"�̼� 2");
	m_vecMission.push_back(L"�̼� 3");
	m_vecMission.push_back(L"�̼� 4");
	m_vecMission.push_back(L"�̼� 5");
	m_vecMission.push_back(L"�̼� 6");
}

const _tchar * CSubtitle_Manager::Get_Mission()
{
	return m_vecMission[m_iMissionIndex++];
}

void CSubtitle_Manager::Free()
{
	//for (auto& Subtitle : m_vecSubtitle)
	//	Safe_Delete_Array(Subtitle);
}
