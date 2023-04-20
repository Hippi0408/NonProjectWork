//==================================================
// enemy.h
// Author: Arita Meigen
//==================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include"object2D.h"

class CEnemy :public CObject2D
{
public:
	enum MOVEPATTERN
	{//�ړ���
		MOVE_UP_STRAIGHT = 0,	//��ɒ��i
		MOVE_DOWN_STRAIGHT,		//���ɒ��i
		MOVE_LEFT_STRAIGHT,		//���ɒ��i
		MOVE_RIGHT_STRAIGHT,	//�E�ɒ��i
		MOVE_UPLEFT_STRAIGHT,	//����ɒ��i
		MOVE_DOWNLEFT_STRAIGHT,	//�����ɒ��i
		MOVE_UPRIGHT_STRAIGHT,	//�E��ɒ��i
		MOVE_DOWNRIGHT_STRAIGHT,//�E���ɒ��i
		MOVE_MAX
	};
	enum SIZE_PATTERN
	{//�T�C�Y�p�^�[��
		SIZEPTN_NORMAL = 0,		//�����`
		SIZEPTN_ELASTICITY,		//�L�k
		SIZEPTN_MAX
	};
	enum ENEMY_TYPE
	{//�G�̃p�^�[��
		ENEMY_NORMAL,		//���E�㉺�̒ʏ�ړ�
		ENEMY_ELASTICITY,	//�L�k
		ENEMY_OBLIQUE,		//�΂߈ړ�
		ENEMY_ELA_OBL,		//�΂߈ړ��ƐL�k����
		ENEMY_INVINCIBLE	//���G
	};

	explicit CEnemy(int nPriority = PRIORITY_OBJECT);
	~CEnemy();
	HRESULT Init()override;										// ������
	void Update()override;										// �X�V
	static void Create(D3DXVECTOR3 pos, MOVEPATTERN moveptn, SIZE_PATTERN sizeptn);	// ����
	bool Collision();											// �Փ�
	void SetMovePattern(MOVEPATTERN moveptn);					// �s���p�^�[���̐ݒ�
	void SetSizePattern(SIZE_PATTERN sizeptn);					// �T�C�Y�p�^�[���̐ݒ�
	void ScreenOut();											// ��ʊO�̏���
	void SetEnemyType();
	void SelectTexture();										//�e�N�X�`���̑I��
private:
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_movesize;		//�T�C�Y�̈ړ��ʁH
	MOVEPATTERN m_moveptn;		//�ړ��p�^�[��
	SIZE_PATTERN m_movesizeptn;		//�ړ��p�^�[��

	ENEMY_TYPE	m_EnemyType;		//�G�̃p�^�[��
	bool m_bGulez;//�O���C�Y

};
#endif // !_ENEMY_H_
