//==================================================
// enemy_advent_effect.cpp
// Author: AritaMeigen
//==================================================

//**************************************************
// include
//**************************************************
#include "enemy_advent_effect.h"
#include "enemy.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CEnemy_Effect::CEnemy_Effect(int nPriority) : CObject2D(nPriority)
{

}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CEnemy_Effect::~CEnemy_Effect()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CEnemy_Effect::Init()
{
	CObject2D::Init();
	m_moveptn = rand() % CEnemy::MOVE_MAX;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	//ノーマルカラー
	SetTexture(CTexture::TEXTURE_ENEMY0);

	//SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//SetCol(D3DXCOLOR(0.8f, 1.0f, 0.8f, 1.0f));
	//SetCol(D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f));

	bool isStraight = (m_moveptn == CEnemy::MOVE_UPLEFT_STRAIGHT || m_moveptn == CEnemy::MOVE_DOWNLEFT_STRAIGHT || m_moveptn == CEnemy::MOVE_UPRIGHT_STRAIGHT || m_moveptn == CEnemy::MOVE_DOWNRIGHT_STRAIGHT);

	if (isStraight && m_sizeptn == CEnemy::SIZEPTN_ELASTICITY)
	{//斜め&伸縮
		SetTexture(CTexture::TEXTURE_ENEMY3);
		m_col = D3DXCOLOR(0.8f, 1.0f, 0.8f, 0.0f);
	}
	else if (m_sizeptn == CEnemy::SIZEPTN_ELASTICITY)
	{//伸縮
		SetTexture(CTexture::TEXTURE_ENEMY1);
		m_col = D3DXCOLOR(1.0f, 0.8f, 0.8f, 0.0f);
	}
	else if (isStraight)
	{//斜め移動
		SetTexture(CTexture::TEXTURE_ENEMY2);		
		m_col = D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.0f);
	}

	return S_OK;
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CEnemy_Effect::Update()
{
	CObject2D::Update();
	m_LifeTimer--;
	m_col.a += m_fAlphagain;
	SetCol(m_col);
	if (m_LifeTimer<=0)
	{
		CEnemy::Create(GetPos(), (CEnemy::MOVEPATTERN)m_moveptn, (CEnemy::SIZE_PATTERN)m_sizeptn);
		Uninit();
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CEnemy_Effect* CEnemy_Effect::Create(D3DXVECTOR3 pos, int Life, CEnemy::SIZE_PATTERN sizptn)
{
	CEnemy_Effect*pEnemy = new CEnemy_Effect;
	if (pEnemy != nullptr)
	{
		pEnemy->m_sizeptn = sizptn;
		pEnemy->Init();
		pEnemy->SetPos(pos);
		pEnemy->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
		pEnemy->m_LifeTimer = Life;
		pEnemy->m_fAlphagain = 1.0f / pEnemy->m_LifeTimer;
	}
	else
	{
		assert(false);
	}
	return pEnemy;
}
