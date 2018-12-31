#include "stdafx.h"
#include "..\Headers\SubtitleManager.h"

_IMPLEMENT_SINGLETON(CSubtitle_Manager)


CSubtitle_Manager::CSubtitle_Manager()
{
}

void CSubtitle_Manager::Ready_Subtitle()
{
	m_vecSubtitle.push_back(L"�Ͼ�� ����Ǯ?");
	m_vecSubtitle.push_back(L"����, ���� ���̸� ������ �Թ���?");
	m_vecSubtitle.push_back(L"�ð���.. �ð��� �����߾�...");
	m_vecSubtitle.push_back(L"���� ����, UI���� �͵� ���� ���̾�");
	m_vecSubtitle.push_back(L"�� �ּ��� ���Ѱž� �̤�");
	m_vecSubtitle.push_back(L"������ �ð�, �� �ð��� �ְ�?");
	m_vecSubtitle.push_back(L"TV ������ ����, ������ �غ��߾�.");
	m_vecSubtitle.push_back(L"���� ����.. �� ������ Ƣ�����");
	m_vecSubtitle.push_back(L"���� �ƹ�ư ����!");
	m_vecSubtitle.push_back(L"   "); 
	m_vecSubtitle.push_back(L"����... �ÿ�ȸ �߰��� ��Ʈ���⳪ �ϰ�");
	m_vecSubtitle.push_back(L"��������?");
	m_vecSubtitle.push_back(L"�ƴ� �̰� �̴�θ� �� �� �̻� ����!");
	m_vecSubtitle.push_back(L"��!!!!!!!! �� �˰ھ�!! ��ٷ��� ���� ��ĥ��");
	m_vecSubtitle.push_back(L"��ġ�� �迡 �� ���Ӵ�� �ٸ����");
	m_vecSubtitle.push_back(L"�װ� �׷��� ���� ������ ���� ���� �Ѥ�");
	m_vecSubtitle.push_back(L"������ �� �� ���� �� �ۿ� ������;");
	m_vecSubtitle.push_back(L"?? �� �� ������ �ִϸ��̼Ǹ� 1������ ��Ҵ�.");
	m_vecSubtitle.push_back(L"�� �ƹ�ư ���� ���ĳ�.");
	// ��Ű�ϰ�
	m_vecSubtitle.push_back(L"�����ּ���!!!");
	m_vecSubtitle.push_back(L"�� �� ���� ���� �ִ� �ž�");
	m_vecSubtitle.push_back(L"�𸣰ھ��... ���� �ߴ�...");
	m_vecSubtitle.push_back(L"���ǿ� �ɸ���� �ȵǰڴ�. �̾�");
	m_vecSubtitle.push_back(L"... ... ����?");
}

const _tchar * CSubtitle_Manager::Get_Subtitle()
{
	return m_vecSubtitle[m_iCurrentIndex++];
}

void CSubtitle_Manager::Ready_Mission()
{
	m_vecMission.push_back(L"������ ���ٵ��");
	m_vecMission.push_back(L"�������� ã�ư���");
	m_vecMission.push_back(L"���� óġ�ϰ� ��� Ÿ��");
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
