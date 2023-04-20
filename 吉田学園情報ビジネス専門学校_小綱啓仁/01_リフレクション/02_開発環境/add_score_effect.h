//==================================================
// �O�ՃG�t�F�N�g
// Author: YudaKaito
//==================================================
#ifndef _ADD_SCORE_EFFECT_H_
#define _ADD_SCORE_EFFECT_H_

//**************************************************
// �C���N���[�h
//**************************************************
#include "object2D.h"

//**************************************************
// �O���O���@���Ԃ�NG�@�|�C���^�����Ȃ�OK
//**************************************************

//**************************************************
// �萔��`
//**************************************************

//**************************************************
// �\���̒�`
//**************************************************

//**************************************************
// �N���X
//**************************************************
class CAddScoreEffect : public CObject2D
{
private:
	static const int MAX_LIFE;
public:
	explicit CAddScoreEffect(int nPriority = PRIORITY_EFFECT0);
	~CAddScoreEffect();

	HRESULT Init() override;
	void Update() override;

	static CAddScoreEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col);

private:
	int m_life;
	D3DXVECTOR3 m_move;
};
#endif	// _WALL_HIT_EFFECT_H_