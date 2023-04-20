//==================================================
// manager.h
// Author: Buriya Kota
//==================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//**************************************************
// �C���N���[�h
//**************************************************
#include <d3dx9.h>

//**************************************************
// �O���O���@���Ԃ�NG�@�|�C���^�����Ȃ�OK
//**************************************************
class CObject;
class CRenderer;
class CInputKeyboard;
class CTexture;
class CCamera;
class CLight;
class CEnemy3D;
class CInput;
class CDebugProc;
class CPlayer3D;
class CSound;
class CModelData;

//**************************************************
// �N���X
//**************************************************
class CManager
{
public:
	// �X�N���[���̕�
	static const int SCREEN_WIDTH = 1280;
	// �X�N���[���̍���
	static const int SCREEN_HEIGHT = 720;

public:
	static CManager* GetInstanse();	// �}�l�[�W���[�̐�������Ă��Ȃ������琶���B�}�l�[�W���[���̎擾
	static CManager* GetManager() { return m_pManager; }	// �}�l�[�W���[�̏��̎擾
private:
	CManager();
	static CManager* m_pManager;

public:
	//���(���[�h)�̎��
	enum MODE
	{
		MODE_TITLE = 0,		//�^�C�g�����
		MODE_TUTORIAL,		//�`���[�g���A��
		MODE_GAME,			//�Q�[�����
		MODE_GAMEOVER,		//�Q�[���I�[�o�[���
		MODE_RESULT,		//���U���g���
		MODE_RANKING,		//�����L���O
		MODE_MAX
	};

	~CManager();

	HRESULT Init(const HINSTANCE hInstance, const HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	// �����_���[�̏��̎擾
	CRenderer *GetRenderer() { return m_pRenderer; }
	// �L�[�{�[�h�̏��̎擾
	CInput *GetInput() { return m_pInput; }
	// �e�N�X�`���̏��̎擾
	CTexture* GetTexture() { return m_pTexture; }
	// ���f���f�[�^�̏��̎擾
	CModelData* GetModelData() { return m_pModelData; }
	// �J�����̏��̎擾
	CCamera* GetCamera() { return m_pCamera; }
	// ���C�g�̏��̎擾
	CLight* GetLight() { return m_pLight; }
	// �T�E���h���̎擾
	CSound* GetSound() { return m_pSound; }

	void SetNowScore(int score) { m_nowScore = score; }
	int GetNowScore() { return m_nowScore; }

	MODE GetGameMode() { return m_mode; }

	void SetMode(MODE mode);

private:
	// ���݂̃��[�h
	MODE m_mode;
	CObject *m_pGameMode;
	CRenderer *m_pRenderer;
	CTexture *m_pTexture;
	CModelData *m_pModelData;
	CCamera *m_pCamera;
	CLight *m_pLight;
	CDebugProc *m_pDebug;
	CSound *m_pSound;
	CInput *m_pInput;

	// ���݂̃X�R�A
	int m_nowScore;

};

#endif	// _MANAGER_H_