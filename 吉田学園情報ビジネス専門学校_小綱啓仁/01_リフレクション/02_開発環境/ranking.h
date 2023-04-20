//==================================================
// ranking.h
// Author: Buriya Kota
//==================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//**************************************************
// インクルード
//**************************************************
#include "game_mode.h"
#include "texture.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************
class CObject2D;
class CScore;

//**************************************************
// クラス
//**************************************************
class CRanking : public CGameMode
{
public:
	static const int MAX_RANK = 1;

public:
	enum SELECT_GAMEMODE
	{
		SELECT_GAMEMODE_RESTART = 0,
		SELECT_GAMEMODE_TITLE,
		SELECT_GAMEMODE_MAX
	};

	CRanking();
	~CRanking() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	void LoadRanking();
	void SaveRanking();
	void SetRanking(int nScore);

	static CRanking *Create();

private:
	// オブジェクト2Dの箱
	CObject2D *m_pRanking;
	// 押しほしいボタン
	CObject2D *m_pPress;
	// 押しほしいボタン
	CObject2D *m_pScoreTex;
	// 押しほしいボタン
	CObject2D *m_pHighScoreTex;

	// ハイスコア
	CScore *m_pHighScore[MAX_RANK];
	// マイスコア
	CScore *m_pMyScore;
	// 位置
	D3DXVECTOR3 m_pos;
	// 大きさ
	D3DXVECTOR3 m_size;
	// スコア保存変数
	int m_aRankingData[MAX_RANK];
	// スコア
	int m_nMyScore;
	// 選択しているモードのカウント
	int m_nSelect;
	CObject2D *m_pMenu[2];
};

#endif	// _RESULT_H_