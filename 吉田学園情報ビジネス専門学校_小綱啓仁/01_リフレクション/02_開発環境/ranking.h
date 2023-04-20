//==================================================
// ranking.h
// Author: Buriya Kota
//==================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//**************************************************
// �C���N���[�h
//**************************************************
#include "game_mode.h"
#include "texture.h"

//**************************************************
// �O���O���@���Ԃ�NG�@�|�C���^�����Ȃ�OK
//**************************************************
class CObject2D;
class CScore;

//**************************************************
// �N���X
//**************************************************
class CRanking : public CGameMode
{
public:
	static const int MAX_RANK = 1;

public:
	enum SELECT_GAMEMODE
	{
		SELECT_GAMEMODE_RESTART = 0,
		SELECT_GAMEMODE_TITLE,
		SELECT_GAMEMODE_MAX
	};

	CRanking();
	~CRanking() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	void LoadRanking();
	void SaveRanking();
	void SetRanking(int nScore);

	static CRanking *Create();

private:
	// �I�u�W�F�N�g2D�̔�
	CObject2D *m_pRanking;
	// �����ق����{�^��
	CObject2D *m_pPress;
	// �����ق����{�^��
	CObject2D *m_pScoreTex;
	// �����ق����{�^��
	CObject2D *m_pHighScoreTex;

	// �n�C�X�R�A
	CScore *m_pHighScore[MAX_RANK];
	// �}�C�X�R�A
	CScore *m_pMyScore;
	// �ʒu
	D3DXVECTOR3 m_pos;
	// �傫��
	D3DXVECTOR3 m_size;
	// �X�R�A�ۑ��ϐ�
	int m_aRankingData[MAX_RANK];
	// �X�R�A
	int m_nMyScore;
	// �I�����Ă��郂�[�h�̃J�E���g
	int m_nSelect;
	CObject2D *m_pMenu[2];
};

#endif	// _RESULT_H_