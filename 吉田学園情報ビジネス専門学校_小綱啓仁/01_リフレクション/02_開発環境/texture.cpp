//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
const char* CTexture::s_FileName[] =
{// �e�N�X�`���̃p�X
	"data/TEXTURE/player.png",		// �v���C���[
	"data/TEXTURE/thumbnail.png",			// �n��
	"data/TEXTURE/shadow000.jpg",			// �e
	"data/TEXTURE/bg_tile.png",				// TITLE_BG
	"data/TEXTURE/TITLE.png",				// TITLE
	"data/TEXTURE/TUTORIAL.png",			// TUTORIAL
	"data/TEXTURE/START.png",				// START
	"data/TEXTURE/RESULT.png",				// RESULT
	"data/TEXTURE/number.png",				// �^�C��
	"data/TEXTURE/snow1.png",				// ��
	"data/TEXTURE/snow_ground.png",			// ��̒n��
	"data/TEXTURE/sky.jpg",					// ��
	"data/TEXTURE/Glitter000.jpg",			// �R�C��
	"data/TEXTURE/choice_R.png",			// �I���A�C�R��R
	"data/TEXTURE/choice_L.png",			// �I���A�C�R��L
	"data/TEXTURE/ranking_number.png",		// ����
	"data/TEXTURE/back_title.png",			// �^�C�g����
	"data/TEXTURE/restart.png",				// �͂��߂���
	"data/TEXTURE/close.png",				// �Ƃ���
	"data/TEXTURE/pause.png",				// �|�[�Y
	"data/TEXTURE/press_enter_or_a.png",	// �����Ăق����{�^��
	"data/TEXTURE/ranking.png",				// �����L���O
	"data/TEXTURE/TUTORIAL03.png",		// ������@
	"data/TEXTURE/TUTORIAL01.png",			// �}�b�v
	"data/TEXTURE/TUTORIAL02.png",				// �`���[�g���A��
	"data/TEXTURE/enemy00.png",			// �G�l�~�[
	"data/TEXTURE/enemy01.png",			// �G�l�~�[
	"data/TEXTURE/enemy02.png",			// �G�l�~�[
	"data/TEXTURE/enemy03.png",			// �G�l�~�[
	"data/TEXTURE/invincible_item.png",	// �G�l�~�[
	"data/TEXTURE/enemy05.png",			// �G�l�~�[
	"data/TEXTURE/score_effect.png",	// �X�R�A�G�t�F�N�g
	"data/TEXTURE/score.png",			// �X�R�A
	"data/TEXTURE/high_score.png",		// �n�C�X�R�A
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//--------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// �S�Ă̓ǂݍ���
//--------------------------------------------------
void CTexture::LoadAll()
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// �e�N�X�`���̓ǂݍ��݂�����Ă���
			continue;
		}

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// �e�N�X�`���̓ǂݍ��݂�����Ă���
		return;
	}

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//--------------------------------------------------
// �S�Ẳ��
//--------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// �e�N�X�`���̉��
			s_pTexture[i]->Release();
			s_pTexture[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// ���
//--------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// �e�N�X�`���̉��
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = nullptr;
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// �e�N�X�`�����g�p���Ȃ�
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// �ǂݍ���
	Load(inTexture);

	return s_pTexture[inTexture];
}
