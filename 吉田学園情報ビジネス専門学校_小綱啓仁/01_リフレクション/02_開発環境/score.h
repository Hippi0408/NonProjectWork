//==================================================
// score.h
// Author: Buriya Kota
//==================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//**************************************************
// インクルード
//**************************************************
#include "object.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************
class CNumber;

//**************************************************
// クラス
//**************************************************
class CScore : public CObject
{
public:
	static const int MAX_SCORE = 8;
public:
	explicit CScore(int nPriority = PRIORITY_EFFECT);
	~CScore() override;

	HRESULT Init() override;
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	void SetLight(bool isLight) { m_isLight = isLight; }
	bool IsLight() { return m_isLight; }

	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void SetColor(D3DXCOLOR color);

	void SetScore(int nScore);
	void AddScore(int nValue) { SetScore(m_nScore + nValue); }
	int GetScore() { return m_nScore; }

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

protected:
	// Number型の配列
	CNumber *m_pNumber[MAX_SCORE];
	// スコアの値
	int m_nScore;
	// ランキングに乗ったかどうか
	bool m_isLight;
	// カウント
	int m_nTime;
	// 色
	D3DXCOLOR m_col;

	// 桁数
	int m_digits;

	// 位置
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
};

#endif	// _SCORE_H_