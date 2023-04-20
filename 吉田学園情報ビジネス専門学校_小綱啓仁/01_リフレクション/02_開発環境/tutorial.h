//==================================================
// tutorial.h
// Author: Buriya Kota
//==================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//**************************************************
// インクルード
//**************************************************
#include "game_mode.h"

//**************************************************
// 前方宣言
//**************************************************
class CObject2D;

//**************************************************
// クラス
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
	{// 状態
		MODE_INPUT = 0,
		MODE_SCROLL_L,
		MODE_SCROLL_R,
		MODE_MAX
	};

	enum TEXTURE
	{// 使用しているテクスチャ
		TEXTURE_MAP = 0,
		TEXTURE_MANUAL,
		TEXTURE_OPERATION_METHOD,
		TEXTURE_MAX
	};

	enum TEXTURE_CHOICE
	{// 使用しているテクスチャ
		TEXTURE_R = 0,
		TEXTURE_L,
		TEXTURE_CHOICE_MAX
	};

	// オブジェクト2Dの箱
	CObject2D *m_pObject2D[TEXTURE_MAX];
	CObject2D *m_pChoice[2];

	int m_time;

	MODE m_mode;
	TEXTURE m_disp;
};

#endif	// _TUTORIAL_H_