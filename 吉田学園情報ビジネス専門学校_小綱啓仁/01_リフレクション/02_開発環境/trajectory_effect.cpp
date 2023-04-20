//==================================================
// trajectory_effect.cpp
// Author: Yuda Katio
//==================================================

//**************************************************
// include
//**************************************************
#include "trajectory_effect.h"
#include "utility.h"

const int CEffectTrajectory::MAX_LIFE = 40;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CEffectTrajectory::CEffectTrajectory(int nPriority) : CObject2D(nPriority)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CEffectTrajectory::~CEffectTrajectory()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CEffectTrajectory::Init()
{
	m_life = MAX_LIFE;
	CObject2D::Init();
	SetTexture(CTexture::TEXTURE_ENEMY0);
	return E_NOTIMPL;
}

//--------------------------------------------------
// 行進
//--------------------------------------------------
void CEffectTrajectory::Update()
{
	CObject2D::Update();

	m_life--;

	SetSize(GetSize() * 0.95f);

	D3DXCOLOR col = GetCol();
	col.a *= 0.95f;
	SetCol(col);

	if (m_life <= 0)
	{
		Uninit();
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CEffectTrajectory * CEffectTrajectory::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col)
{
	CEffectTrajectory* effect = new CEffectTrajectory;

	if (effect != nullptr)
	{
		effect->Init();

		D3DXVECTOR3 randamPos = pos;
		randamPos.x += FloatRandam(-2.5f, 2.5f);
		randamPos.y += FloatRandam(-2.5f, 2.5f);

		effect->SetPos(randamPos);
		effect->SetSize(size);
		effect->SetCol(col);
	}

	return effect;
}
