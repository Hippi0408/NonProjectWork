//==================================================
// game.h
// Author: Buriya Kota
//==================================================
#ifndef _GAME_H_
#define _GAME_H_

//**************************************************
// インクルード
//**************************************************
#include "game_mode.h"
#include "model_data.h"

//**************************************************
// 名前付け
//**************************************************
namespace nl = nlohmann;

//**************************************************
// マクロ定義
//**************************************************

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************
class CScoreGame;
class CPause;
class CMeshField;
class CTimer;
class CObjectX;
class CEnemyManager;
class CPlayer;

//**************************************************
// クラス
//**************************************************
class CGame : public CGameMode
{
public:
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	// プレイヤーの情報の取得
	static CGame* GetGame() { return m_pGame; }
	CScoreGame* GetScore() { return m_pScore; }
	CPause* GetPause() { return m_pPause; }
	CMeshField* GetMeshField(int num) { return m_pMeshField[num]; }
	CTimer* GetTimer() { return m_pTimer; }
	CPlayer* GetPlayer() { return m_pPlayer; }

	// フレームの設定
	int GetFrame() { return m_time; }

	void End() { m_IsEnd = true; }
	bool IsEnd() { return m_IsEnd; }

	static CGame *Create();

private:
	int m_time;		// ゲーム開始からの時間
	static CGame* m_pGame;
	CScoreGame *m_pScore;
	CPause *m_pPause;
	std::vector<CMeshField*> m_pMeshField;
	CTimer *m_pTimer;
	CEnemyManager *m_EnemyManager;
	CPlayer *m_pPlayer;

	bool m_IsEnd;
	int m_endWaitTime;
};

#endif	// _GAME_H_