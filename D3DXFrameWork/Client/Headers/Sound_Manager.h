#pragma once
// ���� ���� �ҽ� Ȯ�� �� �м� �ؾ��� ��

// 1. stdafx.h�� ������� �߰� �κ�
// 2. mainapp Ŭ���� Initialize �� Release �Լ�
// 3. Stage Ŭ���� Initialize �� Progress �Լ� 
// 4. ������Ʈ ����(�Ϻ� ������ Texture ����ó�� Sound������ �ִ� �� Ȯ��)
// 5. ���Ҿ wav���ϸ� �����Ǵ� ������. 
// 6. ����(�� ����)���� DirectX SDK�� �˻�. june2010�������� �ٿ�ް� ��ġ. Ȥ�� ������ �۾��ϴٰ� �ȵȴٸ� directx sdk�� ������, �� ��ġ �� ���ͳݿ��� 
//	  �ٿ�޾� �ʼ������� ��ġ�ؾ� ��. june2010������ �ְ�, ��ġ �� ���ٴ� ���� �ƴ�.
//	  �ڼ��� ��ġ������ ���ͳݿ� �˻��ϸ� ���� ����. ������ �𸣰����� �����ð��� ������.
//	  ��ȭ�� ���ڷ� �ϱ⿣ �ʹ� �� ������.
// ��ġ���� : ��ġ�ϸ� c:\programfiles\directx sdk\Microsoft DirectX SDK (June 2010)�� ��ġ�� ��.
//			: �����Ϸ��� ����->�ɼ�->������Ʈ�׼ַ��->VC++���丮->����ڽ��޴��� ���������� �������Ϸ� �ٲ۴�->�ι�° �������ǥ�ø� Ŭ���ϸ� 
//          : ���� ���� �߰��ȴµ� �ű�� �� ġ������ �����ʿ� ���׸� �׸� Ŭ���Ѵ��� ���� ��η� ã�ư��� Ȯ��.(c:\programfiles\directx sdk\Microsoft DirectX SDK (June 2010)\Include\)

//			: �����Ϸ��� ����->�ɼ�->������Ʈ�׼ַ��->VC++���丮->����ڽ��޴��� ���������� ���̺귯�����Ϸ� �ٲ۴�->�ι�° �������ǥ�ø� Ŭ���ϸ� 
//          : ���� ���� �߰��ȴµ� �ű�� �� ġ������ �����ʿ� ���׸� �׸� Ŭ���Ѵ��� ���� ��η� ã�ư��� Ȯ��.(c:\programfiles\directx sdk\Microsoft DirectX SDK (June 2010)\lib\x86)			


#include "Defines.h"
#include "Base.h"
#include "Graphic_Device.h"

class CSound_Manager final : public CBase
{
	_DECLARE_SINGLETON(CSound_Manager)

private:
	explicit CSound_Manager(void);
	virtual ~CSound_Manager(void) = default;
private:
	//directx sdk (����ũ�μ���Ʈ���� ��ǻ�Ϳ��ִ� �ϵ��� ���� ����Ҽ� �ְ�
	// �������� �Լ���, ���������� �����س��� ������ϰ� ���̺귯�������� ����.)
	//���̷�Ʈ ���� �������̽�
	LPDIRECTSOUND				m_pSound; //����ī�带 ��ǥ�ϴ� ��ü.
	vector<LPDIRECTSOUNDBUFFER> m_vecSoundBuff;	//���������� �ε��ؼ� ������ ����

public:
	HRESULT Ready_Sound_Manager(void);	// ��ġ�� �ʱ�ȭ �ϴ¿�Ȱ.
	HRESULT Add_SoundWaveFile(TCHAR* pFileName);	//���ϰ�θ� ���� ������ �о� �帮�ڴ�.
	void SoundPlay(int iIndex /*���° ������ ����ҷ�?*/, DWORD dwFlag/*���ѷ���, �ѹ�*/); //������ ��� �ϰڴ�.
	void SoundStop(int iIndex);
	bool SoundPlaying(int iIndex);	// ���� iIndex��° ���尡 ����ǰ��ִ��� �ƴ���.
protected:
	virtual void Free();
};
