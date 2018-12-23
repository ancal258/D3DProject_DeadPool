#include "stdafx.h"
#include "..\Headers\SubtitleManager.h"

_IMPLEMENT_SINGLETON(CSubtitle_Manager)


CSubtitle_Manager::CSubtitle_Manager()
{
}

void CSubtitle_Manager::Ready_Subtitle()
{
	m_vecSubtitle.push_back(L"자막용 글씨1");
	m_vecSubtitle.push_back(L"자막용 글씨2");
	m_vecSubtitle.push_back(L"자막용 글씨3");
	m_vecSubtitle.push_back(L"자막용 글씨4");
	m_vecSubtitle.push_back(L"자막용 글씨5");
	m_vecSubtitle.push_back(L"자막용 글씨6");
	m_vecSubtitle.push_back(L"자막용 글씨7");
	m_vecSubtitle.push_back(L"자막용 글씨8");
	m_vecSubtitle.push_back(L"자막용 글씨9"); 
	m_vecSubtitle.push_back(L"자막용 글씨10");
	m_vecSubtitle.push_back(L"자막용 글씨11");
	m_vecSubtitle.push_back(L"자막용 글씨12");
	m_vecSubtitle.push_back(L"자막용 글씨13");
	m_vecSubtitle.push_back(L"자막용 글씨14");
	m_vecSubtitle.push_back(L"자막용 글씨15");
	m_vecSubtitle.push_back(L"자막용 글씨16");
	m_vecSubtitle.push_back(L"자막용 글씨17");
	m_vecSubtitle.push_back(L"자막용 글씨18");
}

const _tchar * CSubtitle_Manager::Get_Subtitle()
{
	return m_vecSubtitle[m_iCurrentIndex++];
}

void CSubtitle_Manager::Free()
{
	//for (auto& Subtitle : m_vecSubtitle)
	//	Safe_Delete_Array(Subtitle);
}
