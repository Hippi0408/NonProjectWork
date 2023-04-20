//**************************************************
// texture.h
// Author  : katsuki mizuki
//**************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>

//==================================================
// 定義
//==================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{
		TEXTURE_PLAYER = 0,						// プレイヤー
		TEXTURE_GROUND,							// 地面
		TEXTURE_SHADOW,							// 影
		TEXTURE_TITLE_BG,						// TITLE_BG
		TEXTURE_TITLE,							// TITLE
		TEXTURE_TUTORIAL,						// TUTORIAL
		TEXTURE_START,							// START
		TEXTURE_RESULT,							// RESULT
		TEXTURE_NUMBER,							// タイム
		TEXTURE_SNOW,							// 雪
		TEXTURE_SNOW_GROUND,					// 雪の地面
		TEXTURE_SKY,							// 空
		TEXTURE_COIN,							// コイン取得時のパーティクル
		TEXTURE_CHOICE_R,						// 選択アイコンR
		TEXTURE_CHOICE_L,						// 選択アイコンL
		TEXTURE_RANKING_NUMBER,					// 順位
		TEXTURE_PAUSE_BACK_TITLE,				// タイトルへ
		TEXTURE_PAUSE_RESTART,					// はじめから
		TEXTURE_PAUSE_CLOSE,					// とじる
		TEXTURE_PAUSE,							// ポーズ
		TEXTURE_PRESS_ENTER_OR_A,				// 押してほしいボタン
		TEXTURE_RANKING,						// ランキング
		TEXTURE_TUTORIAL_OPERATION_METHOD,		// 操作方法
		TEXTURE_TUTORIAL_MAP,					// マップ
		TEXTURE_TUTORIAL_MANUAL,				// チュートリアル

		TEXTURE_ENEMY0,							// エネミー
		TEXTURE_ENEMY1,							// エネミー
		TEXTURE_ENEMY2,							// エネミー
		TEXTURE_ENEMY3,							// エネミー
		TEXTURE_ENEMY4,							// エネミー
		TEXTURE_ENEMY5,							// エネミー

		TEXTURE_SCORE_EFFECT,

		TEXTURE_SCORE,
		TEXTURE_HIGH_SCORE,

		TEXTURE_MAX,
		TEXTURE_NONE,							// 使用しない
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public: /* メンバ関数 */
	void LoadAll();										// 全ての読み込み
	void Load(TEXTURE inTexture);						// 指定の読み込み
	void ReleaseAll();									// 全ての破棄
	void Release(TEXTURE inTexture);					// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// 情報の取得

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// テクスチャの情報
};

#endif // !_TEXTURE_H_
