//==================================================
// title.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "camera.h"
#include "light.h"
#include "input.h"

#include "title.h"
#include "object2D.h"
#include "fade.h"
#include "sound.h"

#include "mesh_sky.h"

//**************************************************
// マクロ定義
//**************************************************
#define CHOICE_POS		(350.0f)
#define TITLE_HEIGHT	(200.0f)
#define MOVE_TITLE		(3.0f)

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CTitle::CTitle()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CTitle::~CTitle()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CTitle::Init()
{
	// サウンド
	CManager::GetManager()->GetSound()->Play(CSound::LABEL_BGM_TITLE);

	CManager::GetManager()->GetCamera()->SetPosV(D3DXVECTOR3(0.0f, 200.0f, -1000.0f));
	CManager::GetManager()->GetCamera()->SetPosR(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//背景
	m_pBG = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(CManager::SCREEN_WIDTH + 300.0f, CManager::SCREEN_WIDTH + 300.0f, 0.0f),
		PRIORITY_BG);
	m_pBG->SetTextureSize(30,30);
	m_pBG->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	m_pBG->SetTexture(CTexture::TEXTURE_TITLE_BG);
	m_pBG->SetMoveRot(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(0.02f)));

	// タイトル
	m_pObject2D = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, -300.0f, 0.0f),
		D3DXVECTOR3(700.0f, 700.0f, 0.0f),
		PRIORITY_BG);
	m_pObject2D->SetTexture(CTexture::TEXTURE_TITLE);

	// スタート
	m_pMenu[0] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 250.0f, 0.0f),
		D3DXVECTOR3(150.0f * 2.0f, 150.0f * 2.0f, 0.0f),
		PRIORITY_BG);
	m_pMenu[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMenu[0]->SetTexture(CTexture::TEXTURE_START);

	// チュートリアル
	m_pMenu[1] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 150.0f, 0.0f),
		D3DXVECTOR3(150.0f * 2.0f, 150.0f * 2.0f, 0.0f),
		PRIORITY_BG);
	m_pMenu[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	m_pMenu[1]->SetTexture(CTexture::TEXTURE_TUTORIAL);

	D3DXVECTOR3 menuPos = m_pMenu[0]->GetPos();

	// 矢印
	m_pChoice = CObject2D::Create(
		D3DXVECTOR3(D3DXVECTOR3(menuPos.x - CHOICE_POS, menuPos.y, menuPos.z)),
		D3DXVECTOR3(50.0f, 70.0f, 0.0f),
		PRIORITY_BG);
	m_pChoice->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pChoice->SetTexture(CTexture::TEXTURE_CHOICE_R);

	m_nSelect = SELECT_GAMEMODE_START;

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CTitle::Uninit()
{
	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CTitle::Update()
{
	MoveTitle_();

	CInput *pInput = CInput::GetKey();

	if (pInput->Trigger(KEY_UP))
	{
		// サウンド
		CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_SELECT);

		m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

		m_nSelect--;
		if (m_nSelect < 0)
		{
			m_nSelect = 1;
		}

		D3DXVECTOR3 menuPos = m_pMenu[m_nSelect]->GetPos();
		m_pChoice->SetPos(D3DXVECTOR3(menuPos.x - CHOICE_POS, menuPos.y, menuPos.z));
		m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (pInput->Trigger(KEY_DOWN))
	{
		// サウンド
		CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_SELECT);

		m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

		m_nSelect++;
		if (m_nSelect >= 2)
		{
			m_nSelect = 0;
		}

		D3DXVECTOR3 menuPos = m_pMenu[m_nSelect]->GetPos();
		m_pChoice->SetPos(D3DXVECTOR3(menuPos.x - CHOICE_POS, menuPos.y, menuPos.z));
		m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pInput->Trigger(KEY_DECISION))
	{
		// サウンド
		CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_ENTER);

		switch (m_nSelect)
		{
		case CTitle::SELECT_GAMEMODE_START:
			// 遷移
			CFade::GetInstance()->SetFade(CManager::MODE_GAME);
			break;

		case CTitle::SELECT_GAMEMODE_TUTRIAL:
			// 遷移
			CFade::GetInstance()->SetFade(CManager::MODE_TUTORIAL);
			break;

		default:
			assert(false);

			break;
		}
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CTitle *CTitle::Create()
{
	CTitle *pTitle;
	pTitle = new CTitle;

	if (pTitle != nullptr)
	{
		pTitle->Init();
	}
	else
	{
		assert(false);
	}

	return pTitle;
}

//--------------------------------------------------
// タイトルの動き
//--------------------------------------------------
void CTitle::MoveTitle_()
{
	D3DXVECTOR3 pos = m_pObject2D->GetPos();

	if (pos.y >= TITLE_HEIGHT)
	{
		pos.y = TITLE_HEIGHT;
	}
	else
	{
		pos.y += MOVE_TITLE;
	}

	m_pObject2D->SetPos(pos);
}