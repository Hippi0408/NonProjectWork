//==================================================
// enemy.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "enemy_manager.h"
#include "enemy.h"
#include "enemy_advent_effect.h"
#include "manager.h"
#include "game.h"

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CEnemyManager::CEnemyManager() 
{
	m_Time = 0;
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CEnemyManager::~CEnemyManager()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CEnemyManager::Init()
{
	m_SizePtn = 1;
	return S_OK;
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CEnemyManager::Update()
{
	if (CGame::GetGame()->IsEnd())
	{
		return;
	}

	m_Time++;	//�^�C�}�[���Z
	if (m_Time % 90 == 0)
	{
		D3DXVECTOR3 pos;
		pos = D3DXVECTOR3((float)(rand() % CManager::SCREEN_WIDTH), (float)(rand() % CManager::SCREEN_HEIGHT), 0.0f);	//���W�w��
		m_SpawnTime = 60;
		m_pObject = CEnemy_Effect::Create(pos, m_SpawnTime, (CEnemy::SIZE_PATTERN)(rand() % m_SizePtn));
	}
	if (m_Time % ERASTIC_TIME == 0)
	{//�ꕪ�o�߂ŏk���o��
		m_SizePtn++;
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CEnemyManager* CEnemyManager::Create()
{
	CEnemyManager*pEnemy = new CEnemyManager;
	if (pEnemy != nullptr)
	{
		pEnemy->Init();
	}
	else
	{
		assert(false);
	}
	return pEnemy;
}
