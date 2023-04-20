//==================================================
// debug_proc.h
// Author: Buriya Kota
//==================================================
#ifndef _DEBUG_PROC_H_
#define _DEBUG_PROC_H_

//**************************************************
// インクルード
//**************************************************
#include <functional>

#include "renderer.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// クラス
//**************************************************
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();

	HRESULT Init();
	void Uninit();

	// 登録関数
	static void Print(const char *pFormat, ...);
	// 描画
	static void Draw();

private:
	static LPD3DXFONT m_pFont;
	static char m_str[1024];

};

#endif	// _DEBUG_PROC_H_
