//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
const char* CTexture::s_FileName[] =
{// テクスチャのパス
	"data/TEXTURE/player.png",		// プレイヤー
	"data/TEXTURE/thumbnail.png",			// 地面
	"data/TEXTURE/shadow000.jpg",			// 影
	"data/TEXTURE/bg_tile.png",				// TITLE_BG
	"data/TEXTURE/TITLE.png",				// TITLE
	"data/TEXTURE/TUTORIAL.png",			// TUTORIAL
	"data/TEXTURE/START.png",				// START
	"data/TEXTURE/RESULT.png",				// RESULT
	"data/TEXTURE/number.png",				// タイム
	"data/TEXTURE/snow1.png",				// 雪
	"data/TEXTURE/snow_ground.png",			// 雪の地面
	"data/TEXTURE/sky.jpg",					// 空
	"data/TEXTURE/Glitter000.jpg",			// コイン
	"data/TEXTURE/choice_R.png",			// 選択アイコンR
	"data/TEXTURE/choice_L.png",			// 選択アイコンL
	"data/TEXTURE/ranking_number.png",		// 順位
	"data/TEXTURE/back_title.png",			// タイトルへ
	"data/TEXTURE/restart.png",				// はじめから
	"data/TEXTURE/close.png",				// とじる
	"data/TEXTURE/pause.png",				// ポーズ
	"data/TEXTURE/press_enter_or_a.png",	// 押してほしいボタン
	"data/TEXTURE/ranking.png",				// ランキング
	"data/TEXTURE/TUTORIAL03.png",		// 操作方法
	"data/TEXTURE/TUTORIAL01.png",			// マップ
	"data/TEXTURE/TUTORIAL02.png",				// チュートリアル
	"data/TEXTURE/enemy00.png",			// エネミー
	"data/TEXTURE/enemy01.png",			// エネミー
	"data/TEXTURE/enemy02.png",			// エネミー
	"data/TEXTURE/enemy03.png",			// エネミー
	"data/TEXTURE/invincible_item.png",	// エネミー
	"data/TEXTURE/enemy05.png",			// エネミー
	"data/TEXTURE/score_effect.png",	// スコアエフェクト
	"data/TEXTURE/score.png",			// スコア
	"data/TEXTURE/high_score.png",		// ハイスコア
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void CTexture::LoadAll()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// テクスチャの読み込みがされている
			continue;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの解放
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = nullptr;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// 読み込み
	Load(inTexture);

	return s_pTexture[inTexture];
}
