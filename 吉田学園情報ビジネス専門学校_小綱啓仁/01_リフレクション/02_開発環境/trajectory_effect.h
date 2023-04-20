//==================================================
// 軌跡エフェクト
// Author: YudaKaito
//==================================================
#ifndef _TRAJECTORY_EFFECT_H_
#define _TRAJECTORY_EFFECT_H_

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
class CEffectTrajectory : public CObject2D
{
private:
	static const int MAX_LIFE;
public:
	explicit CEffectTrajectory(int nPriority = PRIORITY_EFFECT0);
	~CEffectTrajectory();

	HRESULT Init() override;
	void Update() override;

	static CEffectTrajectory *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const D3DXCOLOR col);

private:
	int m_life;
};
#endif	// _TRAJECTORY_EFFECT_H_