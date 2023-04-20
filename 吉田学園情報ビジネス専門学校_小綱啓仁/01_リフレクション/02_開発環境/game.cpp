//==================================================
// game.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "pause.h"
#include "game.h"
#include "ranking.h"
#include "utility.h"

#include "fade.h"
#include "score_game.h"
#include "timer.h"
#include "silhouette.h"
#include "object3D.h"

#include "model.h"
#include "meshfield.h"
#include "mesh_sky.h"
#include "particle.h"
#include "player.h"

#include"enemy_manager.h"
#include "wall_hit_effect.h"

//**************************************************
// マクロ定義
//**************************************************
#define MAX_GOAL	(2)

//**************************************************
// 静的メンバ変数
//**************************************************
CGame *CGame::m_pGame = nullptr;

//**************************************************
// マクロ定義
//**************************************************

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CGame::CGame()
{
	m_pScore = nullptr;
	m_pPause = nullptr;
	m_pTimer = nullptr;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CGame::~CGame()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CGame::Init()
{
	CManager::GetManager()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	// 背景
	{
		CObject2D* bg = CObject2D::Create(
			D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f),
			D3DXVECTOR3(CManager::SCREEN_WIDTH + 300.0f, CManager::SCREEN_WIDTH + 300.0f, 0.0f),
			PRIORITY_BG);
		bg->SetTextureSize(30, 30);
		bg->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		bg->SetTexture(CTexture::TEXTURE_TITLE_BG);
		bg->SetMoveRot(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(0.02f)));
	}

	CManager::GetManager()->GetCamera()->SetPosV(D3DXVECTOR3(0.0f, 200.0f, -500.0f));
	CManager::GetManager()->GetCamera()->SetPosR(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_time = 0;

	m_pScore = CScoreGame::Create(D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(80.0f, 200.0f, 0.0f));
	m_pScore->SetScore(0);

	m_pPause = CPause::Create();

	m_pPlayer = CPlayer::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(50.0f, 50.0f, 0.0f));

	m_EnemyManager = CEnemyManager::Create();
	
	m_endWaitTime = 120;

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CGame::Uninit()
{
	// スコアの設定
	CManager::GetInstanse()->SetNowScore(m_pScore->GetScore());

	m_pTimer = nullptr;
	m_pPause = nullptr;

	if (m_EnemyManager != nullptr)
	{
		delete m_EnemyManager;
		m_EnemyManager = nullptr;
	}

	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CGame::Update()
{
	m_time++;
	m_EnemyManager->Update();
#ifdef _DEBUG
	CInput *pInput = CInput::GetKey();

	if (pInput->Trigger(DIK_RETURN) || pInput->Trigger(JOYPAD_B, 0))
	{
		// 遷移
		CFade::GetInstance()->SetFade(CManager::MODE_RANKING);
	}

	if (pInput->Trigger(DIK_U) || pInput->Trigger(JOYPAD_B, 0))
	{
		// 遷移
		m_pScore->AddScore(5);
	}
#endif // _DEBUG

	if (m_IsEnd)
	{
		if (m_pPlayer != nullptr)
		{
			D3DXVECTOR3 pos = m_pPlayer->GetPos();
			m_pPlayer->Uninit();
			m_pPlayer = nullptr;

			for (int i = 0; i < 200;i++)
			{
				CEffectWallHit::Create(pos, D3DXVECTOR3(FloatRandam(1.0f, -1.0f), FloatRandam(1.0f, -1.0f),0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		m_endWaitTime--;

		if (m_endWaitTime <= 0)
		{
			// 遷移
			CFade::GetInstance()->SetFade(CManager::MODE_RANKING);
		}
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CGame* CGame::Create()
{
	m_pGame = new CGame;

	if (m_pGame != nullptr)
	{
		m_pGame->Init();
	}
	else
	{
		assert(false);
	}

	return m_pGame;
}
