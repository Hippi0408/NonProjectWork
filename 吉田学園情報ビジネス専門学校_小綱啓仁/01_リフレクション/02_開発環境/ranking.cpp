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

// json��include
#include "nlohmann/json.hpp"
#include <fstream>

// ���O�̏ȗ�
namespace nl = nlohmann;
// �t�@�C���p�X�̐ݒ�
const char* pathToJSON = "data/FILE/ranking.json";
// �t�@�C���p�X�̐ݒ�
const char* pathToJsonScore = "data/FILE/my_score.json";

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CRanking::CRanking()
{
	m_nMyScore = 0;
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CRanking::~CRanking()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CRanking::Init()
{
	//CManager::GetManager()->GetSound()->Play(CSound::LABEL_BGM_RANKING);

	CManager::GetManager()->GetCamera()->SetPosV(D3DXVECTOR3(0.0f, 200.0f, -1000.0f));
	CManager::GetManager()->GetCamera()->SetPosR(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �w�i
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

	// �n�C�X�R�A
	m_pHighScoreTex = CObject2D::Create(
		D3DXVECTOR3(500.0f, 100.0f, 0.0f),
		D3DXVECTOR3(400.0f, 300.0f, 0.0f),
		PRIORITY_BG);
	m_pHighScoreTex->SetTexture(CTexture::TEXTURE_HIGH_SCORE);
	m_pHighScoreTex->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �X�R�A
	m_pScoreTex = CObject2D::Create(
		D3DXVECTOR3(500.0f, 200.0f, 0.0f),
		D3DXVECTOR3(400.0f, 300.0f, 0.0f),
		PRIORITY_BG);
	m_pScoreTex->SetTexture(CTexture::TEXTURE_SCORE);
	m_pScoreTex->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_aRankingData[nCntRank] = 0;
		// �n�C�X�R�A
		m_pHighScore[nCntRank] = CScore::Create(D3DXVECTOR3(800.0f, 100.0f + nCntRank * 100.0f, 0.0f), D3DXVECTOR3(60.0f, 80.0f, 0.0f));
		m_pHighScore[nCntRank]->SetScore(m_aRankingData[nCntRank]);
	}

	// �}�C�X�R�A
	m_pMyScore = CScore::Create(D3DXVECTOR3(800.0f, 200.0f, 0.0f), D3DXVECTOR3(60.0f, 80.0f, 0.0f));
	m_pMyScore->SetScore(m_nMyScore);

	// �X�^�[�g
	m_pMenu[0] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 250.0f, 0.0f),
		D3DXVECTOR3(150.0f * 2.0f, 150.0f * 2.0f, 0.0f),
		PRIORITY_BG);
	m_pMenu[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMenu[0]->SetTexture(CTexture::TEXTURE_PAUSE_RESTART);

	// �`���[�g���A��
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
// �I��
//--------------------------------------------------
void CRanking::Uninit()
{
	CObject::DeletedObj();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CRanking::Update()
{
	CInput *pInput = CInput::GetKey();

	if (pInput->Trigger(KEY_UP))
	{
		// �T�E���h
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
		// �T�E���h
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
		// �T�E���h
		CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_ENTER);

		switch (m_nSelect)
		{
		case CRanking::SELECT_GAMEMODE_RESTART:
			// �J��
			CFade::GetInstance()->SetFade(CManager::MODE_GAME);
			break;

		case CRanking::SELECT_GAMEMODE_TITLE:
			// �J��
			CFade::GetInstance()->SetFade(CManager::MODE_TITLE);
			break;

		default:
			assert(false);

			break;
		}
	}

}

//--------------------------------------------------
// �����L���O�̃��Z�b�g
//--------------------------------------------------
void CRanking::LoadRanking()
{
	// �t�@�C���I�[�v��
	std::ifstream ifs(pathToJSON);

	if (ifs)
	{// �t�@�C���J�����Ƃ�
		nl::json j;	// ���X�g�̐���
		ifs >> j;	// ���X�g�ɕ������Ԃ�����

		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{//	�^�O�t��
			std::string name = "RANKING";
			// ���^�𕶎��^�ɕς���
			std::string Number = std::to_string(nCnt);
			name += Number;

			m_aRankingData[nCnt] = j[name];
		}
	}
}

//--------------------------------------------------
// �����L���O�̃Z�[�u
//--------------------------------------------------
void CRanking::SaveRanking()
{
	nl::json j;	// ���X�g�̐���

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{//	�^�O�t��
		std::string name = "RANKING";
		// ���^�𕶎��^�ɕς���
		std::string Number = std::to_string(nCnt);
		name += Number;

		j[name] = m_aRankingData[nCnt];
	}

	// jobj�Ƀf�[�^������
	auto jobj = j.dump();
	std::ofstream writing_file;
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

//--------------------------------------------------
// �����L���O�̍X�V
//--------------------------------------------------
void CRanking::SetRanking(int nScore)
{
	LoadRanking();
	m_nMyScore = nScore;
	m_pMyScore->SetScore(m_nMyScore);

	int nCheck = 0;

	//�����L���O�̍ŉ��ʂƍ���̃X�R�A���ׂ�
	if (nScore >= m_aRankingData[MAX_RANK - 1])
	{
		//��r�������l������
		m_aRankingData[MAX_RANK - 1] = nScore;
	}

	for (int nCntRank1 = 0; nCntRank1 < MAX_RANK - 1; nCntRank1++)
	{
		for (int nCntRnak2 = nCntRank1 + 1; nCntRnak2 < MAX_RANK; nCntRnak2++)
		{//��L�̌J��Ԃ����i�ނ��тɌJ��Ԃ��񐔂�-1�������B

		 // ��̃f�[�^���r����
			if (m_aRankingData[nCntRank1] <= m_aRankingData[nCntRnak2])
			{
				//�ϐ����ꎞ�i�[
				nCheck = m_aRankingData[nCntRnak2];

				//��r�������l������
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

	// �����L���O�̃Z�[�u
	SaveRanking();
}

//--------------------------------------------------
// �����L���O�̐ݒ菈��
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
