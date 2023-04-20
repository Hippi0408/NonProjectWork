//==================================================
// score.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"
#include "score_game.h"
#include "number.h"
#include "manager.h"
#include "sound.h"

#include "add_score_effect.h"

//**************************************************
// �ÓI�����o�ϐ�
//**************************************************

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CScoreGame::CScoreGame(int nPriority /* =4 */) : CScore(nPriority)
{
	m_nScore = 0;
	m_digits = 0;
	SetType(TYPE_SCORE);
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CScoreGame::~CScoreGame()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CScoreGame::Init()
{
	return S_OK;
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CScoreGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	m_nScore = 0;
	m_nTime = 0;

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(size.x * nCnt + pos.x, pos.y, 0.0f), size, PRIORITY_BG);
		m_pNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pNumber[nCnt]->SetTexture(CTexture::TEXTURE_NUMBER);
	}

	m_digits = 1;

	SetPos(pos, size);
	
	SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.75f));
	SetScore(0);
	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CScoreGame::Uninit()
{
	CScore::Uninit();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CScoreGame::Update()
{
	CScore::Update();
}

//--------------------------------------------------
// �ʒu�̐ݒ�Ƒ傫���̐ݒ�
//--------------------------------------------------
void CScoreGame::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	float a = (MAX_SCORE - m_digits);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if ((nCnt - a) >= 0)
		{
			m_pNumber[nCnt]->SetPos(D3DXVECTOR3(size.x * (float)(nCnt - a) + pos.x - size.x * (float)(m_digits -1) * 0.5f, pos.y, 0.0f));
		}
		else
		{
			m_pNumber[nCnt]->SetPos(D3DXVECTOR3(-size.x, 0.0f, 0.0f));
		}
	}
}

//--------------------------------------------------
// �X�R�A�̐ݒ�
//--------------------------------------------------
void CScoreGame::SetScore(int nScore)
{
	m_nScore = nScore;

	if (m_nScore < 0)
	{
		m_nScore = 0;
	}

	int aPosTexU[8];		// �e���̐������i�[

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

	// �e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt]->AnimTexture(aPosTexU[nCnt], 10);
	}

	SetPos(m_pos,m_size);
}

void CScoreGame::AddScore(int nValue)
{
	SetScore(m_nScore + nValue);
	CAddScoreEffect::Create(D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_WALL_HIT);

}

//--------------------------------------------------
// ����
//--------------------------------------------------
CScoreGame *CScoreGame::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScoreGame *pScore;
	pScore = new CScoreGame;

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
