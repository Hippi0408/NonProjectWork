//==================================================
// pause.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "input.h"
#include "sound.h"

#include "pause.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CPause::CPause(int nPriority) : CObject(nPriority)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CPause::~CPause()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CPause::Init()
{
	m_nSelect = 0;
	m_IsPause = false;

	// BG
	m_pObject2D[0] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f), 
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, (float)CManager::SCREEN_HEIGHT, 0.0f),
		PRIORITY_UI);
	m_pObject2D[0]->SetTexture(CTexture::TEXTURE_NONE);
	m_pObject2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// タイトル
	m_pObject2D[1] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, 100.0f, 0.0f),
		D3DXVECTOR3(400.0f, 400.0f, 0.0f),
		PRIORITY_UI);
	m_pObject2D[1]->SetTexture(CTexture::TEXTURE_PAUSE);
	m_pObject2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// リスタート
	m_pMenu[0] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 400.0f, 0.0f),
		D3DXVECTOR3(400.0f, 400.0f, 0.0f),
		PRIORITY_UI);
	m_pMenu[0]->SetTexture(CTexture::TEXTURE_PAUSE_RESTART);
	m_pMenu[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// タイトルに戻る
	m_pMenu[1] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 250.0f, 0.0f),
		D3DXVECTOR3(400.0f, 400.0f, 0.0f),
		PRIORITY_UI);
	m_pMenu[1]->SetTexture(CTexture::TEXTURE_PAUSE_BACK_TITLE);
	m_pMenu[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// 閉じる
	m_pMenu[2] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 100.0f, 0.0f),
		D3DXVECTOR3(400.0f, 400.0f, 0.0f),
		PRIORITY_UI);
	m_pMenu[2]->SetTexture(CTexture::TEXTURE_PAUSE_CLOSE);
	m_pMenu[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CPause::Uninit()
{
	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CPause::Update()
{
	CInput *pInput = CInput::GetKey();

	if (((pInput->Trigger(DIK_P) || pInput->Trigger(JOYPAD_START, 0)) && !m_IsPause))
	{
		m_IsPause = true;

		m_pObject2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		m_pObject2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		for (int nCntMenu = 0; nCntMenu < 3; nCntMenu++)
		{
			m_pMenu[nCntMenu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (((pInput->Trigger(DIK_P) || pInput->Trigger(JOYPAD_START, 0)) && m_IsPause))
	{
		m_nSelect = 0;
		m_IsPause = false;

		for (int nCntObject = 0; nCntObject < 2; nCntObject++)
		{
			m_pObject2D[nCntObject]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		for (int nCntMenu = 0; nCntMenu < 3; nCntMenu++)
		{
			m_pMenu[nCntMenu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	if (m_IsPause)
	{
		if (pInput->Trigger(KEY_UP))
		{
			// サウンド
			CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_SELECT);

			m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

			m_nSelect--;
			if (m_nSelect < 0)
			{
				m_nSelect = 2;
			}

			m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (pInput->Trigger(KEY_DOWN))
		{
			// サウンド
			CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_SELECT);

			m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

			m_nSelect++;
			if (m_nSelect >= 3)
			{
				m_nSelect = 0;
			}

			m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (pInput->Trigger(KEY_DECISION))
		{
			// サウンド
			CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_ENTER);

			m_IsPause = false;

			// 選ばれたモードの処理
			switch (m_nSelect)
			{
			case CPause::SELECT_MODE_RESTART:
				// 遷移
				CFade::GetInstance()->SetFade(CManager::MODE_GAME);
				break;

			case CPause::SELECT_MODE_TAITLE:
				// 遷移
				CFade::GetInstance()->SetFade(CManager::MODE_TITLE);
				break;

			case CPause::SELECT_MODE_CLOSE:
				for (int nCntObject = 0; nCntObject < 2; nCntObject++)
				{
					m_pObject2D[nCntObject]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				for (int nCntMenu = 0; nCntMenu < 3; nCntMenu++)
				{
					m_pMenu[nCntMenu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				break;

			default:
				assert(false);
				break;
			}
		}
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CPause * CPause::Create()
{
	CPause *pPause;
	pPause = new CPause;

	if (pPause != nullptr)
	{
		pPause->Init();
	}
	else
	{
		assert(false);
	}

	return pPause;
}
