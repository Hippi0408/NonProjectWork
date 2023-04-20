//==================================================
// score_game.h
// Author: Yuda Katio
//==================================================
#ifndef _SCORE_GAME_H_
#define _SCORE_GAME_H_

//**************************************************
// �C���N���[�h
//**************************************************
#include "score.h"

//**************************************************
// �N���X
//**************************************************
class CScoreGame : public CScore
{
public:
	static const int MAX_SCORE = 8;
public:
	explicit CScoreGame(int nPriority = PRIORITY_BG);
	~CScoreGame() override;

	HRESULT Init() override;
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void SetScore(int nScore);
	void AddScore(int nValue);

	static CScoreGame *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
	// �ʒu
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;

	// ����
	int m_digits;
};

#endif	// _SCORE_H_