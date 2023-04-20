//==================================================
// score.h
// Author: Buriya Kota
//==================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//**************************************************
// �C���N���[�h
//**************************************************
#include "object.h"

//**************************************************
// �O���O���@���Ԃ�NG�@�|�C���^�����Ȃ�OK
//**************************************************
class CNumber;

//**************************************************
// �N���X
//**************************************************
class CScore : public CObject
{
public:
	static const int MAX_SCORE = 8;
public:
	explicit CScore(int nPriority = PRIORITY_EFFECT);
	~CScore() override;

	HRESULT Init() override;
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override {}

	void SetLight(bool isLight) { m_isLight = isLight; }
	bool IsLight() { return m_isLight; }

	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void SetColor(D3DXCOLOR color);

	void SetScore(int nScore);
	void AddScore(int nValue) { SetScore(m_nScore + nValue); }
	int GetScore() { return m_nScore; }

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

protected:
	// Number�^�̔z��
	CNumber *m_pNumber[MAX_SCORE];
	// �X�R�A�̒l
	int m_nScore;
	// �����L���O�ɏ�������ǂ���
	bool m_isLight;
	// �J�E���g
	int m_nTime;
	// �F
	D3DXCOLOR m_col;

	// ����
	int m_digits;

	// �ʒu
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
};

#endif	// _SCORE_H_