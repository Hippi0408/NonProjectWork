//==================================================
// silhouette.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"
#include "silhouette.h"

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CSilhoette::CSilhoette(int nPriority) : CObject2D(nPriority)
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CSilhoette::~CSilhoette()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CSilhoette::Init()
{
	CObject2D::Init();

	return S_OK;
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CSilhoette::Draw(DRAW_MODE drawMode)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@ -> �L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�ݒ� -> ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// �X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ� -> 0xff(�S�Đ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@ ->
	// �i�Q�ƒl >= �X�e���V���o�b�t�@�̎Q�ƒl�j�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// �X�e���V���e�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);		// Z�ƃX�e���V������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// Z�ƃX�e���V�����s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// Z�̂ݎ��s

	CObject2D::Draw(drawMode);

	// �X�e���V���o�b�t�@ -> ����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CSilhoette *CSilhoette::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CSilhoette *pSilhoette = nullptr;
	pSilhoette = new CSilhoette;

	if (pSilhoette != nullptr)
	{
		pSilhoette->Init();
		pSilhoette->SetPos(pos);
		pSilhoette->SetSize(size);
		pSilhoette->SetCol(col);
	}
	else
	{
		assert(false);
	}

	return pSilhoette;
}
