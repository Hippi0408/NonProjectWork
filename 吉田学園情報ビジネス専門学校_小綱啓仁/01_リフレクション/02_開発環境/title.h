//==================================================
// title.h
// Author: Buriya Kota
//==================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//**************************************************
// インクルード
//**************************************************
#include "game_mode.h"
#include "texture.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************
class CObject2D;
class CCameraTitle;
class CLight;

//**************************************************
// クラス
//**************************************************
class CTitle : public CGameMode
{
public:
	static const int BG_TITLE = 2;
	static const int MENU = 2;

public:
	enum SELECT_GAMEMODE
	{
		SELECT_GAMEMODE_START = 0,
		SELECT_GAMEMODE_TUTRIAL,
		SELECT_GAMEMODE_MAX
	};

	CTitle();
	~CTitle() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	static CTitle *Create();

private:
	void MoveTitle_();

private:
	// オブジェクト2Dの箱
	CObject2D *m_pObject2D;
	CObject2D *m_pMenu[MENU];
	CObject2D *m_pChoice;
	CObject2D *m_pBG;

	CCameraTitle *m_pCameraTitle;
	CLight *m_pLight;
	// 位置
	D3DXVECTOR3 m_pos;
	// 大きさ
	D3DXVECTOR3 m_size;
	// 選択しているモードのカウント
	int m_nSelect;
};

#endif	// _TITLE_H_