//==================================================
// pause.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "input.h"
#include "sound.h"

#include "pause.h"

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CPause::CPause(int nPriority) : CObject(nPriority)
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CPause::~CPause()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CPause::Init()
{
	m_nSelect = 0;
	m_IsPause = false;

	// BG
	m_pObject2D[0] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT * 0.5f, 0.0f), 
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, (float)CManager::SCREEN_HEIGHT, 0.0f),
		PRIORITY_UI);
	m_pObject2D[0]->SetTexture(CTexture::TEXTURE_NONE);
	m_pObject2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// �^�C�g��
	m_pObject2D[1] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, 100.0f, 0.0f),
		D3DXVECTOR3(400.0f, 400.0f, 0.0f),
		PRIORITY_UI);
	m_pObject2D[1]->SetTexture(CTexture::TEXTURE_PAUSE);
	m_pObject2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// ���X�^�[�g
	m_pMenu[0] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 400.0f, 0.0f),
		D3DXVECTOR3(400.0f, 400.0f, 0.0f),
		PRIORITY_UI);
	m_pMenu[0]->SetTexture(CTexture::TEXTURE_PAUSE_RESTART);
	m_pMenu[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// �^�C�g���ɖ߂�
	m_pMenu[1] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 250.0f, 0.0f),
		D3DXVECTOR3(400.0f, 400.0f, 0.0f),
		PRIORITY_UI);
	m_pMenu[1]->SetTexture(CTexture::TEXTURE_PAUSE_BACK_TITLE);
	m_pMenu[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// ����
	m_pMenu[2] = CObject2D::Create(
		D3DXVECTOR3(CManager::SCREEN_WIDTH * 0.5f, CManager::SCREEN_HEIGHT - 100.0f, 0.0f),
		D3DXVECTOR3(400.0f, 400.0f, 0.0f),
		PRIORITY_UI);
	m_pMenu[2]->SetTexture(CTexture::TEXTURE_PAUSE_CLOSE);
	m_pMenu[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CPause::Uninit()
{
	CObject::DeletedObj();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CPause::Update()
{
	CInput *pInput = CInput::GetKey();

	if (((pInput->Trigger(DIK_P) || pInput->Trigger(JOYPAD_START, 0)) && !m_IsPause))
	{
		m_IsPause = true;

		m_pObject2D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		m_pObject2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		for (int nCntMenu = 0; nCntMenu < 3; nCntMenu++)
		{
			m_pMenu[nCntMenu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (((pInput->Trigger(DIK_P) || pInput->Trigger(JOYPAD_START, 0)) && m_IsPause))
	{
		m_nSelect = 0;
		m_IsPause = false;

		for (int nCntObject = 0; nCntObject < 2; nCntObject++)
		{
			m_pObject2D[nCntObject]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		for (int nCntMenu = 0; nCntMenu < 3; nCntMenu++)
		{
			m_pMenu[nCntMenu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	if (m_IsPause)
	{
		if (pInput->Trigger(KEY_UP))
		{
			// �T�E���h
			CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_SELECT);

			m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

			m_nSelect--;
			if (m_nSelect < 0)
			{
				m_nSelect = 2;
			}

			m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (pInput->Trigger(KEY_DOWN))
		{
			// �T�E���h
			CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_SELECT);

			m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

			m_nSelect++;
			if (m_nSelect >= 3)
			{
				m_nSelect = 0;
			}

			m_pMenu[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (pInput->Trigger(KEY_DECISION))
		{
			// �T�E���h
			CManager::GetManager()->GetSound()->Play(CSound::LABEL_SE_ENTER);

			m_IsPause = false;

			// �I�΂ꂽ���[�h�̏���
			switch (m_nSelect)
			{
			case CPause::SELECT_MODE_RESTART:
				// �J��
				CFade::GetInstance()->SetFade(CManager::MODE_GAME);
				break;

			case CPause::SELECT_MODE_TAITLE:
				// �J��
				CFade::GetInstance()->SetFade(CManager::MODE_TITLE);
				break;

			case CPause::SELECT_MODE_CLOSE:
				for (int nCntObject = 0; nCntObject < 2; nCntObject++)
				{
					m_pObject2D[nCntObject]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				for (int nCntMenu = 0; nCntMenu < 3; nCntMenu++)
				{
					m_pMenu[nCntMenu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				break;

			default:
				assert(false);
				break;
			}
		}
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CPause * CPause::Create()
{
	CPause *pPause;
	pPause = new CPause;

	if (pPause != nullptr)
	{
		pPause->Init();
	}
	else
	{
		assert(false);
	}

	return pPause;
}
