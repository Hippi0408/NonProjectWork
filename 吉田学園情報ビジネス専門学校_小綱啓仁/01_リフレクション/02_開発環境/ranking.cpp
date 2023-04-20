//==================================================
// ranking.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "input.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "sound.h"
#include "mesh_sky.h"
#include "camera.h"

// jsonのinclude
#include "nlohmann/json.hpp"
#include <fstream>

// 名前の省略
namespace nl = nlohmann;
// ファイルパスの設定
const char* pathToJSON = "data/FILE/ranking.json";
// ファイルパスの設定
const char* pathToJsonScore = "data/FILE/my_score.json";

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CRanking::CRanking()
{
	m_nMyScore = 0;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CRanking::~CRanking()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CRanking::Init()
{
	//CManager::GetManager()->GetSound()->Play(CSound::LABEL_BGM_RANKING);

	CManager::GetManager()->GetCamera()->SetPosV(D3DXVECTOR3(0.0f, 200.0f, -1000.0f));
	CManager::GetManager()->GetCamera()->SetPosR(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

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

	// ハイスコア
	m_pHighScoreTex = CObject2D::Create(
		D3DXVECTOR3(500.0f, 100.0f, 0.0f),
		D3DXVECTOR3(400.0f, 300.0f, 0.0f),
		PRIORITY_BG);
	m_pHighScoreTex->SetTexture(CTexture::TEXTURE_HIGH_SCORE);
	m_pHighScoreTex->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// スコア
	m_pScoreTex = CObject2D::Create(
		D3DXVECTOR3(500.0f, 200.0f, 0.0f),
		D3DXVECTOR3(400.0f, 300.0f, 0.0f),
		PRIORITY_BG);
	m_pScoreTex->SetTexture(CTexture::TEXTURE_SCORE);
	m_pScoreTex->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_aRankingData[nCntRank] = 0;
		// ハイスコア
		m_pHighScore[nCntRank] = CScore::Create(D3DXVECTOR3(800.0f, 100.0f + nCntRank * 100.0f, 0.0f), D3DXVECTOR3(60.0f, 80.0f, 0.0f));
		m_pHighScore[nCntRank]->SetScore(m_aRankingData[nCntRank]);
	}

	// マイスコア
	m_pMyScore = CScore::Create(D3DXVECTOR3(800.0f, 200.0f, 0.0f), D3DXVECTOR3(60.0f, 80.0f, 0.0f));
	m_pMyScore->SetScore(m_nMyScore);

	// スタート
	m_pMenu[0] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 250.0f, 0.0f),
		D3DXVECTOR3(150.0f * 2.0f, 150.0f * 2.0f, 0.0f),
		PRIORITY_BG);
	m_pMenu[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMenu[0]->SetTexture(CTexture::TEXTURE_PAUSE_RESTART);

	// チュートリアル
	m_pMenu[1] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 150.0f, 0.0f),
		D3DXVECTOR3(150.0f * 2.0f, 150.0f * 2.0f, 0.0f),
		PRIORITY_BG);
	m_pMenu[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	m_pMenu[1]->SetTexture(CTexture::TEXTURE_PAUSE_BACK_TITLE);

	m_nSelect = SELECT_GAMEMODE_RESTART;

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CRanking::Uninit()
{
	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CRanking::Update()
{
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
		m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pInput->Trigger(KEY_DECISION))
	{
		// サウンド
		CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_ENTER);

		switch (m_nSelect)
		{
		case CRanking::SELECT_GAMEMODE_RESTART:
			// 遷移
			CFade::GetInstance()->SetFade(CManager::MODE_GAME);
			break;

		case CRanking::SELECT_GAMEMODE_TITLE:
			// 遷移
			CFade::GetInstance()->SetFade(CManager::MODE_TITLE);
			break;

		default:
			assert(false);

			break;
		}
	}

}

//--------------------------------------------------
// ランキングのリセット
//--------------------------------------------------
void CRanking::LoadRanking()
{
	// ファイルオープン
	std::ifstream ifs(pathToJSON);

	if (ifs)
	{// ファイル開いたとき
		nl::json j;	// リストの生成
		ifs >> j;	// リストに文字をぶち込む

		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{//	タグ付け
			std::string name = "RANKING";
			// 数型を文字型に変える
			std::string Number = std::to_string(nCnt);
			name += Number;

			m_aRankingData[nCnt] = j[name];
		}
	}
}

//--------------------------------------------------
// ランキングのセーブ
//--------------------------------------------------
void CRanking::SaveRanking()
{
	nl::json j;	// リストの生成

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{//	タグ付け
		std::string name = "RANKING";
		// 数型を文字型に変える
		std::string Number = std::to_string(nCnt);
		name += Number;

		j[name] = m_aRankingData[nCnt];
	}

	// jobjにデータを入れる
	auto jobj = j.dump();
	std::ofstream writing_file;
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

//--------------------------------------------------
// ランキングの更新
//--------------------------------------------------
void CRanking::SetRanking(int nScore)
{
	LoadRanking();
	m_nMyScore = nScore;
	m_pMyScore->SetScore(m_nMyScore);

	int nCheck = 0;

	//ランキングの最下位と今回のスコアを比べる
	if (nScore >= m_aRankingData[MAX_RANK - 1])
	{
		//比較した数値を入れる
		m_aRankingData[MAX_RANK - 1] = nScore;
	}

	for (int nCntRank1 = 0; nCntRank1 < MAX_RANK - 1; nCntRank1++)
	{
		for (int nCntRnak2 = nCntRank1 + 1; nCntRnak2 < MAX_RANK; nCntRnak2++)
		{//上記の繰り返しが進むたびに繰り返し回数が-1ずつされる。

		 // 二つのデータを比較する
			if (m_aRankingData[nCntRank1] <= m_aRankingData[nCntRnak2])
			{
				//変数を一時格納
				nCheck = m_aRankingData[nCntRnak2];

				//比較した数値を入れる
				m_aRankingData[nCntRnak2] = m_aRankingData[nCntRank1];
				m_aRankingData[nCntRank1] = nCheck;
			}
		}
	}

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (m_aRankingData[nCntRank] == nScore)
		{
			m_pHighScore[nCntRank]->SetLight(true);
		}

		m_pHighScore[nCntRank]->SetScore(m_aRankingData[nCntRank]);
	}

	// ランキングのセーブ
	SaveRanking();
}

//--------------------------------------------------
// ランキングの設定処理
//--------------------------------------------------
CRanking *CRanking::Create()
{
	CRanking *pRanking;
	pRanking = new CRanking;

	if (pRanking != nullptr)
	{
		pRanking->Init();
	}
	else
	{
		assert(false);
	}

	return pRanking;
}
