//==================================================
// debug_proc.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <tchar.h> // _T

#include "manager.h"

#include "debug_proc.h"

//**************************************************
// 静的メンバ変数
//**************************************************
LPD3DXFONT CDebugProc::m_pFont = nullptr;
char CDebugProc::m_str[1024] = {};

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CDebugProc::CDebugProc()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CDebugProc::~CDebugProc()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CDebugProc::Init()
{
#ifdef _DEBUG
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CDebugProc::Uninit()
{
}

//--------------------------------------------------
// 登録
//--------------------------------------------------
void CDebugProc::Print(const char *pFormat, ...)
{
	char strCpy[256] = {};

	va_list args;
	va_start(args, pFormat);
	vsprintf(strCpy, pFormat, args);
	va_end(args);

	strcat(m_str, strCpy);
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CDebugProc::Draw()
{
	RECT rect = { 0, 0, CManager::SCREEN_WIDTH, CManager::SCREEN_HEIGHT };

	if (m_pFont != nullptr)
	{// テキスト描画
		m_pFont->DrawText(NULL, m_str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
		m_str[0] = '\0';
	}
}
