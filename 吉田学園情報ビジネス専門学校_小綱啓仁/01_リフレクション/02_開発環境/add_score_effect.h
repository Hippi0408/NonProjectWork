//==================================================
// 軌跡エフェクト
// Author: YudaKaito
//==================================================
#ifndef _ADD_SCORE_EFFECT_H_
#define _ADD_SCORE_EFFECT_H_

//**************************************************
// インクルード
//**************************************************
#include "object2D.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// 定数定義
//**************************************************

//**************************************************
// 構造体定義
//**************************************************

//**************************************************
// クラス
//**************************************************
class CAddScoreEffect : public CObject2D
{
private:
	static const int MAX_LIFE;
public:
	explicit CAddScoreEffect(int nPriority = PRIORITY_EFFECT0);
	~CAddScoreEffect();

	HRESULT Init() override;
	void Update() override;

	static CAddScoreEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col);

private:
	int m_life;
	D3DXVECTOR3 m_move;
};
#endif	// _WALL_HIT_EFFECT_H_