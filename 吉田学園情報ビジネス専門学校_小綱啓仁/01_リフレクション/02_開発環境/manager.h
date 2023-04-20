//==================================================
// manager.h
// Author: Buriya Kota
//==================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//**************************************************
// インクルード
//**************************************************
#include <d3dx9.h>

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************
class CObject;
class CRenderer;
class CInputKeyboard;
class CTexture;
class CCamera;
class CLight;
class CEnemy3D;
class CInput;
class CDebugProc;
class CPlayer3D;
class CSound;
class CModelData;

//**************************************************
// クラス
//**************************************************
class CManager
{
public:
	// スクリーンの幅
	static const int SCREEN_WIDTH = 1280;
	// スクリーンの高さ
	static const int SCREEN_HEIGHT = 720;

public:
	static CManager* GetInstanse();	// マネージャーの生成されていなかったら生成。マネージャー情報の取得
	static CManager* GetManager() { return m_pManager; }	// マネージャーの情報の取得
private:
	CManager();
	static CManager* m_pManager;

public:
	//画面(モード)の種類
	enum MODE
	{
		MODE_TITLE = 0,		//タイトル画面
		MODE_TUTORIAL,		//チュートリアル
		MODE_GAME,			//ゲーム画面
		MODE_GAMEOVER,		//ゲームオーバー画面
		MODE_RESULT,		//リザルト画面
		MODE_RANKING,		//ランキング
		MODE_MAX
	};

	~CManager();

	HRESULT Init(const HINSTANCE hInstance, const HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	// レンダラーの情報の取得
	CRenderer *GetRenderer() { return m_pRenderer; }
	// キーボードの情報の取得
	CInput *GetInput() { return m_pInput; }
	// テクスチャの情報の取得
	CTexture* GetTexture() { return m_pTexture; }
	// モデルデータの情報の取得
	CModelData* GetModelData() { return m_pModelData; }
	// カメラの情報の取得
	CCamera* GetCamera() { return m_pCamera; }
	// ライトの情報の取得
	CLight* GetLight() { return m_pLight; }
	// サウンド情報の取得
	CSound* GetSound() { return m_pSound; }

	void SetNowScore(int score) { m_nowScore = score; }
	int GetNowScore() { return m_nowScore; }

	MODE GetGameMode() { return m_mode; }

	void SetMode(MODE mode);

private:
	// 現在のモード
	MODE m_mode;
	CObject *m_pGameMode;
	CRenderer *m_pRenderer;
	CTexture *m_pTexture;
	CModelData *m_pModelData;
	CCamera *m_pCamera;
	CLight *m_pLight;
	CDebugProc *m_pDebug;
	CSound *m_pSound;
	CInput *m_pInput;

	// 現在のスコア
	int m_nowScore;

};

#endif	// _MANAGER_H_