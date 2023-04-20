//==================================================
// renderer.h
// Author: Buriya Kota
//==================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//**************************************************
// インクルード
//**************************************************
#include <d3dx9.h>

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// クラス
//**************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(const HWND hWnd, const bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice();		// {return m_pD3DDevice;} returnするだけならこういう書き方もある

private:
	void DrawFPS();

private:
	// Direct3Dオブジェクト
	LPDIRECT3D9 m_pD3D;
	// Deviceオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	// フォント
	LPD3DXFONT m_pFont;
	// ワイヤーフレームの切り替え
	bool m_bMesh;
};

#endif	// _RENDERER_H_