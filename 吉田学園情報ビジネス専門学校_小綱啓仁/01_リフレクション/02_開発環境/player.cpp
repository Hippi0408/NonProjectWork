//==================================================
// player.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "utility.h"
#include "debug_proc.h"

#include "manager.h"
#include "input.h"

#include "player.h"

#include "game.h"
#include "score_game.h"

// �G�t�F�N�g
#include "trajectory_effect.h"
#include "wall_hit_effect.h"

#define SPEED		(7.0f)
#define CURVE		(1.2f)
//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CPlayer::CPlayer(int nPriority) : CObject2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nReflectionCount = 0;
	m_playerState = PLAYER_STATE_NONE;
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CPlayer::~CPlayer()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CPlayer::Init()
{
	CObject2D::Init();

	StartMove_();

	SetTexture(CTexture::TEXTURE_PLAYER);

	m_bInvincible = false;

	m_nInvincibleTime = 0;

	return S_OK;
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CPlayer::Update()
{
	CObject2D::Update();

	Reflection_();
	Control_();

	D3DXVECTOR3 pos = GetPos();
	pos += m_move * (SPEED + (m_nReflectionCount * 0.05f));
	SetPos(pos);

	if (m_bInvincible)
	{
		m_nInvincibleTime++;

		if (INVINCIBLE_ITEM - 10 < m_nInvincibleTime)
		{
			for (int i = 0; i < 25; i++)
			{
				CEffectWallHit::Create(pos, D3DXVECTOR3(FloatRandam(1.0f, -1.0f), FloatRandam(1.0f, -1.0f), 0.0f), D3DXCOLOR(FloatRandam(1.0f, 0.0f), FloatRandam(1.0f, 0.0f), FloatRandam(1.0f, 0.0f), 1.0f));
			}

		}

		if (INVINCIBLE_ITEM < m_nInvincibleTime)
		{
			m_nInvincibleTime = 0;

			m_bInvincible = false;
		}
	}


	//���艟���G�t�F�N�g /*���Ȃ���*/
	if (!m_bInvincible)
	{
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 12.0f, 0.0f);
		CEffectTrajectory::Create(pos + D3DXVECTOR3(10.0f, 10.0f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(10.0f, -10.0f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(-10.0f, 10.0f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(-10.0f, -10.0f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(7.5f, 7.5f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(7.5f, -7.5f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(-7.5f, 7.5f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(-7.5f, -7.5f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		SetCol(D3DXCOLOR(FloatRandam(0.0f,1.0f), FloatRandam(0.0f, 1.0f), FloatRandam(0.0f, 1.0f), 1.0f));

		D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 12.0f, 0.0f);
		CEffectTrajectory::Create(pos + D3DXVECTOR3(10.0f, 10.0f, 0.0f), size, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(10.0f, -10.0f, 0.0f), size, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(-10.0f, 10.0f, 0.0f), size, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(-10.0f, -10.0f, 0.0f), size, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(7.5f, 7.5f, 0.0f), size, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(7.5f, -7.5f, 0.0f), size, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(-7.5f, 7.5f, 0.0f), size, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		CEffectTrajectory::Create(pos + D3DXVECTOR3(-7.5f, -7.5f, 0.0f), size, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CPlayer *CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->Init();
		pPlayer->SetPos(pos);
		pPlayer->SetSize(size);
	}
	else
	{
		assert(false);
	}

	return pPlayer;
}

//--------------------------------------------------
// �v���C���[�̐���
//--------------------------------------------------
void CPlayer::StartMove_()
{
	if ((m_move.x == 0.0f) && (m_move.y == 0.0f))
	{
		m_move.x = FloatRandam(1.0f, -1.0f);
		m_move.y = FloatRandam(1.0f, -1.0f);
	}

	D3DXVec3Normalize(&m_move, &m_move);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
void CPlayer::Reflection_()
{
	D3DXVECTOR3 pos = GetPos();

	D3DXVECTOR3 nor(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 size = GetSize();

	{// ����
		if (pos.x < 0.0f + size.x * 0.5f)
		{// ��
			D3DXVECTOR3 effectPos(GetPos());
			effectPos.x -= size.x;

			if (m_playerState == PLAYER_STATE_RIGHT)
			{// ���΂̕ǂɓ���������
				for (int i = 0; i < 200; i++)
				{
					effectPos.y = FloatRandam(0.0f, CManager::SCREEN_HEIGHT);
					ReflectionEffect_2(effectPos, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
				}

				CGame::GetGame()->GetScore()->AddScore(3);
				m_nReflectionCount++;
			}
			else if (m_playerState == PLAYER_STATE_LEFT)
			{// �����ǂɓ���������
				for (int i = 0; i < 200; i++)
				{
					effectPos.y = FloatRandam(0.0f, CManager::SCREEN_HEIGHT);
					ReflectionEffect_0(effectPos, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				}
				m_nReflectionCount++;
			}
			else
			{// ����ȊO
				for (int i = 0; i < 200; i++)
				{
					effectPos.y = FloatRandam(0.0f, CManager::SCREEN_HEIGHT);
					ReflectionEffect_1(effectPos, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
				}

				CGame::GetGame()->GetScore()->AddScore(1);
				m_nReflectionCount++;
			}

			nor.x = 1.0f;
			m_playerState = PLAYER_STATE_LEFT;
		}
		else if (pos.x > CManager::SCREEN_WIDTH - size.x * 0.5f)
		{// �E
			D3DXVECTOR3 effectPos(GetPos());
			effectPos.x += size.x;

			if (m_playerState == PLAYER_STATE_LEFT)
			{// ���΂̕ǂɓ���������
				for (int i = 0; i < 200; i++)
				{
					effectPos.y = FloatRandam(0.0f, CManager::SCREEN_HEIGHT);
					ReflectionEffect_2(effectPos, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
				}

				CGame::GetGame()->GetScore()->AddScore(3);
				m_nReflectionCount++;
			}
			else if (m_playerState == PLAYER_STATE_RIGHT)
			{// �����ǂɓ���������
				for (int i = 0; i < 200; i++)
				{
					effectPos.y = FloatRandam(0.0f, CManager::SCREEN_HEIGHT);
					ReflectionEffect_0(effectPos, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				}
				m_nReflectionCount++;
			}
			else
			{// ����ȊO
				for (int i = 0; i < 200; i++)
				{
					effectPos.y = FloatRandam(0.0f, CManager::SCREEN_HEIGHT);
					ReflectionEffect_1(effectPos, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
				}

				CGame::GetGame()->GetScore()->AddScore(1);
				m_nReflectionCount++;
			}
			nor.x = -1.0f;
			m_playerState = PLAYER_STATE_RIGHT;
		}
		else if (pos.y < 0.0f + size.y * 0.5f)
		{// ��
			D3DXVECTOR3 effectPos(GetPos());
			effectPos.y -= size.y;
			if (m_playerState == PLAYER_STATE_DOWN)
			{// ���΂̕ǂɓ���������
				for (int i = 0; i < 200; i++)
				{
					effectPos.x = FloatRandam(0.0f, CManager::SCREEN_WIDTH);
					ReflectionEffect_2(effectPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				}

				CGame::GetGame()->GetScore()->AddScore(1);
				m_nReflectionCount++;
			}
			else if (m_playerState == PLAYER_STATE_UP)
			{// �����ǂɓ���������
				for (int i = 0; i < 200; i++)
				{
					effectPos.x = FloatRandam(0.0f, CManager::SCREEN_WIDTH);
					ReflectionEffect_0(effectPos, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				}
				m_nReflectionCount++;
			}
			else
			{// ����ȊO
				for (int i = 0; i < 200; i++)
				{
					effectPos.x = FloatRandam(0.0f, CManager::SCREEN_WIDTH);
					ReflectionEffect_1(effectPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				}

				CGame::GetGame()->GetScore()->AddScore(1);
				m_nReflectionCount++;
			}
			nor.y = 1.0f;
			m_playerState = PLAYER_STATE_UP;
		}
		else if (pos.y > CManager::SCREEN_HEIGHT - size.y * 0.5f)
		{// ��
			D3DXVECTOR3 effectPos(GetPos());
			effectPos.y += size.y;
			if (m_playerState == PLAYER_STATE_UP)
			{// ���΂̕ǂɓ���������
				for (int i = 0; i < 200; i++)
				{
					effectPos.x = FloatRandam(0.0f, CManager::SCREEN_WIDTH);
					ReflectionEffect_2(effectPos, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				}

				CGame::GetGame()->GetScore()->AddScore(1);
				m_nReflectionCount++;
			}
			else if (m_playerState == PLAYER_STATE_DOWN)
			{// �����ǂɓ���������
				for (int i = 0; i < 200; i++)
				{
					effectPos.x = FloatRandam(0.0f, CManager::SCREEN_WIDTH);
					ReflectionEffect_0(effectPos, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				}
				m_nReflectionCount++;
			}
			else
			{// ����ȊO
				for (int i = 0; i < 200; i++)
				{
					effectPos.x = FloatRandam(0.0f, CManager::SCREEN_WIDTH);
					ReflectionEffect_1(effectPos, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				}

				CGame::GetGame()->GetScore()->AddScore(1);
				m_nReflectionCount++;
			}
			nor.y = -1.0f;
			m_playerState = PLAYER_STATE_DOWN;
		}

		D3DXVECTOR3 refrectVector = CalcReflectVector(m_move, nor);
		m_move = refrectVector;

#ifdef _DEBUG
		// �f�o�b�O�\��
		CDebugProc::Print("�o�E���h�� : \n %d \n", m_nReflectionCount);
#endif // _DEBUG

	}

	{// �����߂�
		if (pos.x < 0.0f + size.x * 0.5f)
		{
			pos.x = 0.0f + size.x * 0.5f;
		}
		if (pos.x > CManager::SCREEN_WIDTH - size.x * 0.5f)
		{
			pos.x = CManager::SCREEN_WIDTH - size.x * 0.5f;
		}
		if (pos.y < 0.0f + size.y * 0.5f)
		{
			pos.y = 0.0f + size.y * 0.5f;
		}
		if (pos.y > CManager::SCREEN_HEIGHT - size.y * 0.5f)
		{
			pos.y = CManager::SCREEN_HEIGHT - size.y * 0.5f;
		}
	}

	SetPos(pos);
}

//--------------------------------------------------
// �v���C���[�̈ړ�
//--------------------------------------------------
void CPlayer::Control_()
{
	CInput *pInput = CInput::GetKey();

	D3DXMATRIX mtxRot;
	D3DXMatrixIdentity(&mtxRot);

	//���_�̏㉺
	if (pInput->Press(JOYPAD_L1) || pInput->Press(KEY_LEFT))
	{
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, D3DXToRadian(-CURVE));
	}
	if (pInput->Press(JOYPAD_R1) || pInput->Press(KEY_RIGHT))
	{
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, D3DXToRadian(CURVE));
	}

	D3DXVec3TransformCoord(&m_move, &m_move, &mtxRot);
}

//--------------------------------------------------
// �v���C���[�����˕Ԃ������ɐ�����G�t�F�N�g�Q
//--------------------------------------------------
void CPlayer::ReflectionEffect_0(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec)
{
	CEffectWallHit::Create(pos, vec, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
}

void CPlayer::ReflectionEffect_1(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & vec)
{
	CEffectWallHit::Create(pos, vec, D3DXCOLOR(FloatRandam(1.0f, 0.0f), FloatRandam(1.0f, 0.0f), FloatRandam(1.0f, 0.0f), 1.0f));
}

void CPlayer::ReflectionEffect_2(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & vec)
{
	D3DXCOLOR color;
	switch (rand() % 3)
	{
	case 0:
		color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 1:
		color = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 2:
		color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}

	CEffectWallHit::Create(pos, vec, color);
}
