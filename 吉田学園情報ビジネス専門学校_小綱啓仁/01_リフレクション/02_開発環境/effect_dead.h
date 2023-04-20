//==================================================
// 軌跡エフェクト
// Author: YudaKaito
//==================================================
#ifndef _EFFECT_DEAD_H_
#define _EFFECT_DEAD_H_

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
class CEffectDead : public CObject2D
{
private:
	static const int MAX_LIFE;
public:
	explicit CEffectDead(int nPriority = PRIORITY_EFFECT0);
	~CEffectDead();

	HRESULT Init() override;
	void Update() override;

	static CEffectDead *Create(const D3DXVECTOR3 pos, const D3DXCOLOR col);

private:
	int m_life;
};
#endif	// _TRAJECTORY_EFFECT_H_