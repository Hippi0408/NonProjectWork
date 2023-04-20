//==================================================
// �O�ՃG�t�F�N�g
// Author: YudaKaito
//==================================================
#ifndef _EFFECT_DEAD_H_
#define _EFFECT_DEAD_H_

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
class CEffectDead : public CObject2D
{
private:
	static const int MAX_LIFE;
public:
	explicit CEffectDead(int nPriority = PRIORITY_EFFECT0);
	~CEffectDead();

	HRESULT Init() override;
	void Update() override;

	static CEffectDead *Create(const D3DXVECTOR3 pos, const D3DXCOLOR col);

private:
	int m_life;
};
#endif	// _TRAJECTORY_EFFECT_H_