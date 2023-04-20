//==================================================
// fade.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "fade.h"

//**************************************************
// 静的メンバ変数
//**************************************************
CFade *CFade::m_pFade = nullptr;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CFade::CFade(int nPriority /* =6 */) : CObject2D(nPriority)
{
	m_pObject2D = nullptr;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CFade::~CFade()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CFade::Init()
{
	CObject2D::Init();

	SetType(TYPE_MODE);
	m_fFadeSpeed = 0.07f;
	m_fMaxAlpha = 0.0f;
	m_fade = FADE_NONE;

	m_pObject2D = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3((float)CManager::SCREEN_WIDTH, (float)CManager::SCREEN_HEIGHT, 0.0f),
		PRIORITY_FADE);
	m_pObject2D->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	m_pObject2D->SetType(TYPE_MODE);

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CFade::Uninit()
{
	// リリースはリリースオールでやってある
	m_pFade = nullptr;

	CObject2D::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CFade::Update()
{
	CObject2D::Update();

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{// α値の増減
			m_fMaxAlpha -= m_fFadeSpeed;

			if (m_fMaxAlpha <= 0.0f)
			{// αが０以下になったら消す
				m_fMaxAlpha = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_fMaxAlpha += m_fFadeSpeed;

			if (m_fMaxAlpha >= 1.0f)
			{// α値が位置を超えたらモード変更
				m_fMaxAlpha = 1.0f;
				m_fade = FADE_IN;
				CManager::GetInstanse()->SetMode(m_modeNext);
			}
		}
	}

	if (m_pObject2D != nullptr)
	{
		// 色の変更
		m_pObject2D->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fMaxAlpha));
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CFade::Draw(DRAW_MODE drawMode)
{
	CObject2D::Draw(drawMode);
}

//--------------------------------------------------
// フェードの設定
//--------------------------------------------------
void CFade::SetFade(CManager::MODE modeNext)
{
	// フェードアウト状態に
	m_fade = FADE_OUT;
	// 次の画面を設定
	m_modeNext = modeNext;

	if (m_pObject2D != nullptr)
	{
		// 黒いポリゴンにしておく
		m_pObject2D->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

//--------------------------------------------------
// シングルトンのインスタンス取得
//--------------------------------------------------
CFade *CFade::GetInstance()
{
	if (m_pFade == nullptr)
	{
		m_pFade = new CFade;
		m_pFade->Init();
	}

	return m_pFade;
}
