//==================================================
// particle.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"

#include "particle.h"

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CParticle::CParticle(int nPriority) : CObject3D(nPriority)
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CParticle::~CParticle()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CParticle::Init()
{
	CObject3D::Init();

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CParticle::Uninit()
{
	CObject3D::Uninit();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CParticle::Update()
{
	CObject3D::Update();

	// ����
	m_life--;

	if (m_life < 0)
	{
		Uninit();
		return;
	}

	D3DXVECTOR3 size = GetSize();
	if (size.x < 0.0f || size.y < 0.0f)
	{// �傫����0��菬�����Ȃ����Ƃ�
		Uninit();
		return;
	}

	// �p�[�e�B�N���̓����̐ݒ�
	MovePos(m_posMove);
	SetRot(GetRot() + m_rotMove);
	SetSize(GetSize() + m_sizeMove);
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CParticle::Draw(DRAW_MODE drawMode)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z�o�b�t�@�Ɋւ�炸�`��
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);//A�e�X�g
	//�f�t�H���g��false�ł�
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000000);//�F�����Ă�
	//����͏������������I�����܂�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);//���傫��

	CObject3D::Draw(drawMode);

	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Z�e�X�g�̐ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int life, int nPriority)
{
	CParticle *pParticle;
	pParticle = new CParticle(nPriority);

	if (pParticle != nullptr)
	{
		pParticle->Init();
		pParticle->SetPos(pos);
		pParticle->SetSize(size);
		pParticle->SetLife(life);
		pParticle->SetBillboard(true);
	}
	else
	{
		assert(false);
	}

	return pParticle;
}
