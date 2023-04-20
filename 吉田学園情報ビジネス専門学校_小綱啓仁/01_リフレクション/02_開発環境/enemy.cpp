//==================================================
// enemy.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "object2D.h"
#include "debug_proc.h"
#include"manager.h"
#include "sound.h"
#include "trajectory_effect.h"

#include "game.h"
#include "score_game.h"
#include "utility.h"
#include "wall_hit_effect.h"
#include "add_score_effect.h"

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CEnemy::CEnemy(int nPriority) : CObject2D(nPriority)
{

}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CEnemy::~CEnemy()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CEnemy::Init()
{
	CObject2D::Init();
	m_bGulez = false;
	SetTexture(CTexture::TEXTURE_ENEMY0);
	return S_OK;
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CEnemy::Update()
{
	{
		float scalse = FloatRandam(18.5f, 17.5f);
		D3DXVECTOR3 size = D3DXVECTOR3(scalse, scalse, 0.0f);

		CEffectTrajectory::Create(GetPos() + D3DXVECTOR3(7.5f, 7.5f, 0.0f), size, GetCol() * 0.8f);
		CEffectTrajectory::Create(GetPos() + D3DXVECTOR3(7.5f, -7.5f, 0.0f), size, GetCol() * 0.8f);
		CEffectTrajectory::Create(GetPos() + D3DXVECTOR3(-7.5f, 7.5f, 0.0f), size, GetCol() * 0.8f);
		CEffectTrajectory::Create(GetPos() + D3DXVECTOR3(-7.5f, -7.5f, 0.0f), size, GetCol() * 0.8f);
	}

	CObject2D::Update();
	if (GetSize().x >= 80.0f || GetSize().x <= 40.0f)
	{
		m_movesize *= -1;
	}

	SetSize(GetSize()+ m_movesize);
	MovePos(m_move);	//�ړ�

	if (CGame::GetGame()->GetPlayer() != nullptr)
	{
		if (IsCollisionCircle(GetPos(), 50.0f * 0.5f + 20.0f, CGame::GetGame()->GetPlayer()->GetPos(), 50.0f * 0.5f))
		{
			m_bGulez = true;

			if (IsCollisionCircle(GetPos(), 50.0f * 0.5f, CGame::GetGame()->GetPlayer()->GetPos(), 50.0f * 0.5f))
			{//�{�̂ɔ�e

				if (CGame::GetGame()->GetPlayer()->GetInvincible())
				{
					CGame::GetGame()->GetScore()->AddScore(1);
				}
				else if(!CGame::GetGame()->GetPlayer()->GetInvincible() && m_EnemyType == ENEMY_INVINCIBLE)
				{//���G�A�C�e���̎擾
					CGame::GetGame()->GetPlayer()->SetInvincible(true);
				}
				else
				{
					CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_DEATH);
					CGame::GetGame()->End();
				}

				//�ȉ��̏����𖳎�����
				Uninit();

				//�ȉ��̏����𖳎�����
				return;
			}
		}
		else if(m_bGulez && m_EnemyType != ENEMY_INVINCIBLE)
		{//�O���C�Y����
			CGame::GetGame()->GetScore()->AddScore(1);

			CAddScoreEffect::Create(D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			CAddScoreEffect::Create(D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(7.0f, 7.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			CAddScoreEffect::Create(D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(4.0f, 4.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

			for (int i = 0; i < 200; i++)
			{
				CEffectWallHit::Create(GetPos(), D3DXVECTOR3(FloatRandam(1.0f, -1.0f), FloatRandam(1.0f, -1.0f), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}


			//�G�l�~�[�j��
			Uninit();

			//�ȉ��̏����𖳎�����
			return;
		}
	}

	if (CGame::GetGame()->IsEnd())
	{
		for (int i = 0; i < 200; i++)
		{
			CEffectWallHit::Create(GetPos(), D3DXVECTOR3(FloatRandam(1.0f, -1.0f), FloatRandam(1.0f, -1.0f), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		//�G�l�~�[�j��
		Uninit();
	}

	// ��ʊO����
	ScreenOut();
}

//--------------------------------------------------
// ����
//--------------------------------------------------
void CEnemy::Create(D3DXVECTOR3 pos, MOVEPATTERN moveptn, SIZE_PATTERN sizeptn)
{
	CEnemy*pEnemy = new CEnemy;
	if (pEnemy != nullptr)
	{
		pEnemy->Init();
		pEnemy->SetPos(pos);
		pEnemy->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
		pEnemy->SetMovePattern(moveptn);
		pEnemy->SetSizePattern(sizeptn);
		pEnemy->SetEnemyType();
	}
	else
	{
		assert(false);
	}
}

//--------------------------------------------------
// �Փ�
//--------------------------------------------------
bool CEnemy::Collision()
{
	return false;
}

//--------------------------------------------------
// �ړ��p�^�[���̐ݒ�
//--------------------------------------------------
void CEnemy::SetMovePattern(MOVEPATTERN moveptn)
{
	m_moveptn = moveptn;
	switch (m_moveptn)
	{
	case CEnemy::MOVE_UP_STRAIGHT:
		m_move = D3DXVECTOR3(0.0f, -1.25f, 0.0f);
		break;
	case CEnemy::MOVE_DOWN_STRAIGHT:
		m_move = D3DXVECTOR3(0.0f, 1.25f, 0.0f);
		break;
	case CEnemy::MOVE_LEFT_STRAIGHT:
		m_move = D3DXVECTOR3(-1.25f, 0.0f, 0.0f);
		break;
	case CEnemy::MOVE_RIGHT_STRAIGHT:
		m_move = D3DXVECTOR3(1.25f, 0.0f, 0.0f);
		break;

	case MOVE_UPLEFT_STRAIGHT:	//�E��ɒ��i
		m_move = D3DXVECTOR3(-0.95f, -0.95f, 0.0f);
		break;
	case MOVE_DOWNLEFT_STRAIGHT://�E���ɒ��i
		m_move = D3DXVECTOR3(-0.95f, 0.95f, 0.0f);
		break;
	case MOVE_UPRIGHT_STRAIGHT:	//�E��ɒ��i
		m_move = D3DXVECTOR3(0.95f, -0.95f, 0.0f);
		break;
	case MOVE_DOWNRIGHT_STRAIGHT://�E���ɒ��i
		m_move = D3DXVECTOR3(0.95f, 0.95f, 0.0f);
		break;
	default:
		break;
	}
}

//--------------------------------------------------
// �T�C�Y�p�^�[���̐ݒ�
//--------------------------------------------------
void CEnemy::SetSizePattern(SIZE_PATTERN sizeptn)
{
	m_movesizeptn = sizeptn;
	switch (sizeptn)
	{
	case CEnemy::SIZEPTN_NORMAL:
		SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
		break;
	case CEnemy::SIZEPTN_ELASTICITY:
		m_movesize= D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		break;
	}
}

//--------------------------------------------------
// ��ʊO����
//--------------------------------------------------
void CEnemy::ScreenOut()
{
	if (GetPos().x > CManager::SCREEN_WIDTH + GetSize().x * 0.5f)
	{
		Uninit();
	}
	else if (GetPos().x < -GetSize().x * 0.5f)
	{
		Uninit();
	}
	else if (GetPos().y > CManager::SCREEN_WIDTH + GetSize().y * 0.5f)
	{
		Uninit();
	}
	else if (GetPos().y < -GetSize().y * 0.5f)
	{
		Uninit();
	}
}

//--------------------------------------------------
// �G�̎�ނ�ݒ�
//--------------------------------------------------
void CEnemy::SetEnemyType()
{
	if (m_moveptn == CEnemy::MOVE_UPLEFT_STRAIGHT || m_moveptn == CEnemy::MOVE_DOWNLEFT_STRAIGHT || m_moveptn == CEnemy::MOVE_UPRIGHT_STRAIGHT || m_moveptn == CEnemy::MOVE_DOWNRIGHT_STRAIGHT)
	{//�΂�
		m_EnemyType = ENEMY_OBLIQUE;
		if (m_movesizeptn == ENEMY_ELASTICITY)
		{//�΂�&�L�k
			m_EnemyType = ENEMY_ELA_OBL;
		}
	}
	else if (m_movesizeptn == ENEMY_ELASTICITY)
	{//�L�k
		m_EnemyType = ENEMY_ELASTICITY;
	}
	else
	{//�ʏ�
		if (rand() % 100 + 1 <= 10)
		{
			m_EnemyType = ENEMY_INVINCIBLE;
		}
		else
		{
			m_EnemyType = ENEMY_NORMAL;
		}
	}
	SelectTexture();
}

//--------------------------------------------------
// ��ނɉ����ăe�N�X�`����ݒ�
//--------------------------------------------------
void CEnemy::SelectTexture()
{
	switch (m_EnemyType)
	{
	case CEnemy::ENEMY_NORMAL:
		SetTexture(CTexture::TEXTURE_ENEMY0);
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case CEnemy::ENEMY_ELASTICITY:
		SetTexture(CTexture::TEXTURE_ENEMY1);
		SetCol(D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f));
		break;
	case CEnemy::ENEMY_OBLIQUE:
		SetTexture(CTexture::TEXTURE_ENEMY2);
		SetCol(D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0f));
		break;
	case CEnemy::ENEMY_ELA_OBL:
		SetTexture(CTexture::TEXTURE_ENEMY3);
		SetCol(D3DXCOLOR(0.8f, 1.0f, 0.8f, 1.0f));
		break;
	case CEnemy::ENEMY_INVINCIBLE:
		SetTexture(CTexture::TEXTURE_ENEMY4);
		SetCol(D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f));
		break;
	default:
		assert(false);
		break;
	}
}

