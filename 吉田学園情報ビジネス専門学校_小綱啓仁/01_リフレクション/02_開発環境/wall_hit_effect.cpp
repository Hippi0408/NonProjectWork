//==================================================
// trajectory_effect.cpp
// Author: Yuda Katio
//==================================================

//**************************************************
// include
//**************************************************
#include "wall_hit_effect.h"
#include "utility.h"

const int CEffectWallHit::MAX_LIFE = 60;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CEffectWallHit::CEffectWallHit(int nPriority) : CObject2D(nPriority)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CEffectWallHit::~CEffectWallHit()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CEffectWallHit::Init()
{
	m_life = MAX_LIFE;
	CObject2D::Init();
	return E_NOTIMPL;
}

//--------------------------------------------------
// 行進
//--------------------------------------------------
void CEffectWallHit::Update()
{
	CObject2D::Update();

	m_life--;

	SetSize(GetSize() * 0.955f);

	D3DXCOLOR col = GetCol();
	col.a *= 0.99f;
	SetCol(col);

	if (m_life <= 0)
	{
		Uninit();
	}

	SetPos(GetPos() + m_move);
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CEffectWallHit * CEffectWallHit::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 vec, const D3DXCOLOR col)
{
	D3DXVECTOR3 size(D3DXVECTOR3(FloatRandam(27.5f, 12.5f), FloatRandam(27.5f, 12.5f), 0.0f));

	CEffectWallHit* effect = new CEffectWallHit;

	if (effect != nullptr)
	{
		effect->Init();

		D3DXVECTOR3 randamPos = pos;
		randamPos.x += FloatRandam(5.5f, -5.5f);
		randamPos.y += FloatRandam(5.5f, -5.5f);

		effect->SetPos(randamPos);
		effect->SetSize(size);
		effect->SetCol(col);

		effect->m_move = vec * FloatRandam(-2.5f, 12.5f);
	}

	return effect;
}
