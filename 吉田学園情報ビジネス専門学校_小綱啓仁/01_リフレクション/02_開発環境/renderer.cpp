//==================================================
// renderer.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>
#include <tchar.h> // _T

#include "manager.h"
#include "debug_proc.h"
#include "object2D.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "game.h"
#include "pause.h"

//**************************************************
// グローバル変数
//**************************************************
CCamera* g_pCamera;
CLight* g_pLight;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CRenderer::CRenderer()
{
	// Direct3Dオブジェクト
	m_pD3D = nullptr;
	// Deviceオブジェクト
	m_pD3DDevice = nullptr;
	// フォント
	m_pFont = nullptr;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CRenderer::~CRenderer()
{
	assert(m_pD3D == nullptr);
	assert(m_pD3DDevice == nullptr);
	assert(m_pFont == nullptr);
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CRenderer::Init(const HWND hWnd, const bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	m_bMesh = true;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = CManager::SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = CManager::SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;				// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;			// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;							// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if ((FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画と頂点処理をCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))))
	{
		// 生成失敗
		return E_FAIL;
	}

	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// フォグの有効設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	// フォグカラーの設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	// フォグモードの設定（範囲指定：D3DFOG_LINEAR、密度指定：D3DFOG_EXP）
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);
	// フォグの密度
	float fFogDensity = 0.001f;
	// 密度の設定
	m_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&fFogDensity));

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CRenderer::Uninit()
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif // _DEBUG

	// デバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CRenderer::Update()
{
	if (CManager::GetInstanse()->GetGameMode() != CManager::MODE_GAME)
	{
		// すべての更新
		CObject::UpdateAll();
	}
	else
	{
		CPause *pPause = CGame::GetGame()->GetPause();

		if (pPause->GetIsPause())
		{
			// ポーズのUpdate
			pPause->Update();
		}
		else
		{
			// すべての更新
			CObject::UpdateAll();
		}
	}

#ifdef _DEBUG
	// インプット
	CInput *pInput = CInput::GetKey();

	//ワイヤーフレーム切り替え
	if (pInput->Trigger(DIK_F1))
	{
		m_bMesh = !m_bMesh;
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, m_bMesh ? D3DFILL_SOLID : D3DFILL_WIREFRAME);
	}

	CDebugProc::Print("ワイヤーフレームの切り替え ( F1 ) ; [ %s ]\n\n", m_bMesh ? "OFF" : "ON");
#endif // DEBUG

}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CRenderer::Draw()
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// すべてのリリース
		CObject::DrawAll();

		CManager::GetManager()->GetCamera()->Set();

#ifdef _DEBUG

		// デバッグ
		CDebugProc::Draw();

#endif // _DEBUG

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//--------------------------------------------------
// FPSの表示
//--------------------------------------------------
void CRenderer::DrawFPS()
{
	int nCountFPS = GetFPS();
	RECT rect = { 0, 0, CManager::SCREEN_WIDTH, CManager::SCREEN_HEIGHT };
	TCHAR str[256];

	wsprintf(str, _T("FPS : %d\n"), nCountFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif // _DEBUG

//--------------------------------------------------
// デバイスの取得処理
//--------------------------------------------------
LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return m_pD3DDevice;
}
