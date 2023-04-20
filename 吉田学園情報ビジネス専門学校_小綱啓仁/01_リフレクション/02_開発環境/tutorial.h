//==================================================
// tutorial.h
// Author: Buriya Kota
//==================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//**************************************************
// �C���N���[�h
//**************************************************
#include "game_mode.h"

//**************************************************
// �O���錾
//**************************************************
class CObject2D;

//**************************************************
// �N���X
//**************************************************
class CTutorial : public CGameMode
{
public:
	CTutorial();
	~CTutorial() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	static CTutorial *Create();

private:
	enum MODE
	{// ���
		MODE_INPUT = 0,
		MODE_SCROLL_L,
		MODE_SCROLL_R,
		MODE_MAX
	};

	enum TEXTURE
	{// �g�p���Ă���e�N�X�`��
		TEXTURE_MAP = 0,
		TEXTURE_MANUAL,
		TEXTURE_OPERATION_METHOD,
		TEXTURE_MAX
	};

	enum TEXTURE_CHOICE
	{// �g�p���Ă���e�N�X�`��
		TEXTURE_R = 0,
		TEXTURE_L,
		TEXTURE_CHOICE_MAX
	};

	// �I�u�W�F�N�g2D�̔�
	CObject2D *m_pObject2D[TEXTURE_MAX];
	CObject2D *m_pChoice[2];

	int m_time;

	MODE m_mode;
	TEXTURE m_disp;
};

#endif	// _TUTORIAL_H_