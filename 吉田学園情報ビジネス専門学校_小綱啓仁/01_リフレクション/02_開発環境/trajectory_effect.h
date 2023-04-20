//==================================================
// �O�ՃG�t�F�N�g
// Author: YudaKaito
//==================================================
#ifndef _TRAJECTORY_EFFECT_H_
#define _TRAJECTORY_EFFECT_H_

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
class CEffectTrajectory : public CObject2D
{
private:
	static const int MAX_LIFE;
public:
	explicit CEffectTrajectory(int nPriority = PRIORITY_EFFECT0);
	~CEffectTrajectory();

	HRESULT Init() override;
	void Update() override;

	static CEffectTrajectory *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const D3DXCOLOR col);

private:
	int m_life;
};
#endif	// _TRAJECTORY_EFFECT_H_