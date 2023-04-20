//==================================================
// tutorial.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"

#include "input.h"
#include "sound.h"

#include "tutorial.h"
#include "fade.h"

#include "object2D.h"

static const float MOVE_TIME = 60.0f;
static const float CHOICE_ICON_HEIGHT = 55.0f;

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CTutorial::CTutorial()
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CTutorial::~CTutorial()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CTutorial::Init()
{
	// �T�E���h
	CManager::GetManager()->GetSound()->Play(CSound::LABEL_BGM_TITLE);

	m_time = 0;

	m_mode = MODE_INPUT;
	m_disp = (TEXTURE)0;

	CTexture::TEXTURE tex[] =
	{
		CTexture::TEXTURE_TUTORIAL_MAP,
		CTexture::TEXTURE_TUTORIAL_MANUAL,
		CTexture::TEXTURE_TUTORIAL_OPERATION_METHOD
	};
#define ARRAY_LENGTH(a) (sizeof(a)/sizeof((a)[0])) 
	static_assert(ARRAY_LENGTH(tex) == TEXTURE_MAX, "baka");

	for (int nCntTexture = 0; nCntTexture < TEXTURE_MAX; nCntTexture++)
	{
		// �e�N�X�`��
		m_pObject2D[nCntTexture] = CObject2D::Create(
			D3DXVECTOR3(CManager::SCREEN_WIDTH * (0.5f + nCntTexture), CManager::SCREEN_HEIGHT * 0.5f, 0.0f),
			D3DXVECTOR3((float)CManager::SCREEN_WIDTH, (float)CManager::SCREEN_HEIGHT, 0.0f),
			PRIORITY_BG);
		m_pObject2D[nCntTexture]->SetTexture(tex[nCntTexture]);
	}

	CTexture::TEXTURE texChoice[] =
	{
		CTexture::TEXTURE_CHOICE_R,
		CTexture::TEXTURE_CHOICE_L,
	};
	static_assert(ARRAY_LENGTH(texChoice) == TEXTURE_CHOICE_MAX, "baka");

	// R
	m_pChoice[TEXTURE_R] = CObject2D::Create(
		D3DXVECTOR3((float)CManager::SCREEN_WIDTH - 100.0f, CManager::SCREEN_HEIGHT - CHOICE_ICON_HEIGHT, 0.0f),
		D3DXVECTOR3(100.0f, 100.0f, 0.0f),
		PRIORITY_BG);
	m_pChoice[TEXTURE_R]->SetTexture(texChoice[TEXTURE_R]);

	// L
	m_pChoice[TEXTURE_L] = CObject2D::Create(
		D3DXVECTOR3(100.0f, CManager::SCREEN_HEIGHT - CHOICE_ICON_HEIGHT, 0.0f),
		D3DXVECTOR3(100.0f, 100.0f, 0.0f),
		PRIORITY_BG);
	m_pChoice[TEXTURE_L]->SetTexture(texChoice[TEXTURE_L]);

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CTutorial::Uninit()
{
	CObject::DeletedObj();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CTutorial::Update()
{
	CInput *pInput = CInput::GetKey();

	switch (m_mode)
	{
	case MODE_INPUT:
		if (pInput->Trigger(KEY_RIGHT))
		{// �E�ɍs��
			if (m_disp == (TEXTURE)(TEXTURE_MAX - 1))
			{
				CFade::GetInstance()->SetFade(CManager::MODE_TITLE);
			}
			else
			{
				// �T�E���h
				CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_ENTER);

				m_mode = MODE_SCROLL_L;
				m_disp = (TEXTURE)(m_disp + 1);
				m_time = 0;
			}
		}
		else if (pInput->Trigger(KEY_LEFT))
		{// ���ɍs��
			if (m_disp == 0)
			{
				// �������Ȃ�
			}
			else
			{
				// �T�E���h
				CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_ENTER);

				m_mode = MODE_SCROLL_R;
				m_disp = (TEXTURE)(m_disp - 1);
				m_time = 0;
			}
		}
		else
		{
			m_time++;
			float posAddX = (m_time / 15 % 2) * 25.0f;
			m_pChoice[TEXTURE_R]->SetPos(D3DXVECTOR3((float)CManager::SCREEN_WIDTH - 100.0f + posAddX, CManager::SCREEN_HEIGHT - CHOICE_ICON_HEIGHT, 0.0f));
			m_pChoice[TEXTURE_L]->SetPos(D3DXVECTOR3(100.0f - posAddX, CManager::SCREEN_HEIGHT - CHOICE_ICON_HEIGHT, 0.0f));

			if (m_disp == (TEXTURE)0)
			{
				m_pChoice[TEXTURE_L]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			else
			{
				m_pChoice[TEXTURE_L]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}

		break;

	case MODE_SCROLL_L:

		for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
		{// ���ɃX�N���[��
			D3DXVECTOR3 pos = m_pObject2D[nCnt]->GetPos();

			pos.x -= CManager::SCREEN_WIDTH / MOVE_TIME;

			m_pObject2D[nCnt]->SetPos(pos);
		}

		m_time++;

		if (m_time >= MOVE_TIME)
		{// �X�N���[�����I�������
			m_mode = MODE_INPUT;
		}

		break;

	case MODE_SCROLL_R:
		for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
		{
			D3DXVECTOR3 pos = m_pObject2D[nCnt]->GetPos();

			pos.x += CManager::SCREEN_WIDTH / MOVE_TIME;

			m_pObject2D[nCnt]->SetPos(pos);
		}

		m_time++;

		if (m_time >= MOVE_TIME)
		{// �X�N���[�����I�������
			m_mode = MODE_INPUT;
		}

		break;
	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CTutorial* CTutorial::Create()
{
	CTutorial *pTutorial;
	pTutorial = new CTutorial;

	if (pTutorial != nullptr)
	{
		pTutorial->Init();
	}
	else
	{
		assert(false);
	}

	return pTutorial;
}
