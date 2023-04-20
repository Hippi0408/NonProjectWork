//==================================================
// player.h
// Author: Buriya Kota
//==================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

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
class CPlayer : public CObject2D
{
public:
	enum PLAYER_STATE
	{
		PLAYER_STATE_NONE = 0,
		PLAYER_STATE_UP,
		PLAYER_STATE_DOWN,
		PLAYER_STATE_RIGHT,
		PLAYER_STATE_LEFT,
		PLAYER_STATE_MAX
	};

	static const int INVINCIBLE_ITEM = 300;

	explicit CPlayer(int nPriority = PRIORITY_PLAYER);
	~CPlayer();

	HRESULT Init() override;
	void Update() override;

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	void SetInvincible(bool bInvincible) { m_bInvincible = bInvincible; }
	bool GetInvincible() { return m_bInvincible; }

private:
	void StartMove_();
	void Reflection_();
	void Control_();
	void ReflectionEffect_0(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec);
	void ReflectionEffect_1(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec);
	void ReflectionEffect_2(const D3DXVECTOR3& pos, const D3DXVECTOR3& vec);
	

private:
	// �ړ���
	D3DXVECTOR3 m_move;
	// �o�E���h��
	int m_nReflectionCount;
	// ���ǂ̕ǂɓ���������
	PLAYER_STATE m_playerState;
	//���G��Ԃ̗L��
	bool m_bInvincible;
	//���G����
	int m_nInvincibleTime;
};


#endif	// _PLAYER_H_