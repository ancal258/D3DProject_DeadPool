#include "stdafx.h"
#include "..\Headers\SubtitleManager.h"

_IMPLEMENT_SINGLETON(CSubtitle_Manager)


CSubtitle_Manager::CSubtitle_Manager()
{
}

void CSubtitle_Manager::Ready_Subtitle()
{
	m_vecSubtitle.push_back(L"일어났니 데드풀?");
	m_vecSubtitle.push_back(L"ㅇㅇ, 뭐야 왜이리 게임이 밋밋해?");
	m_vecSubtitle.push_back(L"시간이.. 시간이 부족했어...");
	m_vecSubtitle.push_back(L"조명도 없고, UI같은 것도 없고 말이야");
	m_vecSubtitle.push_back(L"난 최선을 다한거야 ㅜㅜ");
	m_vecSubtitle.push_back(L"게임할 시간, 놀 시간은 있고?");
	m_vecSubtitle.push_back(L"TV 쪽으로 가봐, 선물을 준비했어.");
	m_vecSubtitle.push_back(L"무슨 선물.. 뭐 오리라도 튀어나오나");
	m_vecSubtitle.push_back(L"ㄴㄴ 아무튼 가봐!");
	m_vecSubtitle.push_back(L"   "); 
	m_vecSubtitle.push_back(L"에휴... 시연회 중간에 터트리기나 하고");
	m_vecSubtitle.push_back(L"여보세요?");
	m_vecSubtitle.push_back(L"아니 이거 이대로면 난 더 이상 안해!");
	m_vecSubtitle.push_back(L"아!!!!!!!! 아 알겠어!! 기다려봐 당장 고칠게");
	m_vecSubtitle.push_back(L"고치는 김에 좀 게임답게 꾸며줘봐");
	m_vecSubtitle.push_back(L"그게 그렇게 쉽게 됐으면 진작 했지 ㅡㅡ");
	m_vecSubtitle.push_back(L"어차피 뭐 맵 찍은 것 밖에 없구만;");
	m_vecSubtitle.push_back(L"?? 니 그 ㅈ같은 애니메이션만 1주일을 잡았다.");
	m_vecSubtitle.push_back(L"뭐 아무튼 당장 고쳐놔.");
	// 비키니걸
	m_vecSubtitle.push_back(L"도와주세요!!!");
	m_vecSubtitle.push_back(L"넌 왜 여기 같혀 있는 거야");
	m_vecSubtitle.push_back(L"모르겠어요... 눈을 뜨니...");
	m_vecSubtitle.push_back(L"심의에 걸릴까봐 안되겠다. 미안");
	m_vecSubtitle.push_back(L"... ... 심의?");
}

const _tchar * CSubtitle_Manager::Get_Subtitle()
{
	return m_vecSubtitle[m_iCurrentIndex++];
}

void CSubtitle_Manager::Ready_Mission()
{
	m_vecMission.push_back(L"강아지 쓰다듬기");
	m_vecMission.push_back(L"오리한테 찾아가기");
	m_vecMission.push_back(L"적을 처치하고 헬기 타기");
	m_vecMission.push_back(L"미션 4");
	m_vecMission.push_back(L"미션 5");
	m_vecMission.push_back(L"미션 6");
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
