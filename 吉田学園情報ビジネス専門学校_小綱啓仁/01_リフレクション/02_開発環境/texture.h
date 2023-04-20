//**************************************************
// texture.h
// Author  : katsuki mizuki
//**************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//==================================================
// �C���N���[�h
//==================================================
#include <d3dx9.h>

//==================================================
// ��`
//==================================================
class CTexture
{
public: /* ��` */
	enum TEXTURE
	{
		TEXTURE_PLAYER = 0,						// �v���C���[
		TEXTURE_GROUND,							// �n��
		TEXTURE_SHADOW,							// �e
		TEXTURE_TITLE_BG,						// TITLE_BG
		TEXTURE_TITLE,							// TITLE
		TEXTURE_TUTORIAL,						// TUTORIAL
		TEXTURE_START,							// START
		TEXTURE_RESULT,							// RESULT
		TEXTURE_NUMBER,							// �^�C��
		TEXTURE_SNOW,							// ��
		TEXTURE_SNOW_GROUND,					// ��̒n��
		TEXTURE_SKY,							// ��
		TEXTURE_COIN,							// �R�C���擾���̃p�[�e�B�N��
		TEXTURE_CHOICE_R,						// �I���A�C�R��R
		TEXTURE_CHOICE_L,						// �I���A�C�R��L
		TEXTURE_RANKING_NUMBER,					// ����
		TEXTURE_PAUSE_BACK_TITLE,				// �^�C�g����
		TEXTURE_PAUSE_RESTART,					// �͂��߂���
		TEXTURE_PAUSE_CLOSE,					// �Ƃ���
		TEXTURE_PAUSE,							// �|�[�Y
		TEXTURE_PRESS_ENTER_OR_A,				// �����Ăق����{�^��
		TEXTURE_RANKING,						// �����L���O
		TEXTURE_TUTORIAL_OPERATION_METHOD,		// ������@
		TEXTURE_TUTORIAL_MAP,					// �}�b�v
		TEXTURE_TUTORIAL_MANUAL,				// �`���[�g���A��

		TEXTURE_ENEMY0,							// �G�l�~�[
		TEXTURE_ENEMY1,							// �G�l�~�[
		TEXTURE_ENEMY2,							// �G�l�~�[
		TEXTURE_ENEMY3,							// �G�l�~�[
		TEXTURE_ENEMY4,							// �G�l�~�[
		TEXTURE_ENEMY5,							// �G�l�~�[

		TEXTURE_SCORE_EFFECT,

		TEXTURE_SCORE,
		TEXTURE_HIGH_SCORE,

		TEXTURE_MAX,
		TEXTURE_NONE,							// �g�p���Ȃ�
	};

	static const char* s_FileName[];	// �t�@�C���p�X

public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public: /* �����o�֐� */
	void LoadAll();										// �S�Ă̓ǂݍ���
	void Load(TEXTURE inTexture);						// �w��̓ǂݍ���
	void ReleaseAll();									// �S�Ă̔j��
	void Release(TEXTURE inTexture);					// �w��̔j��
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// ���̎擾

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
