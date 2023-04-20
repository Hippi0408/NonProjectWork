//==================================================
// trajectory_effect.cpp
// Author: Yuda Katio
//==================================================

//**************************************************
// include
//**************************************************
#include "add_score_effect.h"
#include "utility.h"

const int CAddScoreEffect::MAX_LIFE = 60;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CAddScoreEffect::CAddScoreEffect(int nPriority) : CObject2D(nPriority)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CAddScoreEffect::~CAddScoreEffect()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CAddScoreEffect::Init()
{
	m_life = MAX_LIFE;
	CObject2D::Init();

	SetTexture(CTexture::TEXTURE_SCORE_EFFECT);

	return E_NOTIMPL;
}

//--------------------------------------------------
// 行進
//--------------------------------------------------
void CAddScoreEffect::Update()
{
	CObject2D::Update();

	m_life--;

	SetSize(GetSize() * 1.15f);

	D3DXCOLOR col = GetCol();
	col.a *= 0.99f;
	SetCol(col);

	if (m_life <= 0)
	{
		Uninit();
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CAddScoreEffect * CAddScoreEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col)
{
	CAddScoreEffect* effect = new CAddScoreEffect;

	if (effect != nullptr)
	{
		effect->Init();

		effect->SetPos(pos);
		effect->SetSize(size);
		effect->SetCol(col);

	}

	return effect;
}
