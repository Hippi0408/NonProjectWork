//==================================================
// shadow.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "manager.h"

#include "shadow.h"

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CShadow::CShadow()
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CShadow::~CShadow()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CShadow::Init()
{
	// ������
	CObject3D::Init();

	// �e�N�X�`���̐ݒ�
	CObject3D::SetTexture(CTexture::TEXTURE_SHADOW);

	return S_OK;
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CShadow::Draw(DRAW_MODE drawMode)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	//���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �`��
	CObject3D::Draw(drawMode);

	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CShadow *pShadow = nullptr;
	pShadow = new CShadow;

	if (pShadow != nullptr)
	{
		pShadow->Init();
		pShadow->SetPos(pos);
		pShadow->SetSize(size);
	}
	else
	{
		assert(false);
	}

	return pShadow;
}
