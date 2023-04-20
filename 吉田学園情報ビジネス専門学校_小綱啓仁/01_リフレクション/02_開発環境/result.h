//==================================================
// result.h
// Author: Buriya Kota
//==================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//**************************************************
// インクルード
//**************************************************
#include "game_mode.h"
#include "texture.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************
class CObject2D;

//**************************************************
// クラス
//**************************************************
class CResult : public CGameMode
{
public:
	CResult();
	~CResult() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	static CResult *Create();

private:
	// オブジェクト2Dの箱
	CObject2D *m_pObject2D[2];
	// 位置
	D3DXVECTOR3 m_pos;
	// 大きさ
	D3DXVECTOR3 m_size;

};

#endif	// _RESULT_H_