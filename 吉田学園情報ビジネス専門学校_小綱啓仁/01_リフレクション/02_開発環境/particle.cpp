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
// コンストラクタ
//--------------------------------------------------
CParticle::CParticle(int nPriority) : CObject3D(nPriority)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CParticle::~CParticle()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CParticle::Init()
{
	CObject3D::Init();

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CParticle::Uninit()
{
	CObject3D::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CParticle::Update()
{
	CObject3D::Update();

	// 寿命
	m_life--;

	if (m_life < 0)
	{
		Uninit();
		return;
	}

	D3DXVECTOR3 size = GetSize();
	if (size.x < 0.0f || size.y < 0.0f)
	{// 大きさが0より小さくなったとき
		Uninit();
		return;
	}

	// パーティクルの動きの設定
	MovePos(m_posMove);
	SetRot(GetRot() + m_rotMove);
	SetSize(GetSize() + m_sizeMove);
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CParticle::Draw(DRAW_MODE drawMode)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zバッファに関わらず描画
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);//Aテスト
	//デフォルトはfalseです
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000000);//色せってい
	//これは消したいいろを選択します
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);//より大きい

	CObject3D::Draw(drawMode);

	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zテストの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//--------------------------------------------------
// 生成
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
