//==================================================
// manager.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <time.h>
#include <assert.h>

#include "manager.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "model_data.h"
#include "debug_proc.h"
#include "sound.h"

#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectX.h"
#include "model.h"

#include "silhouette.h"

//**************************************************
// �ÓI�����o�ϐ�
//**************************************************
CManager* CManager::m_pManager = nullptr;

//--------------------------------------------------
// �C���X�^���X�̐���
//--------------------------------------------------
CManager * CManager::GetInstanse()
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManager;
	}
	return m_pManager;
}

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CManager::CManager()
{
	m_pRenderer = nullptr;
	m_pTexture = nullptr;
	m_pModelData = nullptr;
	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pDebug = nullptr;
	m_pSound = nullptr;
	m_pInput = nullptr;
	m_pGameMode = nullptr;
	m_mode = MODE_TITLE;
	m_nowScore = 0;
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CManager::~CManager()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CManager::Init(const HINSTANCE hInstance, const HWND hWnd)
{
	// �^�������̏�����
	srand((unsigned int)time(NULL));

	m_pTexture = new CTexture;
	m_pModelData = new CModelData;
	m_pRenderer = new CRenderer;

	// ����������
	if (FAILED(m_pRenderer->Init(hWnd, false)))		// ����FALSE�ɂ���ƃt���X�N���[���ɂȂ�TRUE�̓E�B���h�E���[�h
	{//�����������s�����ꍇ
		return -1;
	}

	// �C���v�b�g�̐���
	m_pInput = CInput::Create();
	// ���͏����̏���������
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	m_pSound = new CSound;

	// ����������
	if (FAILED(m_pSound->Init(hWnd)))
	{//�����������s�����ꍇ
		return -1;
	}

	// ����������
	m_pCamera = new CCamera;
	m_pCamera->Init();

	// ����������
	m_pLight = new CLight;
	m_pLight->Init();

	m_pDebug = new CDebugProc;
	m_pDebug->Init();

	SetMode(MODE_TITLE);

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CManager::Uninit()
{
	// ���ׂẴ����[�X
	CObject::ReleaseAll();

	if (m_pRenderer != nullptr)
	{// �I������
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pInput != nullptr)
	{// �I������
		m_pInput->Uninit();
		m_pInput = nullptr;
	}

	if (m_pTexture != nullptr)
	{// �I������
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pModelData != nullptr)
	{// �I������
		m_pModelData->ReleaseAll();
		delete m_pModelData;
		m_pModelData = nullptr;
	}

	if (m_pCamera != nullptr)
	{// �J�����I������
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// ���C�g�I������
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	// �f�o�b�O�̏I������
	if (m_pDebug != nullptr)
	{
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	if (m_pSound != nullptr)
	{// �I������
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CManager::Update()
{
	// �X�V����
	m_pInput->Update();
	// �J�����̃Z�b�g
	m_pCamera->Update();
	// �X�V����
	m_pRenderer->Update();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CManager::Draw()
{
	// �`�揈��
	m_pRenderer->Draw();
}

//==================================================
// �Q�[�����[�h�̕ύX
//==================================================
void CManager::SetMode(MODE mode)
{
	if (m_pGameMode != nullptr)
	{// �`�F�b�N
		m_pGameMode->Uninit();
		m_pGameMode = nullptr;
	}

	CManager::GetManager()->GetSound()->Stop();
	m_mode = mode;

	CObject::ReleaseWithoutMode();

	//���݂̉��(���[�h)�̏I������
	switch (m_mode)
	{
	case MODE_TITLE:				// �^�C�g�����
		m_pGameMode = CTitle::Create();
		break;

	case MODE_TUTORIAL:
		m_pGameMode = CTutorial::Create();
		break;

	case MODE_GAME:					// �Q�[�����
		m_pGameMode = CGame::Create();
		break;

	case MODE_GAMEOVER:				// �Q�[�����
		break;

	case MODE_RESULT:				// ���U���g���
		m_pGameMode = CResult::Create();
		break;

	case MODE_RANKING:				// ���U���g���
		CRanking *pRanking;

		pRanking = CRanking::Create();
		pRanking->SetRanking(GetNowScore());

		m_pGameMode = pRanking;
		break;

	default:
		assert(false);
		break;
	}
}
