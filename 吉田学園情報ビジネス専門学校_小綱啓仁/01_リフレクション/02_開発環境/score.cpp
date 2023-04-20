//==================================================
// score.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"
#include "score.h"
#include "number.h"

//**************************************************
// 静的メンバ変数
//**************************************************

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CScore::CScore(int nPriority /* =4 */) : CObject(nPriority)
{
	m_nScore = 0;
	SetType(TYPE_SCORE);
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CScore::~CScore()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CScore::Init()
{
	return S_OK;
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	m_nScore = 0;
	m_nTime = 0;

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(size.x * nCnt + pos.x, pos.y, 0.0f), size);
		m_pNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pNumber[nCnt]->SetTexture(CTexture::TEXTURE_NUMBER);
	}

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CScore::Uninit()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_pNumber[nCnt] == nullptr)
		{
			continue;
		}

		m_pNumber[nCnt]->Uninit();
	}

	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CScore::Update()
{
	//ENTERの点滅
	m_nTime++;
	m_nTime %= 61;

	if (m_nTime > 31)
	{
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}

	if (m_isLight)
	{
		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			m_pNumber[nCnt]->SetCol(m_col);
		}
	}
}

//--------------------------------------------------
// 位置の設定と大きさの設定
//--------------------------------------------------
void CScore::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	float a = (float)(MAX_SCORE - m_digits);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if ((nCnt - a) >= 0)
		{
			m_pNumber[nCnt]->SetPos(D3DXVECTOR3(size.x * (float)(nCnt - a) + pos.x - size.x * (float)(m_digits - 1) * 0.5f, pos.y, 0.0f));
		}
		else
		{
			m_pNumber[nCnt]->SetPos(D3DXVECTOR3(-size.x, 0.0f, 0.0f));
		}
	}
}

//--------------------------------------------------
// 色の設定
//--------------------------------------------------
void CScore::SetColor(D3DXCOLOR color)
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt]->SetCol(color);
	}
}

//--------------------------------------------------
// スコアの設定
//--------------------------------------------------
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;

	if (m_nScore < 0)
	{
		m_nScore = 0;
	}

	int aPosTexU[8];		// 各桁の数字を格納

	m_digits = 0;

	{
		int score = m_nScore;
		for (int nCnt = 7; nCnt >= 0; --nCnt)
		{
			if (score > 0 || m_digits == 0)
			{
				m_digits++;
				m_pNumber[nCnt]->SetCol(D3DXCOLOR(1.5f, 1.5f, 1.5f, 0.75f));
			}
			else
			{
				m_pNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
			}

			aPosTexU[nCnt] = score % 10;
			score /= 10;
		}
	}

	// テクスチャ座標の設定
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt]->AnimTexture(aPosTexU[nCnt], 10);
	}

	SetPos(m_pos, m_size);
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScore *pScore;
	pScore = new CScore;

	if (pScore != nullptr)
	{
		pScore->Init(pos, size);
	}
	else
	{
		assert(false);
	}

	return pScore;
}
