//==================================================
// game_mode.h
// Author: Buriya Kota
//==================================================
#ifndef _GAME_MODE_H_
#define _GAME_MODE_H_

//**************************************************
// インクルード
//**************************************************
#include "object.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// クラス
//**************************************************
class CGameMode : public CObject
{
public:
	CGameMode();
	~CGameMode() override;

	HRESULT Init() override { return S_OK; }
	void Uninit() override {}
	void Update() override {}
	void Draw(DRAW_MODE /*drawMode*/) override {}

private:

};

#endif	// _GAME_MODE_H_