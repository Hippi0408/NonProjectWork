//==================================================
// timer.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"
#include "game.h"
#include "fade.h"

#include "timer.h"
#include "score.h"
#include "number.h"

//**************************************************
// �ÓI�����o�ϐ�
//**************************************************

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CTimer::~CTimer()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CTimer::Init()
{
	return S_OK;
}

//--------------------------------------------------
// �������@�I�[�o�[���[�h
//--------------------------------------------------
HRESULT CTimer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_nTime = 0;
	m_bIsStop = false;
	m_nFrame = 0;
	m_nSceneFrame = 0;

	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(size.x * nCnt + pos.x, pos.y, 0.0f), size);
		m_pNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pNumber[nCnt]->SetTexture(CTexture::TEXTURE_NUMBER);
	}

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CTimer::Uninit()
{
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
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
// �X�V
//--------------------------------------------------
void CTimer::Update()
{
	if (!m_bIsStop)
	{
		//�t���[���J�E���^�[
		m_nCounter++;

		if (m_nCounter >= 60 && m_nTime > 0)
		{
			SubTimer(1);

			m_nCounter = 0;
		}
	}
	else
	{
		m_nFrame++;

		if (m_nFrame >= 60)
		{
			if (m_nTime > 0)
			{
			//	CGame::GetGame()->GetScore()->AddScore(100);
				SubTimer(1);
			}
			else if (m_nTime <= 0)
			{
				m_nSceneFrame++;
				if (m_nSceneFrame >= 60)
				{
					// �J��
					CFade::GetInstance()->SetFade(CManager::MODE_RANKING);
				}
			}
		}
	}
}

//--------------------------------------------------
// �ʒu�̐ݒ�Ƒ傫���̐ݒ�
//--------------------------------------------------
void CTimer::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		m_pNumber[nCnt]->SetPos(D3DXVECTOR3(size.x * nCnt + pos.x, pos.y, 0.0f));
	}
}

//--------------------------------------------------
// �^�C���̐ݒ�
//--------------------------------------------------
void CTimer::SetTimer(int nTime)
{
	m_nTime = nTime;

	int aPosTexU[3];		//�e���̐������i�[

	{
		int timer = m_nTime;
		for (int i = 2; i >= 0; --i)
		{
			aPosTexU[i] = timer % 10;
			timer /= 10;
		}
	}

	// �e�N�X�`�����W�̐ݒ�
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		m_pNumber[nCnt]->AnimTexture(aPosTexU[nCnt], 10);
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CTimer *CTimer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTimer *pTimer;
	pTimer = new CTimer;

	if (pTimer != nullptr)
	{
		pTimer->Init(pos, size);
	}
	else
	{
		assert(false);
	}

	return pTimer;

}
